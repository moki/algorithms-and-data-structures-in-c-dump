#include <assert.h>
#include <stdio.h>
#include "queue.h"

#define N (10)

int main(int argc, char **argv) {
  Queue q = QueueConstructor(12);

  Enqueue(q, 1);
  Enqueue(q, 1);
  Enqueue(q, 1);
  Enqueue(q, 1);
  Enqueue(q, 1);
  Enqueue(q, 1);
  assert(!QueueIsEmpty(q));
  assert(Dequeue(q) == 1);
  assert(Dequeue(q) == 1);
  assert(Dequeue(q) == 1);
  assert(Dequeue(q) == 1);
  assert(Dequeue(q) == 1);
  assert(Dequeue(q) == 1);
  assert(QueueIsEmpty(q));
  Enqueue(q, 15);
  Enqueue(q, 6);
  Enqueue(q, 9);
  Enqueue(q, 8);
  Enqueue(q, 4);
  Enqueue(q, 17);
  Enqueue(q, 3);
  Enqueue(q, 5);
  Enqueue(q, 4);
  Enqueue(q, 3);
  Enqueue(q, 2);
  Enqueue(q, 1);
  assert(!QueueIsEmpty(q));
  assert(Dequeue(q) == 15);
  assert(Dequeue(q) == 6);
  assert(Dequeue(q) == 9);
  assert(Dequeue(q) == 8);
  assert(Dequeue(q) == 4);
  assert(Dequeue(q) == 17);
  assert(Dequeue(q) == 3);
  assert(Dequeue(q) == 5);
  assert(Dequeue(q) == 4);
  assert(Dequeue(q) == 3);
  assert(Dequeue(q) == 2);
  assert(Dequeue(q) == 1);
  assert(QueueIsEmpty(q));
  
  QueueDestructor(q);
}