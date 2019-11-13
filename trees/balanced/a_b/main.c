#include "abt.h"
#include <assert.h>

static int a = 2;
static int b = 4;

int main(int argc, char **argv) {
	abt_t *tree = abt_constructor(sizeof(int), sizeof(int), a, b);

	int k_a = 1;
	int v_a = 10;

	abt_insert(tree, &k_a, &v_a);

	int k_b = 2;
	int v_b = 20;

	abt_insert(tree, &k_b, &v_b);

	int k_c = 3;
	int v_c = 30;

	abt_insert(tree, &k_c, &v_c);

	int k_d = 4;
	int v_d = 40;

	abt_insert(tree, &k_d, &v_d);

	// int k_e = 5;
	// int v_e = 50;

	// abt_insert(tree, &k_e, &v_e);

	// int k_f = 6;
	// int v_f = 60;

	// abt_insert(tree, &k_f, &v_f);

	// int k_g = 7;
	// int v_g = 70;

	// abt_insert(tree, &k_g, &v_g);

	// int k_h = 8;
	// int v_h = 80;

	// abt_insert(tree, &k_h, &v_h);

	// int k_i = 9;
	// int v_i = 90;

	// abt_insert(tree, &k_i, &v_i);

	void *r = abt_find(tree, &k_a);

	printf("looked k: %d, found v: %d\n", k_a, *(int *) r);
	free(r);

	r = abt_find(tree, &k_b);

	printf("looked k: %d, found v: %d\n", k_b, *(int *) r);
	free(r);

	r = abt_find(tree, &k_c);

	printf("looked k: %d, found v: %d\n", k_c, *(int *) r);
	free(r);

	r = abt_find(tree, &k_d);

	printf("looked k: %d, found v: %d\n", k_d, *(int *) r);
	free(r);

	// r = abt_find(tree, &k_e);

	// printf("looked k: %d, found v: %d\n", k_e, *(int *) r);
	// free(r);

	// r = abt_find(tree, &k_f);

	// printf("looked k: %d, found v: %d\n", k_f, *(int *) r);
	// free(r);

	// r = abt_find(tree, &k_g);

	// printf("looked k: %d, found v: %d\n", k_g, *(int *) r);
	// free(r);

	// r = abt_find(tree, &k_h);

	// printf("looked k: %d, found v: %d\n", k_h, *(int *) r);
	// free(r);

	// r = abt_find(tree, &k_i);

	// printf("looked k: %d, found v: %d\n", k_i, *(int *) r);
	// free(r);

	abt_delete(tree, &k_a);
	abt_delete(tree, &k_b);
	abt_delete(tree, &k_c);
	abt_delete(tree, &k_d);
	// abt_delete(tree, &k_e);
	// abt_delete(tree, &k_f);
	// abt_delete(tree, &k_g);
	// abt_delete(tree, &k_h);
	// abt_delete(tree, &k_i);

	// r = abt_find(tree, &k_i);

	// printf("looked k: %d, found v: %d\n", k_i, r ? *(int *) r : 0);

	r = NULL;

	abt_destructor(tree);
}