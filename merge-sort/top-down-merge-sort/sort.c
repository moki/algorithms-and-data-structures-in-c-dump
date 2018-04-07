#include <stdlib.h>

void merge(int *xs, int l, int m, int r) {
  int *ys = malloc(sizeof(int) * (r + 1));
  int i, j, k;

  i = m + 1;

  while (i > l) {
    *(ys + i - 1) = *(xs + i - 1);
    i--;
  }

  j = m;

  while (j < r) {
    *(ys + r + m - j) = *(xs + j + 1);
    j++;
  }

  k = l;

  while (k <= r) {
    if (*(ys + j) < *(ys + i)) {
      *(xs + k) = *(ys + j--);
    } else {
      *(xs + k) = *(ys + i++);
    }

    k++;
  }

  free(ys);
}

void mergeSort(int *xs, int l, int r) {
  int m = (r + l) / 2;
  
  if (r <= l) return;

  mergeSort(xs, l, m);
  mergeSort(xs, m + 1, r);
  merge(xs, l, m, r);
}