#include <assert.h>
#include "heap.h"
#include <stdlib.h>

int main(int argc, char **argv) {
  Heap h = HeapConstructor();

  HeapInsert(h, 1);
  HeapInsert(h, 2);
  HeapInsert(h, 3);
  HeapInsert(h, 4);

  HeapRawDisplay(h);
  HeapIncrease(h, 2, 5);
  HeapRawDisplay(h);

  assert(HeapPeekMax(h) == 5);
  assert(HeapSize(h) == 4);

  assert(HeapExtractMax(h) == 5);
  
  assert(HeapSize(h) == 3);

  HeapDestructor(h);

  int a[3] = {1, 2, 3};
  int c[6] = {1, 2, 3, 4, 5, 6};

  Heap h1 = HeapConstructorArray(a, (size_t) sizeof(a) / sizeof(a[0]));
  HeapDestructor(h1);

  Heap sorted = HeapSort(c, (size_t) sizeof(c) / sizeof(c[0]));
  HeapDestructor(sorted);
}