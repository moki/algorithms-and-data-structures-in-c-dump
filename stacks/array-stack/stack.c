#include <assert.h>
#include <stdlib.h>
#include "stack.h"

#ifndef NULL
#define NULL 0
#endif

struct stack {
  int size;
  int top;
  int *content;
};

Stack StackConstructor(int size) {
  assert(size);

  Stack s = malloc(sizeof(*s));
  assert(s);

  s->size = size;
  assert(s->size == size);

  s->content = malloc(sizeof(s->content) * s->size);
  assert(s->content);

  s->top = 0;
  assert(s->top == 0);

  return s;
}


void StackDestructor(Stack s) {
  assert(s);

  free(s->content);
  s->content = NULL;
  free(s);
  s = NULL;
}

int StackPop(Stack s) {
  assert(s);
  // check for underflow
  assert(s->top);

  int oldtop = s->top;
  int item = s->content[--s->top];
  
  assert(s->top + 1 == oldtop);
  
  return item;
}

void StackPush(Stack s, int i) {
  assert(s);
  // check for overflow
  assert(s->top < s->size);

  int oldtop = s->top;

  s->content[s->top++] = i;
  
  assert(s->content[oldtop] == i);
  assert(s->top == oldtop + 1);
}

void StackTraverse(Stack s, void (*f)(int)) {
  assert(s);

  while (s->top) {
    f(StackPop(s));
  }
}

int StackIsEmpty(Stack s) {
  return s->top == 0;
}