#ifndef TREE_SITTER_TEST_BENCH_H
#define TREE_SITTER_TEST_BENCH_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "libs/tree-sitter/includes/api.h"
#include "libs/tree-sitter/includes/parser.h"
#include "libs/debug.h"

#define REPEAT 3

typedef struct {
  int size;
  TSInputEdit *edits;
} EditArray;

typedef struct {
} BenchmarkResult;

typedef struct {
  TSParser *parser;
  TSLiteralMap *lit_map;
} UtilData;

typedef struct {
  unsigned int src_size;
  unsigned int src_height;
  unsigned int dest_size;
  unsigned int dest_height;
  double reuse_ratio;
  uint64_t total_val;
  uint64_t init1_val;
  uint64_t init2_val;
  uint64_t diff_val;
  uint64_t parse1_val;
  uint64_t parse2_val;
  uint32_t edit_script_size;
  bool error;
} RunResult;

typedef struct {
  bool valid;
  char path[500];
  unsigned int src_size;
  unsigned int src_height;
  unsigned int dest_size;
  unsigned int dest_height;
  double reuse_ratio;
  uint64_t total_vals[REPEAT];
  uint64_t init1_vals[REPEAT];
  uint64_t init2_vals[REPEAT];
  uint64_t diff_vals[REPEAT];
  uint64_t parse1_vals[REPEAT];
  uint64_t parse2_vals[REPEAT];
  uint32_t edit_script_size;
  bool error;
} BenchmarkMeasurement;


static inline char *load_file(FILE *file) {
  long lSize;
  char *buffer;

  if (!file) {
    printf("Could not open file!\n");
    exit(1);
  }

  fseek(file, 0L, SEEK_END);
  lSize = ftell(file);
  rewind(file);

/* allocate memory for entire content */
  buffer = calloc(1, lSize + 1);
  if (!buffer) {
    fclose(file);
    printf("Could not allocate buffer!\n");
    exit(1);
  }

/* copy the file into the buffer */
  if (1 != fread(buffer, lSize, 1, file)) {
    fclose(file);
    free(buffer);
    printf("Could not open file!\n");
    exit(1);
  }
  fclose(file);
  return buffer;
}

static inline EditArray load_edits(FILE *file) {
  int buffer;
  int edit_size;
  TSInputEdit *edits;

  if (!file) {
    printf("Could not open file!\n");
    exit(1);
  }

  char chunk[128];
  char *succ;
  int idx = 0;
  int current_edit = -1;
  while (fgets(chunk, sizeof(chunk), file) != NULL) {
    buffer = strtol(chunk, &succ, 10);
    if (current_edit < 0) {
      current_edit = 0;
      edit_size = buffer;
      edits = malloc(buffer * sizeof(TSInputEdit));
    } else {
      switch (idx) {
        case 0:
          edits[current_edit].start_byte = buffer;
          idx++;
          break;
        case 1:
          edits[current_edit].start_point.row = buffer;
          idx++;
          break;
        case 2:
          edits[current_edit].start_point.column = buffer;
          idx++;
          break;
        case 3:
          edits[current_edit].old_end_byte = buffer;
          idx++;
          break;
        case 4:
          edits[current_edit].old_end_point.row = buffer;
          idx++;
          break;
        case 5:
          edits[current_edit].old_end_point.column = buffer;
          idx++;
          break;
        case 6:
          edits[current_edit].new_end_byte = buffer;
          idx++;
          break;
        case 7:
          edits[current_edit].new_end_point.row = buffer;
          idx++;
          break;
        case 8:
          edits[current_edit].new_end_point.column = buffer;
          idx = 0;
          current_edit++;
          break;
        default:
          break;
      }
    }
  }
  return (EditArray) {.size=edit_size, .edits=edits};
}

static inline void print_input_edit(int i, TSInputEdit this_edit) {
  printf("[Edit-%d] Start: %d (%d|%d) || Old End: %d (%d|%d) || New End: %d (%d|%d)\n", i, this_edit.start_byte,
         this_edit.start_point.row, this_edit.start_point.column, this_edit.old_end_byte, this_edit.old_end_point.row,
         this_edit.old_end_point.column, this_edit.new_end_byte, this_edit.new_end_point.row,
         this_edit.new_end_point.column);
}

static inline char *filter_char_array(char *data) {
  int new_len = 0;
  for (int i = 0; data[i] != '\0'; i++) {
    if (data[i] != '\r') {
      data[new_len++] = data[i];
    }
  }
  new_len--;
  data[new_len] = '\0';
  char *tmp = malloc((new_len + 1) * sizeof(char));
  strcpy(tmp, data);
  return tmp;
}

static inline uint64_t find_min(const uint64_t *vals) {
  uint64_t min_val = vals[0];
  for (int i = 1; i < REPEAT; i++) {
    if (vals[i] < min_val) {
      min_val = vals[i];
    }
  }
  return min_val;
}


void write_measurement(FILE *file, BenchmarkMeasurement mm) {
  if (!mm.valid) {
    return;
  }
  if (mm.error) {
    fprintf(file, "%s,%d,%d,%d,%d,,,,,,,,", mm.path, mm.src_size, mm.dest_size, mm.src_height,
            mm.dest_height);
    for (int i = 0; i < 6 * REPEAT; i++) {
      fprintf(file, ",");
    }
    fprintf(file, "\n");
    return;
  }
  double total_av_ms = (double) find_min(mm.total_vals) / 1000000;
  double init1_av_ms = (double) find_min(mm.init1_vals) / 1000000;
  double init2_av_ms = (double) find_min(mm.init2_vals) / 1000000;
  double diff_av_ms = (double) find_min(mm.diff_vals) / 1000000;
  double parse1_av_ms = (double) find_min(mm.parse1_vals) / 1000000;
  double parse2_av_ms = (double) find_min(mm.parse2_vals) / 1000000;
  /*uint64_t total_av = 0;
  uint64_t init1_av = 0;
  uint64_t init2_av = 0;
  uint64_t diff_av = 0;
  uint64_t parse1_av = 0;
  uint64_t parse2_av = 0;
  for (int i = 0; i < REPEAT; i++) {
    total_av += mm.total_vals[i];
    init1_av += mm.init1_vals[i];
    init2_av += mm.init2_vals[i];
    diff_av += mm.diff_vals[i];
    parse1_av += mm.parse1_vals[i];
    parse2_av += mm.parse2_vals[i];
  }
  double total_av_ms = (double) total_av / REPEAT / 1000000;
  double init1_av_ms = (double) init1_av / REPEAT / 1000000;
  double init2_av_ms = (double) init2_av / REPEAT / 1000000;
  double diff_av_ms = (double) diff_av / REPEAT / 1000000;
  double parse1_av_ms = (double) parse1_av / REPEAT / 1000000;
  double parse2_av_ms = (double) parse2_av / REPEAT / 1000000;*/
  fprintf(file, "%s,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f", mm.path, mm.src_size, mm.dest_size, mm.src_height,
          mm.dest_height,
          mm.edit_script_size, mm.reuse_ratio, total_av_ms, init1_av_ms, init2_av_ms, diff_av_ms, parse1_av_ms,
          parse2_av_ms);
  for (int i = 0; i < REPEAT; i++) {
    fprintf(file, ",%llu", mm.total_vals[i]);
  }
  for (int i = 0; i < REPEAT; i++) {
    fprintf(file, ",%llu", mm.init1_vals[i]);
  }
  for (int i = 0; i < REPEAT; i++) {
    fprintf(file, ",%llu", mm.init2_vals[i]);
  }
  for (int i = 0; i < REPEAT; i++) {
    fprintf(file, ",%llu", mm.diff_vals[i]);
  }
  for (int i = 0; i < REPEAT; i++) {
    fprintf(file, ",%llu", mm.parse1_vals[i]);
  }
  for (int i = 0; i < REPEAT; i++) {
    fprintf(file, ",%llu", mm.parse2_vals[i]);
  }
  fprintf(file, "\n");
}

static inline bool search_errors(TSNode node) {
  bool errors = false;
  if (ts_node_has_error(node) || ts_node_is_missing(node)) {
    printf("[SearchErrors] Found ERROR or MISSING Node!\n");
    return true;
  }
  if (node.diff_heap != NULL && node.diff_heap->ref_count != 1) {
    Subtree *sub = (Subtree *) node.id;
    bool is_parent_init = ts_node_parent(node).diff_heap != NULL;
    printf("Found node (IsInline: %d) (%p) with ref %d (%p|%p) [%d %d %d] (ParentIsInit: %d)\n", sub->data.is_inline,
           node.id, node.diff_heap->ref_count, node.diff_heap, node.diff_heap->id, node.diff_heap->treesize,
           node.diff_heap->treeheight, node.diff_heap->position.bytes, is_parent_init);
    return true;
  }
  for (uint32_t i = 0; i < ts_node_child_count(node); i++) {
    TSNode child = ts_node_child(node, i);
    errors = search_errors(child) || errors;
  }
  return errors;
}

void log_to_stdout(void *payload, TSLogType type, const char *message) {
  printf("%s\n", message);
}

void ts_parser_log_to_stderr(TSParser *parser) {
  ts_parser_set_logger(parser, (TSLogger) {.log = log_to_stdout, .payload = NULL});
}

int calc_init_reuse(TSNode node) {
  TSTreeCursor cursor = ts_tree_cursor_new(node);
  int lvl = 0;
  int reuse = 0;
  TSNode curr;
  do {
    curr = ts_tree_cursor_current_node(&cursor);
    if (curr.diff_heap != NULL) {
      reuse++;
    }
    while (ts_diff_tree_cursor_goto_first_child(&cursor)) {
      lvl++;
      curr = ts_tree_cursor_current_node(&cursor);
      if (curr.diff_heap != NULL) {
        reuse++;
      }
    }
    while (!(ts_diff_tree_cursor_goto_next_sibling(&cursor)) && lvl > 0) {
      lvl--;
      ts_diff_tree_cursor_goto_parent(&cursor);
    }
  } while (lvl > 0);
  ts_tree_cursor_delete(&cursor);
  return reuse;
}

#endif //TREE_SITTER_TEST_BENCH_INC_H
