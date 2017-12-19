#include <assert.h>
#include <stdlib.h>
#include "bag.h"

#ifndef NULL
#define NULL 0
#endif

struct node {
  struct node *next;
  int val;
};

struct bag {
  int size;
  struct node *head;
};

Bag BagConstructor(void) {
  Bag b;
  
  b = malloc(sizeof(*b));
  assert(b);
  
  b->size = 0;
  b->head = malloc(sizeof(struct node *));
  b->head = NULL;

  return b;
}

void BagDestructor(Bag b) {
  assert(b);
  struct node *cursor;
  struct node *next;

  cursor = b->head;

  while (cursor) {
    next = cursor->next;
    free(cursor);
    cursor = next;
  }

  free(b->head);
  b->head = NULL;
  free(b);
  b = NULL;
}

void BagAdd(Bag b, int d) {
  assert(b);
  assert(d);
  struct node *newnode;

  newnode = malloc(sizeof(*newnode));

  assert(newnode);

  newnode->val = d;
  newnode->next = b->head;

  b->head = newnode;

  int oldsize = b->size;
  b->size++;
  assert((oldsize + 1) == b->size);
}

void BagTraverse(Bag b, void (*f)(int)) {
  assert(b);
  struct node *cursor = b->head;

  while (cursor) {
    f(cursor->val);
    
    cursor = cursor->next;
  }
}

int BagIsEmpty(Bag b) {
  assert(b);

  return !((long int) b->head);
}

int BagSize(Bag b) {
  if (BagIsEmpty(b)) {
    return 0;
  }

  return b->size;
}