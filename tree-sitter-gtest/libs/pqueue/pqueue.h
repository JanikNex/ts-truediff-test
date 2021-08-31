#ifndef DIFF_SITTER_PRIORITYQUEUE_H
#define DIFF_SITTER_PRIORITYQUEUE_H

#include "../debug.h"


typedef struct {
  Subtree **queue;
  int size;
  int used;
} PriorityQueue;

PriorityQueue *priority_queue_create();

void priority_queue_insert(PriorityQueue *, Subtree *);

Subtree *priority_queue_pop(PriorityQueue *);

void priority_queue_destroy(PriorityQueue *);

static inline bool priority_queue_is_empty(const PriorityQueue *queue) {
  return queue->used == 0;
}

static inline unsigned int priority_queue_head_value(const PriorityQueue *queue) {
  if (queue->used == 0){
    return UINT32_MAX;
  }
  Subtree *head_subtree = queue->queue[1];
  return ts_subtree_node_diff_heap(*head_subtree)->treeheight;
}

#endif //DIFF_SITTER_PRIORITYQUEUE_H
