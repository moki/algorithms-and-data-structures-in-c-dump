#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sort.h"
#include "sort-utils.h"

#define SIZE (10)

int main(int argc, char **argv) {
  int *a = generateRandomArray(SIZE, 20);
  displayArray(a, SIZE);

  assert(!isSorted(a, SIZE));

  SelectionSort(a, SIZE);
  
  assert(isSorted(a, SIZE));
}