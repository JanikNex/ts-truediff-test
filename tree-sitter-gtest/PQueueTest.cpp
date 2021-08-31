#include <cstdlib>
#include "libs/tree-sitter/includes/api.h"
#include "libs/tree-sitter/includes/parser.h"
#include "gtest/gtest.h"

extern "C" {
#include "libs/debug.h"
#include "libs/pqueue/pqueue.h"
}

Subtree *generate_fake_subtree(int tree_height) {
  Subtree *addr = (Subtree *) malloc(sizeof(Subtree));
  TSDiffHeap *diff_heap = (TSDiffHeap *) malloc(sizeof(TSDiffHeap));
  SubtreeInlineData *inline_data = &addr->data;
  inline_data->is_inline = true;
  inline_data->diff_heap = diff_heap;
  diff_heap->treeheight = tree_height;
  return addr;
}


void remove_fake_subtree(Subtree *subtree){
  TSDiffHeap *diff_heap = ts_subtree_node_diff_heap(*subtree);
  free(diff_heap);
  free(subtree);
}

TEST(FakeInitTests, Test1){
  Subtree *sub = generate_fake_subtree(5);
  ASSERT_TRUE(sub != nullptr);
  TSDiffHeap *diff_heap = ts_subtree_node_diff_heap(*sub);
  ASSERT_TRUE(diff_heap != nullptr);
  ASSERT_EQ(diff_heap->treeheight,5);
  remove_fake_subtree(sub);
}

TEST(PQueueTests, Test1){
  PriorityQueue *pqueue = priority_queue_create();
  ASSERT_TRUE(pqueue != nullptr);
  Subtree *sub = generate_fake_subtree(1);
  priority_queue_insert(pqueue, sub);

  Subtree *popped = priority_queue_pop(pqueue);
  TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
  ASSERT_EQ(popped_diff_heap->treeheight, 1);
  remove_fake_subtree(popped);

  priority_queue_destroy(pqueue);
}

TEST(PQueueTests, Test2){
  PriorityQueue *pqueue = priority_queue_create();
  ASSERT_TRUE(pqueue != nullptr);
  {
    Subtree *sub = generate_fake_subtree(1);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(2);
    priority_queue_insert(pqueue, sub);
  }

  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 2);
    remove_fake_subtree(popped);
  }

  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 1);
    remove_fake_subtree(popped);
  }

  priority_queue_destroy(pqueue);
}

TEST(PQueueTests, Test3){
  PriorityQueue *pqueue = priority_queue_create();
  ASSERT_TRUE(pqueue != nullptr);
  {
    Subtree *sub = generate_fake_subtree(1);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(2);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(3);
    priority_queue_insert(pqueue, sub);
  }

  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 3);
    remove_fake_subtree(popped);
  }

  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 2);
    remove_fake_subtree(popped);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 1);
    remove_fake_subtree(popped);
  }

  priority_queue_destroy(pqueue);
}

TEST(PQueueTests, Test5){
  PriorityQueue *pqueue = priority_queue_create();
  ASSERT_TRUE(pqueue != nullptr);
  for (int i = 0; i < 5; i++) {
    Subtree *sub = generate_fake_subtree(i);
    priority_queue_insert(pqueue, sub);
  }
  int expected_next = 4;
  while (!priority_queue_is_empty(pqueue)){
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, expected_next);
    remove_fake_subtree(popped);
    expected_next--;
  }
  priority_queue_destroy(pqueue);
}

TEST(PQueueTests, Test6){
  PriorityQueue *pqueue = priority_queue_create();
  ASSERT_TRUE(pqueue != nullptr);
  {
    Subtree *sub = generate_fake_subtree(5);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 5);
    remove_fake_subtree(popped);
  }
  {
    Subtree *sub = generate_fake_subtree(4);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(1);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 4);
    remove_fake_subtree(popped);
  }
  {
    Subtree *sub = generate_fake_subtree(1);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(1);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(3);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(1);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(3);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(1);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 3);
    remove_fake_subtree(popped);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 3);
    remove_fake_subtree(popped);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 1);
    remove_fake_subtree(popped);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 1);
    remove_fake_subtree(popped);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 1);
    remove_fake_subtree(popped);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 1);
    remove_fake_subtree(popped);
  }
  {
    Subtree *sub = generate_fake_subtree(1);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(1);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(2);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(1);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(2);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *sub = generate_fake_subtree(1);
    priority_queue_insert(pqueue, sub);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 2);
    remove_fake_subtree(popped);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 2);
    remove_fake_subtree(popped);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 1);
    remove_fake_subtree(popped);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 1);
    remove_fake_subtree(popped);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 1);
    remove_fake_subtree(popped);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 1);
    remove_fake_subtree(popped);
  }
  {
    Subtree *popped = priority_queue_pop(pqueue);
    TSDiffHeap *popped_diff_heap = ts_subtree_node_diff_heap(*popped);
    ASSERT_EQ(popped_diff_heap->treeheight, 1);
    remove_fake_subtree(popped);
  }
  priority_queue_destroy(pqueue);
}




