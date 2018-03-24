#include <assert.h>
#include "list.h"


int main(int argc, char **argv) {
  List l = ListConstructor();
  assert(l);

  ListInsert(l, 99);

  ListInsert(l, 13);

  ListInsert(l, 666);

  ListInsert(l, 777);

  assert(ListSearch(l, 13) == 3);

  ListDelete(l, 13);

  assert(ListSearch(l, 13) == -1);

  ListDelete(l, 99);

  assert(ListSearch(l, 99) == -1);

  ListInsert(l, 99);

  assert(ListSearch(l, 99) == 6);

  ListInsert(l, 13);

  assert(ListSearch(l, 13) == 9);

  ListDestructor(l);
  assert(l);
}