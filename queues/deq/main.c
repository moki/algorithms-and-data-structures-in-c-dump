#include <assert.h>
#include <stdio.h>
#include "deq.h"

#define N (10)

int main(int argc, char **argv) {
  Deq deq = DeqConstructor(12);

  DeqInsertBack(deq, 1);
  DeqInsertBack(deq, 1);
  DeqInsertBack(deq, 1);
  DeqInsertBack(deq, 1);
  DeqInsertBack(deq, 1);
  DeqInsertBack(deq, 1);
  assert(!DeqIsEmpty(deq));
  assert(DeqDeleteFront(deq) == 1);
  assert(DeqDeleteFront(deq) == 1);
  assert(DeqDeleteFront(deq) == 1);
  assert(DeqDeleteFront(deq) == 1);
  assert(DeqDeleteFront(deq) == 1);
  assert(DeqDeleteFront(deq) == 1);
  assert(DeqIsEmpty(deq));
  DeqInsertBack(deq, 15);
  DeqInsertBack(deq, 6);
  DeqInsertBack(deq, 9);
  DeqInsertBack(deq, 8);
  DeqInsertBack(deq, 4);
  DeqInsertBack(deq, 17);
  DeqInsertBack(deq, 3);
  DeqInsertBack(deq, 5);
  DeqInsertBack(deq, 4);
  DeqInsertBack(deq, 3);
  DeqInsertBack(deq, 2);
  DeqInsertBack(deq, 1);
  assert(!DeqIsEmpty(deq));
  assert(DeqDeleteFront(deq) == 15);
  assert(DeqDeleteFront(deq) == 6);
  assert(DeqDeleteFront(deq) == 9);
  assert(DeqDeleteFront(deq) == 8);
  assert(DeqDeleteFront(deq) == 4);
  assert(DeqDeleteFront(deq) == 17);
  assert(DeqDeleteFront(deq) == 3);
  assert(DeqDeleteFront(deq) == 5);
  assert(DeqDeleteFront(deq) == 4);
  assert(DeqDeleteFront(deq) == 3);
  assert(DeqDeleteFront(deq) == 2);
  assert(DeqDeleteFront(deq) == 1);

  // Test deletion from back of the deq
  DeqInsertBack(deq, 3);
  DeqInsertBack(deq, 2);
  DeqInsertBack(deq, 1);
  assert(DeqDeleteBack(deq) == 1);
  assert(DeqDeleteBack(deq) == 2);
  assert(DeqDeleteBack(deq) == 3);

  // Test insertion to the front
  DeqInsertFront(deq, 3);
  DeqInsertFront(deq, 2);
  DeqInsertFront(deq, 1);
  assert(DeqDeleteBack(deq) == 3);
  assert(DeqDeleteFront(deq) == 1);
  assert(DeqDeleteFront(deq) == 2);
  
  assert(DeqIsEmpty(deq));
  
  DeqDestructor(deq);
}