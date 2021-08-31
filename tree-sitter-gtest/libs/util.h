#ifndef TEST_SITTER_UTIL_H
#define TEST_SITTER_UTIL_H
#include <time.h>

void print_hash(const unsigned char *hash) {
  for (int i = 0; i < 32; ++i) {
    if (i > 0) {
      printf("-");
    }
    printf("%02x", hash[i]);
  }
  printf("\n");
}

void test_equality(TSDiffHeap heap1, TSDiffHeap heap2) {
  printf("Tree1 | Structural hash: ");
  print_hash(heap1.structural_hash);
  printf("Tree2 | Structural hash: ");
  print_hash(heap2.structural_hash);
  printf("Tree1 | Literal hash: ");
  print_hash(heap1.literal_hash);
  printf("Tree2 | Literal hash: ");
  print_hash(heap2.literal_hash);
  printf("Structurally equal: %s | Literally equal: %s\n",
         ts_diff_heap_hash_eq(heap1.structural_hash, heap2.structural_hash) ? "TRUE" : "FALSE",
         ts_diff_heap_hash_eq(heap1.literal_hash, heap2.literal_hash) ? "TRUE" : "FALSE");
}

void timing(void(*f)()) {
  printf("\n---- TIMING START ----\n");
  clock_t begin = clock();

  f();

  clock_t end = clock();
  double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
  printf("\n---- TIMING: %f ----\n", time_spent);
}


void traverseTree_(TSNode curr, int lvl) {
  for (int i = 0; i < lvl; ++i) {
    printf("\t");
  }
//  printf("%s [%u | %u](%d)(Share: %p)(ID: %p)(Subtree: %p)\n", ts_node_type(curr), ts_node_start_byte(curr),
//         ts_node_end_byte(curr),
//         curr.diff_heap->assigned != NULL, curr.diff_heap->share, curr.diff_heap->id, curr.id);
  printf("%s [%u | %u](%d)(ID: %p)\n", ts_node_type(curr), ts_node_start_byte(curr),
         ts_node_end_byte(curr),
         curr.diff_heap->assigned != NULL, curr.diff_heap->id);
  for (int i = 0; i < ts_node_child_count(curr); i++) {
    TSNode cld = ts_node_child(curr, i);
    if (!ts_node_is_null(cld)) {
      traverseTree_(cld, lvl + 1);
    }
  }
}

void traverseTree(TSNode curr) {
  traverseTree_(curr, 0);
}

void printNode(TSNode node, int offset) {
  for (int i = 0; i < offset; ++i) {
    printf("\t");
  }
  const TSDiffHeap *diff_heap = node.diff_heap;
  if (diff_heap == NULL) {
    printf("%s [%u | %u] UNINITIALIZED\n", ts_node_type(node), ts_node_start_byte(node),
           ts_node_end_byte(node));
  } else {
    printf("%s [%u | %u](%d)(ID: %p)\n", ts_node_type(node), ts_node_start_byte(node),
           ts_node_end_byte(node), diff_heap->assigned != NULL, diff_heap->id);
  }
}

void traverseTree2(TSNode node) {
  TSTreeCursor cursor = ts_tree_cursor_new(node);
  int lvl = 0;
  TSNode curr;
  do {
    curr = ts_tree_cursor_current_node(&cursor);
    printNode(curr, lvl);
    while (ts_diff_tree_cursor_goto_first_child(&cursor)) {
      lvl++;
      curr = ts_tree_cursor_current_node(&cursor);
      printNode(curr, lvl);
    }
    while (!(ts_diff_tree_cursor_goto_next_sibling(&cursor)) && lvl > 0) {
      lvl--;
      ts_diff_tree_cursor_goto_parent(&cursor);
    }
  } while (lvl > 0);
  ts_tree_cursor_delete(&cursor);
}
#endif //TEST_SITTER_UTIL_H
