#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifndef NULL
#define NULL ((void *) 0)
#endif

int isSorted(int *a, int s) {
  int sorted = 1;
  int i = 0;

  while (i++ < s) {
    sorted = *(a + i - 1) <= *(a + i) ? sorted : 0;
  }

  return sorted;
}

int *generateRandomArray(int s, int n) {
  srand(time(NULL));
  int *a = malloc(sizeof(int) * s);
  int i = 0;

  while (i < s) {
    *(a + i) = (int) rand() % n;
    i++;
  }

  return a;
}

void displayArray(int *a, int s) {
  int i = 0;
  printf("\n");

  while (i < s) {
    printf("%d ", *(a + i));
    i++;
  }

  printf("\n");
}