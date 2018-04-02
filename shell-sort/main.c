#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sort.h"
#include "sort-utils.h"

#define SIZE (21)

int main(int argc, char **argv) {
  int *a = generateRandomArray(SIZE, 30);

  assert(!isSorted(a, SIZE));

  ShellSort(a, SIZE);
  
  assert(isSorted(a, SIZE));
}