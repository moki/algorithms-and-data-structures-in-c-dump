#include <stdio.h>
#include <stdlib.h>
#include "splay-tree.h"

void test_int_bst();
int cmp(const void *x, const void *y);
void _display_int(const void *k);

int main(int argc, char **argv) {
	test_int_bst();
}

void test_int_bst() {
	splay_tree splay_tree_int = splay_tree_constructor(cmp);

	int x = 10;
	splay_tree_insert(splay_tree_int, &x);
	int y = 5;
	splay_tree_insert(splay_tree_int, &y);
	int z = 3;
	splay_tree_insert(splay_tree_int, &z);
	int q = 1;
	splay_tree_insert(splay_tree_int, &q);
	int s = 4;
	splay_tree_insert(splay_tree_int, &s);
	int o = 9;
	splay_tree_insert(splay_tree_int, &o);
	int m = 33;
	splay_tree_insert(splay_tree_int, &m);
	int p = 20;
	splay_tree_insert(splay_tree_int, &p);
	int e = 11;
	splay_tree_insert(splay_tree_int, &e);

	splay_tree_traverse(splay_tree_int, _display_int);

	printf("\nfind & printf &s(4): ");
	splay_tree_apply_fn_to_node(splay_tree_int, splay_tree_search(splay_tree_int, &s), _display_int);

	printf("\nmin: ");
	splay_tree_apply_fn_to_node(splay_tree_int, splay_tree_minimum(splay_tree_int), _display_int);

	printf("\nmax: ");
	splay_tree_apply_fn_to_node(splay_tree_int, splay_tree_maximum(splay_tree_int), _display_int);

	printf("\n4's successor: ");
	splay_tree_apply_fn_to_node(splay_tree_int, splay_tree_successor(splay_tree_search(splay_tree_int, &s)), _display_int);

	printf("\n9's successor: ");
	splay_tree_apply_fn_to_node(splay_tree_int, splay_tree_successor(splay_tree_search(splay_tree_int, &o)), _display_int);

	printf("\n11's predecessor: ");
	splay_tree_apply_fn_to_node(splay_tree_int, splay_tree_predecessor(splay_tree_search(splay_tree_int, &e)), _display_int);

	putchar('\n');

	splay_tree_delete(splay_tree_int, &m);

	splay_tree_delete(splay_tree_int, &z);

	splay_tree_traverse(splay_tree_int, _display_int);
	putchar('\n');

	splay_tree_delete(splay_tree_int, &x);

	splay_tree_traverse(splay_tree_int, _display_int);
	putchar('\n');

	splay_tree_destructor(splay_tree_int);
}

void _display_int(const void *k) {
	printf("%d ", *(int *) k);
}

int cmp(const void *x, const void *y) {
	return * (int *) x - * (int *) y;
}