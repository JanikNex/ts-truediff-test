#ifndef TREE_SITTER_TEST_BENCH_H
#define TREE_SITTER_TEST_BENCH_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libs/tree-sitter/includes/api.h"

#define REPEAT 3

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
  fprintf(file, "%s,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f", mm.path, mm.src_size, mm.dest_size, mm.src_height,
          mm.dest_height,
          mm.edit_script_size, total_av_ms, init1_av_ms, init2_av_ms, diff_av_ms, parse1_av_ms, parse2_av_ms);
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

#endif //TREE_SITTER_TEST_BENCH_H
