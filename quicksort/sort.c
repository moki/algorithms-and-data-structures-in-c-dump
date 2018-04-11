#include <stdlib.h>
#include <stdio.h>
#include "sort-utils.h"

void exchange(int *xs, int x, int y) {
  int t = xs[x];

  xs[x] = xs[y];
  xs[y] = t;
}

int partition(int *xs, int l, int r) {
  int i = l - 1;
  int j = r;
  int v = xs[r];

  for (;;) {
    while (xs[++i] < v);
    while (v < xs[--j]) if (j == i) break;
    if (i >= j) break;
    exchange(xs, i, j);
  }
  exchange(xs, i, r);
  return i;
}

void recursive_quicksort(int *xs, int l, int r) {
  if (r <= l) {
    return;
  }

  int i = partition(xs, l, r);

  recursive_quicksort(xs, l, i - 1);
  recursive_quicksort(xs, i + 1, r);
}

void tail_recursive_quicksort(int *xs, int l, int r) {
  while (l < r) {
    int i = partition(xs, l, r);

    tail_recursive_quicksort(xs, l, i - 1);

    l = i + 1;
  }
}

void three_way_quicksort(int *xs, int l, int r) {
  if (r <= l) {
    return;
  }
  
  int i = l - 1, p = l - 1, k, q = r, j = r, v = xs[r];

  for (;;) {
    while (xs[++i] < v);
    while (v < xs[--j]) if (j == l) break;
    if (i >= j) break;

    exchange(xs, i, j);

    if (xs[i] == v) {
      p++;
      exchange(xs, p, i);
    }

    if (xs[j] == v) {
      q--;
      exchange(xs, j, q);
    }
  }

  exchange(xs, i, r);
  j = i - 1;
  i = i + 1;

  for (k = l; k < p; k++, j--) exchange(xs, k, j);
  for (k = r - 1; k > q; k--, i++) exchange(xs, k, i);

  three_way_quicksort(xs, l, j);
  three_way_quicksort(xs, i, r);
}


typedef struct dual_pivot {
  int lt;
  int gt;
} dual_pivot;

dual_pivot dual_pivot_partition(int *xs, int l, int r) {
  if (xs[l] > xs[r]) exchange(xs, l, r);
  int k, i, j;

  k = i = l + 1;
  j = r - 1;

  int lt = xs[l];
  int gt = xs[r];

  for (; k <= j; k++) {
    if (xs[k] < lt) exchange(xs, k, i++);
    else if (xs[k] >= gt) {
      while (k < j && xs[j] > gt) j--;
      exchange(xs, k, j--);
      if (xs[k] < lt) exchange(xs, k, i++);
    }
  }

  exchange(xs, l, i - 1);
  exchange(xs, r, j + 1);

  dual_pivot pivots = {
    j + 1,
    i - 1,
  };

  return pivots;
}

void dual_pivot_quicksort(int *xs, int l, int r) {
  if (r <= l) {
    return;
  }

  // insertion sort small input
  if (r - l < 27) {
    int i;
    for (i = l + 1; i <= r; i++) {
      int j;
      for (j = i; j > l && xs[j] < xs[j - 1]; j--) {
        exchange(xs, j, j - 1);
      }
    }

    return;
  }

  dual_pivot dp = dual_pivot_partition(xs, l, r);
  
  int lt = dp.lt;
  int gt = dp.gt;

  dual_pivot_quicksort(xs, l, lt - 1);
  dual_pivot_quicksort(xs, lt + 1, gt - 1);
  dual_pivot_quicksort(xs, gt + 1, r);
}