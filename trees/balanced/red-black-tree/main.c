#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "red-black-tree.h"

#define MAX_VALUE (100)
#define N (10)

static void test_int(void);

static int cmp_int(const void *x, const void *y);

static void _display_int(const void *);

int main(int argc, char **argv) {
	// init random seed
	srand(time(NULL));
	test_int();
}

static void test_int(void) {
	red_black_tree rbt_int = rb_constructor(cmp_int);
	rb_assert(rbt_int);

	int i;
	int *ts = malloc(sizeof(int) * N);

	for (i = 0; i < N; i++) {
		*(ts + i) = (int) rand() % MAX_VALUE;
		rb_insert(rbt_int, (ts + i));
	}

	rb_assert(rbt_int);

	rb_traverse(rbt_int, _display_int);
	putchar('\n');

	int rn = *(ts + (int) rand() % N);
	printf("Attempting to delete number: %d\n", rn);
	rb_delete(rbt_int, &rn);

	rb_traverse(rbt_int, _display_int);
	putchar('\n');

	rb_destructor(rbt_int);
	free(ts);
}

static int cmp_int(const void *x, const void *y) {
	return * (int *) x - * (int *) y;
}

static void _display_int(const void *k) {
	printf("%d ", *(int *)k);
}