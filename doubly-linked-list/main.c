#include <assert.h>
#include <stdio.h>
#include "list.h"

void display(Node node) {
  printf("%d\n", node->data);
}

int main(int argc, char **argv) {
  List l = ListConstructor();
  assert(l);

  ListInsert(l, 4);
  ListInsert(l, 3);
  ListInsert(l, 2);
  ListInsert(l, 1);

  assert(ListSearch(l, 1)->data == 1);
  assert(ListSearch(l, 2)->data == 2);
  assert(ListSearch(l, 3)->data == 3);
  assert(ListSearch(l, 4)->data == 4);

  printf("Traverse in normal order\n");
  ListTraverse(l, display, TRAVERSE_NORMAL);
  printf("\nTraverse in reverse order\n");
  ListTraverse(l, display, TRAVERSE_REVERSE);

  ListDelete(l, 3);
  ListDelete(l, 2);
  ListDelete(l, 4);
  ListDelete(l, 1);

  assert(!ListSearch(l, 1));
  assert(!ListSearch(l, 2));
  assert(!ListSearch(l, 3));
  assert(!ListSearch(l, 4));

  ListDestructor(l);
}