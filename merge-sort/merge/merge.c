#include <stdio.h>
#include <stdlib.h>

void twowayMerge(int *zs, int *xs, int n, int *ys, int m) {
  int i = 0, j = 0, k = 0;

  while (k < n + m) {
    if (i == n) {
      zs[k++] = ys[j++];
      continue;
    }

    if (j == m) {
      zs[k++] = xs[i++];
      continue;
    }

    zs[k] = xs[i] < ys[j] ? xs[i++] : ys[j++];
    k++;
  }
}