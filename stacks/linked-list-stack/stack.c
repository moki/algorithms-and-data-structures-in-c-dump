#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

struct stack {
  Node head;
};

Node createNode(int data, Node next) {
  Node n = malloc(sizeof(*n));
  assert(n && "Failed to allocate space for a new node");

  n->data = (int) data;
  assert(n->data == data && "Failed to assign provided data to the new node");

  n->next = next;
  assert(n->next == next && "Failed to assign provided next pointer to the new node");

  return n;
}

void _free(Node node) {
  free((void *) node);
}

Stack StackConstructor() {
  Stack s = malloc(sizeof(*s));
  assert(s && "Failed allocate space for the stack");

  Node n = createNode(NULL, NULL);
  n->next = n;

  s->head = n;
  assert(s->head && "Failed to set sentinel node as head");

  return s;
}

void StackDestructor(Stack s) {
  StackTraverse(s, _free);
  free(s->head);
  free(s);
}

void StackPush(Stack s, int i) {
  assert(s && "Provide stack where to insert item");

  Node n = createNode(i, s->head->next);
  assert(n && "Failed to create new stack node");

  s->head->next = n;
  assert(s->head->next == n && "Failed to link head to the new node as the next element");
}

Node StackPop(Stack s) {
  assert(s->head->next != s->head && "Stack is empty, underflow");

  Node n = s->head->next;
  free(s->head->next);

  s->head->next = n->next;

  return n;
}

void StackTraverse(Stack s, void (*f)(Node)) {
  assert(s && "Provide stack you want to traverse");
  assert(s->head && "Stack should have head element");
  assert(f && "Provide function you want to pass each element to");
  Node cursor = s->head;

  while ((cursor = cursor->next) != s->head) {
    f(cursor);
  }
}
