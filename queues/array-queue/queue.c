#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

#ifndef NULL
#define NULL 0
#endif

struct queue {
  int head;
  int tail;
  int size;
  int *content;
};

Queue QueueConstructor(int size) {
  assert(size);

  Queue q = malloc(sizeof(*q));
  assert(q);

  q->size = size + 1;
  assert(q->size);
  
  q->head = q->size;
  assert(q->head);
  q->tail = 0;

  q->content = malloc(sizeof(q->content) * q->size);
  assert(q->content);

  return q;
}

void QueueDestructor(Queue q) {
  assert(q && "Provide Destructor with Queue");

  free(q->content);
  q->content = NULL;

  free(q);
  q = NULL;
}

void Enqueue(Queue q, int i) {
  assert(q && "Provide Queue where to enqueue item");
  assert(!QueueIsFull(q) && "Queue Overflow");
  
  q->content[q->tail] = i;
  q->tail = (q->tail + 1) % q->size;
}

int Dequeue(Queue q) {
  assert(q && "Provide Queue from where to dequeue item");
  assert(!QueueIsEmpty(q) && "Queue Underflow");

  q->head = q->head % q->size;
  
  return q->content[q->head++];  
}

int QueueIsEmpty(Queue q) {
  assert(q && "Provide Queue to check if it's empty");
  return q->head % q->size == q->tail;
}

int QueueIsFull(Queue q) {
  assert(q && "Provide Queue to check if it's full");
  return q->tail + 1 == q->head;
}