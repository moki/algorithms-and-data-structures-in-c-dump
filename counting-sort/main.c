#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sort.h"
#include "sort-utils.h"

#define SIZE (8)
#define MAXVAL (5)

int main(int argc, char **argv) {
  int *a = generateRandomArray(SIZE, MAXVAL);

  assert(!isSorted(a, SIZE));

  int *b = countingSort(a, SIZE, MAXVAL);
  
  free(a);
  
  assert(isSorted(b, SIZE));

  free(b);
}