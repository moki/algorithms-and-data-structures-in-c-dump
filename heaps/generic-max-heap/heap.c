#include <stddef.h>
#include <stdio.h>
#include "heap.h"

// Heap heap_constructor();

static void generic_swap(void *x, void *y, int s) {
	char t;

	do {
		t = *(char *) x;
		*(char *)x++ = *(char *)y;
		*(char *)y++ = t;
	} while (--s > 0);
}

// void heap_heapify(void *base, size_t num, size_t s,
// 		  int (*cmp)(const void *, const void *),
// 		  void (swp)(void *, void *, int s));

void heap_sort(void *base, size_t num, size_t s,
	       int (*cmp)(const void *, const void *),
	       void (swp)(void *, void *, int s)) {
	int i = (num / 2 - 1) * s, n = num * s, c, r;

	if (!swp) {
		swp = generic_swap;
	}

	/* heapify */
	for ( ; i >= 0; i -= s) {
		for (r = i; r * 2 + s < n; r = c) {
			c = r * 2 + s;

			if (c < n - s && cmp(base + c, base + c + s) < 0)
				c += s;

			if (cmp(base + r, base + c) >= 0)
				break;

			swp(base + r, base + c, s);
		}
	}

	/* sort */
	for (i = n - s; i > 0; i -= s) {
		swp(base, base + i, s);

		for (r = 0; r * 2 + s < i; r = c) {
			c = r * 2 + s;

			if (c < i - s && cmp(base + c, base + c + s) < 0)
				c += s;

			if (cmp(base + r, base + c) >= 0)
				break;

			swp(base + r, base + c, s);
		}
	}
}
