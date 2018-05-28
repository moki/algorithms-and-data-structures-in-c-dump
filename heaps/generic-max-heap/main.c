#include <time.h>
#include <stdlib.h>
#include "sort-utils.h"
#include "heap.h"

#define SIZE (5)
#define MAXVAL (10)

int cmp(const void *x, const void *y);

int main(int argc, char **argv) {
	srand(time(NULL));

	int *xs = generateRandomArray(SIZE, MAXVAL);

	displayArray(xs, SIZE);

	heap_sort(xs, SIZE, sizeof(*xs), cmp, NULL);

	displayArray(xs, SIZE);
}

/*
 * cmp - compares two values
 * returns negative int if k < e
 * 	   positive int if k > e
 * 	   zero 	if equals
*/
int cmp(const void *x, const void *y) {
	return * (int *) x - * (int *) y;
}