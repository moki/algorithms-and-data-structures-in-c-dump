#include <assert.h>
#include <stdio.h>
#include "stack.h"

#define N (10)

void display(int v) {
  printf("%d\n", v);
}

int main(int argc, char **argv) {
  int i;
  Stack stack = StackConstructor(N);

  assert(StackIsEmpty(stack));

  for (i = 0; i < N; ++i) {
    StackPush(stack, i);
  }

  assert(!StackIsEmpty(stack));

  StackTraverse(stack, display);

  StackDestructor(stack);
  assert(StackIsEmpty(stack));

  StackTraverse(stack, display);
}