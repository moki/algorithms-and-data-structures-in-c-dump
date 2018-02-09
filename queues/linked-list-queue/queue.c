#include <assert.h>
#include <stdlib.h>
#include "queue.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

struct queue {
  Sentinel sentinel;
};

struct sentinel {
  Node next;
  Node prev;
};

Node _createNode(int data, Node next) {
  Node n = malloc(sizeof(*n));
  assert(n && "Failed to allocate space for a new node");

  n->data = (int) data;
  assert(n->data == data && "Failed to assign provided data to the new node");

  n->next = next;
  assert(n->next == next && "Failed to assign provided next pointer to the new node");

  return n;
}

Queue QueueConstructor() {
  Queue q = malloc(sizeof(*q));
  assert(q && "Failed to allocate space for the Queue");

  Sentinel s = malloc(sizeof(*s));
  assert(s);
  
  s->next = s->prev = s;
  assert(s->next == s);
  assert(s->prev == s);

  q->sentinel = s;

  return q;
}

void QueueDestructor(Queue q) {
  if (q->sentinel == q->sentinel->next) {
    free(q->sentinel);
    free(q);
    return;
  }

  Node cursor = q->sentinel->next;

  while (cursor != q->sentinel) {
    cursor = cursor->next;
  }

  free(q->sentinel);
  free(q);
}

void QueueEnqueue(Queue q, int i) {
  assert(q && "Provide Queue where you want to insert item");

  Node n = _createNode(i, q->sentinel);
  assert(n && "Failed to create new queue node");

  q->sentinel->prev->next = n;
  assert(q->sentinel->prev->next == n);

  q->sentinel->prev = n;
  assert(q->sentinel->prev == n && "Failed to link sentinel to the new node as the next element");
}

Node QueueDequeue(Queue q) {
  assert(q && "Provide the queue you want to dequeue element from");
  assert(q->sentinel != (Sentinel) q->sentinel->next && "Queue is empty, underflow");
  
  Node n = malloc(sizeof(*n));
  *n = *q->sentinel->next;
  assert(n);

  free(q->sentinel->next);

  q->sentinel->next = n->next;
  assert(q->sentinel->next == n->next);
  
  return n;
}