//
// Created by Janik on 01.05.2021.
//

#ifndef TEST_SITTER_DEBUG_H
#define TEST_SITTER_DEBUG_H
typedef struct {
  union {
    char *long_data;
    char short_data[24];
  };
  uint32_t length;
} ExternalScannerState;

typedef struct {
  uint32_t bytes;
  TSPoint extent;
} Length;

typedef struct {
  bool is_inline: 1;
  bool visible: 1;
  bool named: 1;
  bool extra: 1;
  bool has_changes: 1;
  bool is_missing: 1;
  bool is_keyword: 1;
  uint8_t symbol;
  uint8_t padding_bytes;
  uint8_t size_bytes;
  uint8_t padding_columns;
  uint8_t padding_rows: 4;
  uint8_t lookahead_bytes: 4;
  uint16_t parse_state;
  TSDiffHeap *diff_heap;
} SubtreeInlineData;

typedef struct {
  volatile uint32_t ref_count;
  Length padding;
  Length size;
  uint32_t lookahead_bytes;
  uint32_t error_cost;
  uint32_t child_count;
  TSSymbol symbol;
  TSStateId parse_state;
  TSDiffHeap *diff_heap;

  bool visible: 1;
  bool named: 1;
  bool extra: 1;
  bool fragile_left: 1;
  bool fragile_right: 1;
  bool has_changes: 1;
  bool has_external_tokens: 1;
  bool depends_on_column: 1;
  bool is_missing: 1;
  bool is_keyword: 1;

  union {
    // Non-terminal subtrees (`child_count > 0`)
    struct {
      uint32_t visible_child_count;
      uint32_t named_child_count;
      uint32_t node_count;
      uint32_t repeat_depth;
      int32_t dynamic_precedence;
      uint16_t production_id;
      struct {
        TSSymbol symbol;
        TSStateId parse_state;
      } first_leaf;
    };

    // External terminal subtrees (`child_count == 0 && has_external_tokens`)
    ExternalScannerState external_scanner_state;

    // Error terminal subtrees (`child_count == 0 && symbol == ts_builtin_sym_error`)
    int32_t lookahead_char;
  };
} SubtreeHeapData;

typedef union {
  SubtreeInlineData data;
  const SubtreeHeapData *ptr;
} Subtree;

struct TSTree {
  Subtree root;
  const TSLanguage *language;
  int *parent_cache;
  uint32_t parent_cache_start;
  uint32_t parent_cache_size;
  TSRange *included_ranges;
  unsigned included_range_count;
};


struct TSDiffHeap {
  void *id;
  volatile uint32_t ref_count;
  const unsigned char structural_hash[32];
  unsigned char literal_hash[32];
  unsigned int treeheight;
  unsigned int treesize;
  void *share;
  TSDiffHeap *preemptive_assignment;
  Subtree *assigned;
  Length position;
  Length padding;
  Length size;
};

struct TSLiteralMap {
  uint32_t symbol_count;
  uint8_t *symbol_map;
};

typedef struct {
  const Subtree *subtree;
  Length position;
  uint32_t child_index;
  uint32_t structural_child_index;
} TreeCursorEntry;
#endif //TEST_SITTER_DEBUG_H
