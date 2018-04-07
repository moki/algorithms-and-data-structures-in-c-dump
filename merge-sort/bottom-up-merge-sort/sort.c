#include <stdlib.h>

void merge(int *xs, int l, int m, int r) {
  int *ys = malloc(sizeof(int) * (r + 1));
  int i, j, k;

  for (i = 0; i < r; i++) {
    xs[i] = ys[i];
  }
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
  int i, m;

  for (m = 1; m <= r - l; m += m ) {
    for (i = l; i <= r - m; i += m + m) {
      merge(xs, i, i + m - 1, (i + m + m - 1) < r ? (i + m + m - 1) : r);
    }
  }
}