#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sort.h"
#include "sort-utils.h"

#define SIZE (10)

int main(int argc, char **argv) {
  int *a = generateRandomArray(SIZE, 20);

  assert(!isSorted(a, SIZE));
  
  mergeSort(a, 0, SIZE);
  
  assert(isSorted(a, SIZE));

  free(a);
}