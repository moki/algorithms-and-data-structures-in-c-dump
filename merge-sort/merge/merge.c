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

void abstractInplaceMerge(int *xs, int l, int m, int r) {
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
}