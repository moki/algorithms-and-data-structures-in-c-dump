#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sort.h"
#include "sort-utils.h"
#include "binary-utils.h"
#include <stdint.h>
#include <limits.h>
#include <time.h>

#define SIZE (130)
#define MAXVAL (300)

int main(int argc, char **argv) {
  // init random seed
  srand(time(NULL));

  // test out Sedgewick radix quicksort implementation
  int *a = generateRandomArray(SIZE, MAXVAL);

  assert(!isSorted(a, SIZE));
  displayArray(a, SIZE);

  radixqsort(a, 0, SIZE - 1);
  displayArray(a, SIZE);

  assert(isSorted(a, SIZE));
  
  free(a);

  // test out pointers radix quicksort implementation
  a = generateRandomArray(SIZE, MAXVAL);

  assert(!isSorted(a, SIZE));
  displayArray(a, SIZE);

  radixqsrt(a, SIZE);

  displayArray(a, SIZE);

  assert(isSorted(a, SIZE));
  
  free(a);
}