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
  
  recursive_quicksort(a, 0, SIZE - 1);
  
  displayArray(a, SIZE);

  assert(isSorted(a, SIZE));

  free(a);

  a = generateRandomArray(SIZE, 20);

  displayArray(a, SIZE);

  assert(!isSorted(a, SIZE));
  
  tail_recursive_quicksort(a, 0, SIZE - 1);
  
  displayArray(a, SIZE);
  
  assert(isSorted(a, SIZE));

  free(a);

  a = generateRandomArray(SIZE, 20);

  displayArray(a, SIZE);

  assert(!isSorted(a, SIZE));
  
  three_way_quicksort(a, 0, SIZE - 1);
  
  displayArray(a, SIZE);
  
  assert(isSorted(a, SIZE));

  free(a);

  a = generateRandomArray(SIZE, 20);

  displayArray(a, SIZE);

  assert(!isSorted(a, SIZE));
  
  dual_pivot_quicksort(a, 0, SIZE - 1);
  
  displayArray(a, SIZE);
  
  assert(isSorted(a, SIZE));

  free(a);

  a = generateRandomArray(((int) SIZE * 10), 20);

  displayArray(a, ((int) SIZE * 10));

  assert(!isSorted(a, ((int) SIZE * 10)));
  
  dual_pivot_quicksort(a, 0, ((int) SIZE * 10) - 1);
  
  displayArray(a, ((int) SIZE * 10));
  
  assert(isSorted(a, ((int) SIZE * 10)));

  free(a);
}