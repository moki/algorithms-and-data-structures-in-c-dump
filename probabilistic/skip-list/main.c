#include <stdio.h>
#include <time.h>
#include <limits.h>
#include "skip-list.h"

#define TEST_INT_VALUES_N ( (unsigned int) ( ((unsigned int)1) << ((unsigned int)(8)) ) )

static int seed_reset = 0;
static int _random(void);

static int cmp_int(const void *, const void *);
static void display_int(void *);
static void test_sl_int(void);

int main(int argc, char **argv) {
	test_sl_int();
}

static int cmp_int(const void *x, const void *y) {
	return * (int *) x - * (int *) y;
}

static void display_int(void *key) {
	printf("%d ", *(int *) key);
}

static int _random(void) {
	if (!seed_reset) {
		srand(time(NULL));
		seed_reset = 1;
	}

	return (unsigned int)(rand() % RAND_MAX);
}

static void test_sl_int(void) {
	skip_list sl_int = sl_constructor(cmp_int);
	int *xs = malloc(sizeof(int *) * (int) TEST_INT_VALUES_N);
	int i;

	/* generate array of random values */
	for (i = 0; i < TEST_INT_VALUES_N; i++)
		*(xs+i) = _random() % (int) (1 << 31);

	/* insert test */
	for (i = 0; i < TEST_INT_VALUES_N; i++)
		sl_insert(sl_int, xs + i);

	/* display test */
	sl_traverse_apply_key(sl_int, display_int);
	putchar('\n');

	/* find k smallest test */
	printf("\nTable\n[k-th]:[value]\n\n");

	unsigned int k;
	void *v;
	for (i = 0; i < TEST_INT_VALUES_N; i++) {
		k = (_random() % TEST_INT_VALUES_N);
		v = sl_find_k_smallest(sl_int, k);
		printf("%d:%d%s",
			k,
			v ? *(int *) v : -1,
			i + 1 != TEST_INT_VALUES_N ? ", " : "");

		if (i && (i + 1) % 5 == 0)
			putchar('\n');
	}

	putchar('\n');

	free(xs);
	sl_destructor(sl_int);
}