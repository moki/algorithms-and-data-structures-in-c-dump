#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include "binary-utils.h"
#include <stdio.h>

static void _radixqsort(int *xs, int l, int r, int k);

static void _radixqsrt_u(unsigned *l, unsigned *r, unsigned bit);

void exchange(int *xs, int x, int y) {
  int t = xs[x];

  xs[x] = xs[y];
  xs[y] = t;
}

void exchange_p(unsigned *x, unsigned *y) {
  unsigned t = *x;
  *x = *y;
  *y = t;
}

// 32 - i - 1
// msd bit to lsd
int isithbitset_msd(int n, int i) {
  return (n & (1 << (32 - i - 1))) != 0;
}

// Sedgewick radix quicksort implementation
static void _radixqsort(int *xs, int l, int r, int k) {
  int i = l, j = r;
  
  if (r <= l || k > 32) return;

  while (j != i) {
    while (isithbitset_msd(xs[i], k) == 0 && (i < j)) i++;
    while (isithbitset_msd(xs[j], k) == 1 && (j > i)) j--;
    exchange(xs, i, j);
  }

  if (isithbitset_msd(xs[r], k) == 0) j++;
  _radixqsort(xs, l, j - 1, k + 1);
  _radixqsort(xs, j, r, k + 1);
}

void radixqsort(int *xs, int l, int r) {
  _radixqsort(xs, l, r, 0);
}

// pointers radix quicksort implementation
static void _radixqsrt_u(unsigned *l, unsigned *r, unsigned bit) {
  if (!bit || r < l + 1) return;

  unsigned *ll = l, *rr = r - 1;
  
  for (;;) {
    while (ll < rr && !(*ll & bit)) ll++;
    while (ll < rr && (*rr & bit)) rr--;

    if (ll >= rr) break;
    exchange_p(ll, rr);
  }

  if (!(bit & *ll) && ll < r) ll++;
  bit >>= 1;
  
  _radixqsrt_u(l, ll, bit);
  _radixqsrt_u(ll, r, bit);
}

void radixqsrt(int *xs, const size_t s) {
  size_t i;
  unsigned *ys = (unsigned *) xs;

  for (i = 0; i < s; i++) {
    ys[i] ^= INT_MIN;
  }

  _radixqsrt_u(ys, ys + s, INT_MIN);

  for (i = 0; i < s; i++) {
    ys[i] ^= INT_MIN;
  }
}
