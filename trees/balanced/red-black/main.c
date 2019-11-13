#include "rbt.h"

int main(int argc, char **argv) {
	rbt_t *tree = rbt_constructor(sizeof(int), sizeof(int), NULL, NULL, RBT_REBALANCE_MODE_TOP_DOWN);

	int k_a = 1;
	int v_a = 10;

	rbt_insert(tree, &k_a, &v_a);

	int k_b = 2;
	int v_b = 20;

	rbt_insert(tree, &k_b, &v_b);

	int k_c = 3;
	int v_c = 30;

	rbt_insert(tree, &k_c, &v_c);

	int k_d = 4;
	int v_d = 40;

	rbt_insert(tree, &k_d, &v_d);

	int k_e = 5;
	int v_e = 50;

	rbt_insert(tree, &k_e, &v_e);

	int k_f = 6;
	int v_f = 60;

	rbt_insert(tree, &k_f, &v_f);

	int k_g = 10;
	int v_g = 100;

	rbt_insert(tree, &k_g, &v_g);

	rbt_delete(tree, &k_g);
	rbt_delete(tree, &k_f);
	rbt_delete(tree, &k_b);

	int *r;

	r = rbt_find(tree, &k_a);

	printf("l: %d, f: %d\n", k_a, r ? *(int *) r : 0);

	r = rbt_find(tree, &k_d);

	printf("l: %d, f: %d\n", k_d, r ? *(int *) r : 0);

	r = rbt_find(tree, &k_f);

	printf("l: %d, f: %d\n", k_f, r ? *(int *) r : 0);

	rbt_delete(tree, &k_e);
	rbt_delete(tree, &k_a);


	rbt_destructor(tree);
}