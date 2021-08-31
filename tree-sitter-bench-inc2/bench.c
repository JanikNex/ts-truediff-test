#include "bench.h"

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "csv_util.h"
#include "libs/tree-sitter/includes/api.h"
#include "libs/tree-sitter/includes/parser.h"
#include "libs/debug.h"
#include "dirs.h"
#include "libs/tree-sitter-python/tokens.h"

#ifdef __linux__
const char *SEPARATOR = "/";
//const char *ROOT_DIR = "/tmp/tmp.aTIwvv2igL/tree-sitter-bench/data";
const char *ROOT_DIR = "/home/janik/python_keras/tmp";
#elif _WIN32
//const char *ROOT_DIR = "C:\\Users\\Janik\\Desktop\\keras_fetch\\python_keras";
const char *ROOT_DIR = "C:\\Users\\Thomas\\Desktop\\truediff-master\\benchmark\\python_keras";
const char *SEPARATOR = "\\";
#endif

const int NS_PER_SECOND = 1000000000;

TSLanguage *tree_sitter_python();

UtilData util_data;

static inline void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td) {
  td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
  td->tv_sec = t2.tv_sec - t1.tv_sec;
  if (td->tv_sec > 0 && td->tv_nsec < 0) {
    td->tv_nsec += NS_PER_SECOND;
    td->tv_sec--;
  } else if (td->tv_sec < 0 && td->tv_nsec > 0) {
    td->tv_nsec -= NS_PER_SECOND;
    td->tv_sec++;
  }
}

RunResult benchmark_run(char *code1, char *code2, EditArray edits) {
  clockid_t clck_id = CLOCK_MONOTONIC;
  struct timespec parse1_start, parse1_stop, parse1_dur, parse2_start, parse2_stop, parse2_dur, init1_start,
    init1_stop, init1_dur, init2_start, init2_stop, init2_dur, diff_start, diff_stop, diff_dur;
  uint64_t parse_time1, parse_time2, init1_time, init2_time, diff_time, total_time;
  double reuse_ratio;
  // PARSE
  clock_gettime(clck_id, &parse1_start);
  TSTree *tree = ts_parser_parse_string(
    util_data.parser,
    NULL,
    code1,
    strlen(code1)
  );
  clock_gettime(clck_id, &parse1_stop);
  sub_timespec(parse1_start, parse1_stop, &parse1_dur);
  parse_time1 = (uint64_t) parse1_dur.tv_sec * NS_PER_SECOND + parse1_dur.tv_nsec;

  // CONTROL TREE
  TSTree *control_tree = ts_parser_parse_string(
    util_data.parser,
    NULL,
    code2,
    strlen(code2)
  );

  ts_diff_heap_initialize(control_tree, code2, util_data.lit_map);
  TSNode control_root = ts_tree_root_node(control_tree);

  // INIT TREE 1

  clock_gettime(clck_id, &init1_start);
  ts_diff_heap_initialize(tree, code1, util_data.lit_map);
  clock_gettime(clck_id, &init1_stop);
  sub_timespec(init1_start, init1_stop, &init1_dur);
  init1_time = (uint64_t) init1_dur.tv_sec * NS_PER_SECOND + init1_dur.tv_nsec;

  // APPLY EDITS
  for (int i = 0; i < edits.size; i++) {
    TSInputEdit this_edit = edits.edits[i];
    //print_input_edit(i,this_edit);
    ts_tree_edit(tree, &this_edit);
  }

  // PARSE TREE 2
  clock_gettime(clck_id, &parse2_start);
  TSTree *tree2 = ts_parser_parse_string(
    util_data.parser,
    tree,
    code2,
    strlen(code2)
  );
  clock_gettime(clck_id, &parse2_stop);
  sub_timespec(parse2_start, parse2_stop, &parse2_dur);
  parse_time2 = (uint64_t) parse2_dur.tv_sec * NS_PER_SECOND + parse2_dur.tv_nsec;

  // TEST INCREMENTALLY PARSED TREE
  TSNode root1 = ts_tree_root_node(tree);
  TSNode root2 = ts_tree_root_node(tree2);
  bool has_err = false;
  if (search_errors(root2)) {
    if (search_errors(control_root)) {
      printf("First parse check found an error, but it matches the control tree!\n");
    } else {
      printf("First parse check found an error!\n");
      has_err = true;
    }
  }
  if (ts_node_end_byte(root2) != ts_node_end_byte(control_root)) {
    printf("Second parse check found an error!\n");
    has_err = true;
  }
  if (!has_err) {
    int reused_nodes = calc_init_reuse(root2);
    reuse_ratio = (double) reused_nodes / control_root.diff_heap->treesize;
  } else {
    reuse_ratio = -1;
  }
  RunResult rr;
  if (!has_err) {
    // INIT TREE 2
    clock_gettime(clck_id, &init2_start);
    ts_diff_heap_initialize(tree2, code2, util_data.lit_map);
    clock_gettime(clck_id, &init2_stop);
    sub_timespec(init2_start, init2_stop, &init2_dur);
    init2_time = (uint64_t) init2_dur.tv_sec * NS_PER_SECOND + init2_dur.tv_nsec;

    // TEST INITIALIZATION OF INCREMENTALLY PARSED TREE
    root2 = ts_tree_root_node(tree2);
    if (ts_incremental_parse_test(control_root, root2)) {
      printf("Third parse check found an error!\n");
      has_err = true;
    }

    rr = (RunResult) {
      .src_size = root1.diff_heap->treesize,
      .src_height = root1.diff_heap->treeheight,
      .dest_size = root2.diff_heap->treesize,
      .dest_height = root2.diff_heap->treeheight,
      .parse1_val = parse_time1,
      .parse2_val = parse_time2,
      .reuse_ratio = reuse_ratio
    };

    // DIFF
    clock_gettime(clck_id, &diff_start);
    TSDiffResult diff_result = ts_compare_to(tree, tree2, code1, code2, util_data.lit_map);
    clock_gettime(clck_id, &diff_stop);
    sub_timespec(diff_start, diff_stop, &diff_dur);
    diff_time = (uint64_t) diff_dur.tv_sec * NS_PER_SECOND + diff_dur.tv_nsec;
    total_time = init1_time + init2_time + diff_time;
    TSNode constructed_tree_root = ts_tree_root_node(diff_result.constructed_tree);

    rr.total_val = total_time;
    rr.init1_val = init1_time;
    rr.init2_val = init2_time;
    rr.diff_val = diff_time;
    rr.edit_script_size = ts_edit_script_length(diff_result.edit_script);
    rr.error = ts_reconstruction_test(root2, constructed_tree_root) || has_err;

    ts_edit_script_delete(diff_result.edit_script);
    ts_diff_heap_delete(diff_result.constructed_tree);
    ts_tree_delete(diff_result.constructed_tree);
  } else {
    rr.error = true;
  }
  // CLEANUP
  ts_diff_heap_delete(control_tree);
  ts_tree_delete(control_tree);

  ts_diff_heap_delete(tree2);
  ts_diff_heap_delete(tree);

  ts_tree_delete(tree2);
  ts_tree_delete(tree);

  return rr;
}


BenchmarkMeasurement benchmark_file(char *base, char *prev_base, char *filename, char *edit_dir, int commit_idx) {
  char prev_file_path[500];
  memset(prev_file_path, 0, 500);
  sprintf(&prev_file_path[0], "%s%s", prev_base, filename);
  char new_file_path[500];
  memset(new_file_path, 0, 500);
  sprintf(&new_file_path[0], "%s%s", base, filename);
  FILE *prev_file = fopen(&prev_file_path[0], "rb");
  if (prev_file == NULL) {
    return (BenchmarkMeasurement) {.valid=false};
  }
  FILE *new_file = fopen(&new_file_path[0], "rb");
  if (new_file == NULL) {
    fclose(prev_file);
    return (BenchmarkMeasurement) {.valid=false};
  }
  char *prev_code_tmp = load_file(prev_file);
  char *new_code_tmp = load_file(new_file);
  if (strcmp(prev_code_tmp, new_code_tmp) == 0) {
    free(prev_code_tmp);
    free(new_code_tmp);
    return (BenchmarkMeasurement) {.valid=false};
  }
  char *prev_code = filter_char_array(prev_code_tmp);
  char *new_code = filter_char_array(new_code_tmp);
  free(prev_code_tmp);
  free(new_code_tmp);

  char edit_file_path[500];
  memset(edit_file_path, 0, 500);
  sprintf(&edit_file_path[0], "%s%d-%s.edits", edit_dir, commit_idx, filename);
  FILE *edit_file = fopen(&edit_file_path[0], "r");
  if (edit_file == NULL) {
    printf("Could not find EditFile: %s\n", edit_file_path);
    free(prev_code);
    free(new_code);
    return (BenchmarkMeasurement) {.valid = false};
  }

  EditArray loaded_edits = load_edits(edit_file);
  fclose(edit_file);

  BenchmarkMeasurement res = {
    .valid = true,
    .error = false
  };
  strncpy(res.path, new_file_path, 500);
  printf("Diff %s\n", new_file_path);
  for (int i = 0; i < REPEAT; i++) {
    RunResult rr = benchmark_run(prev_code, new_code, loaded_edits);
    if (i == 0) {
      res.src_size = rr.src_size;
      res.src_height = rr.src_height;
      res.dest_size = rr.dest_size;
      res.dest_height = rr.dest_height;
      res.edit_script_size = rr.edit_script_size;
      res.reuse_ratio = rr.reuse_ratio;
    }
    res.total_vals[i] = rr.total_val;
    res.init1_vals[i] = rr.init1_val;
    res.init2_vals[i] = rr.init2_val;
    res.diff_vals[i] = rr.diff_val;
    res.parse1_vals[i] = rr.parse1_val;
    res.parse2_vals[i] = rr.parse2_val;
    res.error = res.error || rr.error;
  }
  free(prev_code);
  free(new_code);
  free(loaded_edits.edits);
  return res;
}

BenchmarkResult benchmark() {
  int diffed_files = 0;
  int total_errors = 0;
  struct dirent *commit_dir;
  char prev_commit[260];
  memset(prev_commit, 0, 260);
  char prev_commit_path[500];
  memset(prev_commit_path, 0, 500);
  char this_commit_path[500];
  memset(this_commit_path, 0, 500);
  char edit_dir_path[500];
  memset(edit_dir_path, 0, 500);
  sprintf(&edit_dir_path[0], "%s%sedits%s", ROOT_DIR, SEPARATOR, SEPARATOR);
  FILE *csv_file = create_csv_file("keras_bench_incgrrr.csv");
  write_header(csv_file, REPEAT);
  for (int idx = 0; idx < DIR_SIZE; idx++) {
    printf("=> Directory: %s\n", dir_list[idx]);
    if (idx == 0) {
      strncpy(prev_commit, dir_list[idx], 260);
      sprintf(&prev_commit_path[0], "%s%s%s%s", ROOT_DIR, SEPARATOR, prev_commit, SEPARATOR);
      continue;
    }
    memset(this_commit_path, 0, 500);
    sprintf(&this_commit_path[0], "%s%s%s%s", ROOT_DIR, SEPARATOR, dir_list[idx], SEPARATOR);
    DIR *this_commit_dir = opendir(&this_commit_path[0]);
    if (this_commit_dir == NULL) {
      printf("Could not open %s\n", this_commit_path);
      exit(1);
    }
    while ((commit_dir = readdir(this_commit_dir)) != NULL) {
      if (strcmp(".", commit_dir->d_name) == 0 || strcmp("..", commit_dir->d_name) == 0) {
        continue;
      }
      BenchmarkMeasurement ms = benchmark_file(&this_commit_path[0], &prev_commit_path[0], commit_dir->d_name,
                                               edit_dir_path, idx);
      write_measurement(csv_file, ms);
      if (ms.valid) {
        diffed_files++;
        if (ms.error) {
          total_errors++;
        }
      }
    }
    strncpy(prev_commit_path, this_commit_path, 500);
    strncpy(prev_commit, dir_list[idx], 260);
    closedir(this_commit_dir);
  }
  printf("==================DONE=================\n");
  printf("Diffed files: %d\n", diffed_files);
  printf("Total errors: %d\n", total_errors);
  printf("=======================================\n");
  close_file(csv_file);
}


int main() {
  // Create a parser.
  util_data.parser = ts_parser_new();

  // Set the parser's language.
  ts_parser_set_language(util_data.parser, tree_sitter_python());

  util_data.lit_map = ts_literal_map_create(tree_sitter_python());
  for (int i = 0; i < literal_token_size; i++) {
    ts_literal_map_add_literal(util_data.lit_map, literal_tokens[i]);
  }

  benchmark();


  ts_literal_map_destroy(util_data.lit_map);
  ts_parser_delete(util_data.parser);
}
