#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "heap.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

#define INITIAL_CAPACITY (3)

struct heap {
  int capacity;
  int used;
  int *xs;
};

/*
 *  Heap Constructor
*/
Heap HeapConstructor() {
  Heap h = malloc(sizeof(*h));
  assert(h && "Failed to allocate space for a new heap");

  h->used = 0;
  h->capacity = INITIAL_CAPACITY;
  h->xs = calloc(sizeof(int), h->capacity);

  return h;
}

/*
 *  Heap Constructor, from unsorted array
*/
Heap HeapConstructorArray(int *a, size_t size) {
  assert(*a);
  assert(size);

  Heap h = malloc(sizeof(*h));
  
  assert(h && "Failed to allocate space for a new heap");

  h->used = h->capacity = size;
  h->xs = calloc(sizeof(int), h->capacity);

  // HeapRawDisplay(h);
  
  int i = -1;

  while (i++ < h->capacity) {
    h->xs[i] = a[i];
  }

  // HeapRawDisplay(h);

  i = (h->used - 1) / 2;

  while (i >= 0) {
    heapify(h, i);
    // HeapRawDisplay(h);
    i--;
  }

  return h;
}

/*
 * Recursive heapify
*/ 
// void heapify(Heap h, int i) {
//   int l = 2 * i + 1;
//   int r = 2 * i + 2;
//   int largest;

//   largest =
//     l < h->used && h->xs[l] > h->xs[i]
//       ? l
//       : i;
  
//   largest =
//     r < h->used && h->xs[r] > h->xs[largest]
//       ? r
//       : largest;
  
//   if (largest != i) {
//     int t = h->xs[i];
//     h->xs[i] = h->xs[largest];
//     h->xs[largest] = t;
//     heapify(h, largest);
//   }
// }

/*
 * Iterative heapify
*/ 
void heapify(Heap h, int i) {
  while (1) {
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int largest;

    largest =
      l < h->used && h->xs[l] > h->xs[i]
        ? l
        : i;
    
    largest =
      r < h->used && h->xs[r] > h->xs[largest]
        ? r
        : largest;
    
    if (largest == i) {
      break;
    }

    int t = h->xs[i];
    h->xs[i] = h->xs[largest];
    h->xs[largest] = t;
    i = largest;
  }
}

Heap HeapSort(int *a, size_t size) {
  Heap h = HeapConstructorArray(a, size);
  int i = h->used - 1;

  while (i > 0) {
    int t = h->xs[0];
    h->xs[0] = h->xs[i];
    h->xs[i] = t;
    h->used--;
    heapify(h, 0);
    i--;
  }

  return h;
}

/*
 *  Heap Insertion method
*/
void HeapInsert(Heap h, int i) {
  assert(h);
  assert(i);

  // Scale underlying array up, if neccessary
  if (h->used >= h->capacity) {
    h->capacity *= 2;
    h->xs = realloc(h->xs, sizeof(int) * h->capacity);
  }

  // Temporarly place new value in the end of the heap as the leaf
  h->xs[h->used] = i;

  // Swim/Bubble/Percolate Up newly added value if needed
  int k = h->used;

  while (k > 0 && h->xs[(k - 1) / 2] < h->xs[k]) {
    int t = h->xs[(k - 1) / 2];

    h->xs[(k - 1) / 2] = h->xs[k];    
    h->xs[k] = t;

    k = (k - 1) / 2;
  }

  h->used++;
}

int HeapPeekMax(Heap h) {
  assert(h);

  return h->xs[0];
}

int HeapExtractMax(Heap h) {
  assert(h);
  assert(h->used);
  h->used--;

  // Swap min and max
  int m = h->xs[0];
  h->xs[0] = h->xs[h->used];
  h->xs[h->used] = 0;

  // Sink min value in the heap from top to deserved closest leaf
  heapify(h, 0);

  // Scale underlying array down, if neccessary
  if (h->capacity - h->used <= 3) {
    h->capacity /= 2;
    h->xs = realloc(h->xs, sizeof(int) * h->capacity);
  }

  return m;
}

int HeapIsEmpty(Heap h) {
  return !h->used;
}

int HeapSize(Heap h) {
  return h->used;
}

void HeapDestructor(Heap h) {
  free(h->xs);
  free(h);
}

void HeapRawDisplay(Heap h) {
  assert(h);

  int c = 0;

  while (c < h->capacity) {
    printf("%d\n", h->xs[c]);
    c++;
  }

  printf("\n");
}