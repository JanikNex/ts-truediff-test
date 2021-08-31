#ifndef TREE_SITTER_BENCH_CSV_UTIL_H
#define TREE_SITTER_BENCH_CSV_UTIL_H

#include <stdio.h>

FILE *create_csv_file(char *filename) {
  return fopen(filename, "w+");
}

void write_header(FILE *fp, int runs) {
  fprintf(fp,
          "Filename,Src tree size,Dest tree size,Src tree height,Dest tree height,EditScript size,Average total Diffing time (ms),Average initialization time 1 (ms),Average initialization time 2 (ms),Average Diffing time (ms),Average Parsing Time 1 (ms),Average Parsing Time 2 (ms)");
  char *header;
  for (int i = 0; i < 6; i++) {
    if (i == 0) {
      header = "Total";
    } else if (i == 1) {
      header = "Init 1_";
    } else if (i == 2) {
      header = "Init 2_";
    } else if (i == 3) {
      header = "Diff";
    } else if (i == 4) {
      header = "Parse 1_";
    } else if (i == 5) {
      header = "Parse 2_";
    }
    for (int j = 0; j < runs; j++) {
      fprintf(fp, ", %s%d", header, j + 1);
    }
  }
  fprintf(fp, "\n");
}

void close_file(FILE *fp) {
  fclose(fp);
}

#endif //TREE_SITTER_BENCH_CSV_UTIL_H
