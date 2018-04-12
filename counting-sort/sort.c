#include <stdlib.h>

int *countingSort(int *xs, int s, int k) {
  // working storage
  int *ys = calloc(sizeof(int), k);
  // sorted output
  int *zs = malloc(sizeof(int) * s);

  int i;
  for (i = 0; i < s; i++) {
    ys[xs[i]]++;
  }

  for (i = 0; i < k - 1; i++) {
    ys[i + 1] = ys[i] + ys[i + 1];
  }

  for (i = s - 1; i >= 0; i--) {
    zs[ys[xs[i]] - 1] = xs[i];
    ys[xs[i]]--;
  }

  return zs;
}