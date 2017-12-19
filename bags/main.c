#include <assert.h>
#include <stdio.h>
#include "bag.h"

#define N (50)

void display(int v) {
  printf("%d\n", v);
}

int main(int argc, char **argv) {
  Bag bag = BagConstructor();
  
  int i;

  assert(BagSize(bag) == 0);
  assert(BagIsEmpty(bag));

  for (i = 1; i <= N; i++) {
    BagAdd(bag, i);
  }

  assert(BagSize(bag) == 50);
  assert(!BagIsEmpty(bag));

  BagTraverse(bag, display);

  BagDestructor(bag);

  assert(BagIsEmpty(bag));
  assert(BagSize(bag) == 0);

  BagTraverse(bag, display);
}