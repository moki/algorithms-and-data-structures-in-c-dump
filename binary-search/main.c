#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "search.h"

#define SIZE (10)

void displayArray(int *, int);

void generateSortedArray(int *, int, int);

int cmp(const void *k, const void *e);

int main(int argc, char **argv) {
	int *xs = malloc(sizeof(int) * SIZE);

	generateSortedArray(xs, 4, SIZE);

	displayArray(xs, SIZE);

	int x = 1;
	int y = 3;

	assert(cmp(&x, &y) == -2);

	int z = 8;

	void *zp = _bsearch(&z, xs, SIZE, sizeof(int), cmp);

	assert(zp);

	if (zp) printf("The value(%d) pointer is: %p, index in the array: %ld\n", z, zp, (int *) zp - xs);

	z = 9;

	zp = _bsearch(&z, xs, SIZE, sizeof(int), cmp);

	assert(zp);

	if (zp) printf("The value(%d) pointer is: %p, index in the array: %ld\n", z, zp, (int *) zp - xs);

	z = 15;

	zp = _bsearch(&z, xs, SIZE, sizeof(int), cmp);

	assert(!zp);

	if (zp) printf("The value(%d) pointer is: %p, index in the array: %ld\n", z, zp, (int *) zp - xs);

	z = 4;

	zp = _bsearch(&z, xs, SIZE, sizeof(int), cmp);

	assert(zp);

	if (zp) printf("The value(%d) pointer is: %p, index in the array: %ld\n", z, zp, (int *) zp - xs);

	z = 6;

	zp = _bsearch(&z, xs, SIZE, sizeof(int), cmp);

	assert(zp);

	if (zp) printf("The value(%d) pointer is: %p, index in the array: %ld\n", z, zp, (int *) zp - xs);

	free(xs);
}

// generate sorted array in range [l, r)
void generateSortedArray(int *xs, int l, int r) {
	int i;

	for (i = 0; i < r; i++) {
		xs[i] = l + i;
	}
}

void displayArray(int* xs, int s) {
	int i;

	for (i = 0; i < s; i++) {
		printf("%d%c", xs[i], i == (s - 1) ? '\n' : ' ');
	}
}

/*
 * cmp - compares two values
 * returns negative int if k < e
 * 	   positive int if k > e
 * 	   zero 	if equals
*/
int cmp(const void *k, const void *e) {
	return * (int *) k - * (int *) e;
}