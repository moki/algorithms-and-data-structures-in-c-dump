#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

#define INITIAL_CAPACITY (2)

struct list {
  int head;
  int used;
  int capacity;
  int *data;
};

List ListConstructor() {
  List l = malloc(sizeof(*l));
  assert(l && "Failed to allocate space for a new list");

  l->used = 0;
  l->capacity = INITIAL_CAPACITY;
  l->data = calloc(sizeof(int), 3 * l->capacity);
  l->head = -3;

  return l;
}


void ListInsert(List l, int i) {
  if (l->used >= l->capacity) {
    l->capacity *= 2;
    l->data = realloc(l->data, sizeof(int) * 3 * l->capacity);
  }

  l->data[l->head + 3] = i;
  l->data[l->head + 3 + 1] = l->head;
  l->data[l->head + 3 + 2] = 0;

  l->data[0 + 1] = l->data[l->head + 2] = l->head + 3;

  l->head += 3;
  l->used++;
}


int ListSearch(List l, int i) {
  int c = 0;
  assert(l);
  assert(i);

  int r = -1;

  while (c < l->capacity * 3) {
    if (l->data[c] == i) {
      r = c;
    }

    c += 3;
  }

  return r;
}

void ListDelete(List l, int i) {
  assert(l);
  assert(i);
  assert(l->used > 0);

  int t = ListSearch(l, i);

  int cursor = t + 3;

  while (cursor <= l->head) {
    l->data[cursor - 3] = l->data[cursor];
    l->data[cursor - 3 + 1] = l->data[cursor + 1] - 3;
    if (cursor == l->head) {
      l->data[cursor - 3 + 2] = l->data[cursor + 2];
    } else {
      l->data[cursor - 3 + 2] = l->data[cursor + 2] - 3;
    }

    cursor += 3;
  }

  l->used--;

  if (l->used > 0) {
    l->data[1] -= 3;
  }

  if (t == 0) {
    l->data[1] = l->head - 3;
  }

  l->head -= 3;

  if (l->capacity - l->used >= 2) {
    l->capacity /= 2;
    l->data = realloc(l->data, sizeof(int) * 3 * l->capacity);
  }
}

void ListDestructor(List l) {
  free(l->data);

  free(l);
}