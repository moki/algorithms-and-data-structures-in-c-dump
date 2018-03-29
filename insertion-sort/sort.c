#include <stdio.h>

void InsertionSort(int *a, int size) {
  int i = 0;

  while (i < size) {
    int j = i;

    while (j > 0 && *(a + j) < *(a + j - 1)) {
      int t = *(a + j - 1);
      *(a + j - 1) = *(a + j);
      *(a + j) = t;

      j--;
    }

    i++;
  }
}