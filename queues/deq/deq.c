#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "deq.h"

#ifndef NULL
#define NULL 0
#endif

struct deq {
  int head;
  int tail;
  int size;
  int *content;
};

Deq DeqConstructor(int size) {
  assert(size);

  Deq deq = malloc(sizeof(*deq));
  assert(deq);

  deq->size = size + 1;
  assert(deq->size);
  
  deq->head = deq->size;
  assert(deq->head);
  deq->tail = 0;

  deq->content = malloc(sizeof(deq->content) * deq->size);
  assert(deq->content);

  return deq;
}

void DeqDestructor(Deq deq) {
  assert(deq && "Provide Destructor with Dequeue");

  free(deq->content);
  deq->content = NULL;

  free(deq);
  deq = NULL;
}

void DeqInsertBack(Deq deq, int i) {
  assert(deq && "Provide Dequeue where to enqueue item");
  assert(!DeqIsFull(deq) && "Dequeue Overflow");
  
  deq->content[deq->tail] = i;
  deq->tail = (deq->tail + 1) % deq->size;
}

int DeqDeleteFront(Deq deq) {
  assert(deq && "Provide Dequeue from where to dequeue item");
  assert(!DeqIsEmpty(deq) && "Dequeue Underflow");

  deq->head = deq->head % deq->size;
  
  return deq->content[deq->head++];
}

void DeqInsertFront(Deq deq, int i) {
  assert(deq && "Provide Dequeue from where to dequeue item");
  deq->head = (deq->head - 1) % deq->size;
  assert(deq->head != deq->tail);

  deq->content[deq->head] = i;
}

int DeqDeleteBack(Deq deq) {
  assert(deq && "Provide Dequeue from where to delete item from back");
  assert(deq->tail % deq->size != deq->head && "Dequeue underflow");
  deq->tail = (deq->tail - 1 + deq->size) % deq->size;

  return deq->content[deq->tail];
}

int DeqIsEmpty(Deq deq) {
  assert(deq && "Provide Dequeue to check if it's empty");
  return deq->head % deq->size == deq->tail;
}

int DeqIsFull(Deq deq) {
  assert(deq && "Provide Dequeue to check if it's full");
  return deq->tail + 1 == deq->head;
}