#include <stdio.h>

void *_bsearch(const void *k, const void *xs, size_t n, size_t es,
	       int (*cmp)(const void *k, const void *e)) {
	const char *m;
	int k2m;

	while (n > 0) {
		m = xs + (n >> 1) * es;
		k2m = cmp(k, m);

		if (k2m == 0)
			return (void *) m;

		if (k2m > 0) {
			xs = m + 1 * es;
			n--;
		}

		n >>= 1;
	}

	return NULL;
}
