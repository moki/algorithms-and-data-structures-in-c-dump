#include <stdio.h>
#include <stdlib.h>
#include "merge.h"

#define SIZE_A (5)
#define SIZE_B (8)

int main(int argc, char **argv) {
  // create array *a [0,...,4] and array *b [5,...,9]
  int a[SIZE_A] = {5, 19, 35, 55, 80};
  int b[SIZE_B] = {7, 15, 25, 40, 60, 77, 85, 100};
  int c[SIZE_A + SIZE_B];

  // display array *a [0,...,4] and array *b [5,...,9]
  int i = 0;

  while (i < SIZE_A) {
    printf("%d ", a[i]);
    i++;
  }

  i = 0;

  while (i < SIZE_B) {
    printf("%d ", b[i]);
    i++;
  }

  putchar('\n');

  twowayMerge(c, a, SIZE_A, b, SIZE_B);

  i = 0;
  
  while (i < SIZE_A + SIZE_B) {
    printf("%d ", c[i]);
    i++;
  }

  putchar('\n');
}