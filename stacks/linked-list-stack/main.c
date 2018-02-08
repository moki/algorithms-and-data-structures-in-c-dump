#include <assert.h>
#include <stdio.h>
#include "stack.h"

void display(Node node) {
  printf("%d\n", node->data);
}

int main(int argc, char **argv) {
  Stack s = StackConstructor();
  assert(s);

  StackPush(s, 4);
  StackPush(s, 3);
  StackPush(s, 2);
  StackPush(s, 1);

  StackPop(s);
  StackPop(s);
  StackPop(s);
  StackPop(s);

  StackDestructor(s);
}