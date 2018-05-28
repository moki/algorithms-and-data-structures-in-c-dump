#include <stdio.h>
#include <stdlib.h>
#include "binary-search-tree.h"

void test_int_bst();
int cmp(const void *x, const void *y);
void _display_int(const void *k);

int main(int argc, char **argv) {
	test_int_bst();
}

void test_int_bst() {
	binary_search_tree bst_int_cst = binary_search_tree_constructor(cmp);

	int x = 10;
	binary_search_tree_insert(bst_int_cst, &x);
	int y = 5;
	binary_search_tree_insert(bst_int_cst, &y);
	int z = 3;
	binary_search_tree_insert(bst_int_cst, &z);
	int q = 1;
	binary_search_tree_insert(bst_int_cst, &q);
	int s = 4;
	binary_search_tree_insert(bst_int_cst, &s);
	int o = 9;
	binary_search_tree_insert(bst_int_cst, &o);
	int m = 33;
	binary_search_tree_insert(bst_int_cst, &m);
	int p = 20;
	binary_search_tree_insert(bst_int_cst, &p);
	int e = 11;
	binary_search_tree_insert(bst_int_cst, &e);

	binary_search_tree_traverse(bst_int_cst, _display_int);

	printf("\nfind & printf &s(4): ");
	binary_search_tree_apply_fn_to_node(bst_int_cst, binary_search_tree_search(bst_int_cst, &s), _display_int);

	printf("\nmin: ");
	binary_search_tree_apply_fn_to_node(bst_int_cst, binary_search_tree_minimum(bst_int_cst), _display_int);

	printf("\nmax: ");
	binary_search_tree_apply_fn_to_node(bst_int_cst, binary_search_tree_maximum(bst_int_cst), _display_int);

	printf("\n4's successor: ");
	binary_search_tree_apply_fn_to_node(bst_int_cst, binary_search_tree_successor(binary_search_tree_search(bst_int_cst, &s)), _display_int);

	printf("\n9's successor: ");
	binary_search_tree_apply_fn_to_node(bst_int_cst, binary_search_tree_successor(binary_search_tree_search(bst_int_cst, &o)), _display_int);

	printf("\n11's predecessor: ");
	binary_search_tree_apply_fn_to_node(bst_int_cst, binary_search_tree_predecessor(binary_search_tree_search(bst_int_cst, &e)), _display_int);

	putchar('\n');

	binary_search_tree_delete(bst_int_cst, &m);

	binary_search_tree_delete(bst_int_cst, &z);

	binary_search_tree_traverse(bst_int_cst, _display_int);
	putchar('\n');

	binary_search_tree_delete(bst_int_cst, &x);

	binary_search_tree_traverse(bst_int_cst, _display_int);
	putchar('\n');

	binary_search_tree_destructor(bst_int_cst);
}

void _display_int(const void *k) {
	printf("%d ", *(int *) k);
}

int cmp(const void *x, const void *y) {
	return * (int *) x - * (int *) y;
}