#include "bst.h"

static size_t key_length_fn(const void *key);

static void display_node_key_int(bst_node_t *node);
static void _free(bst_node_t *node);

int main(int argc, char **argv) {
	bst_t *tree = bst_constructor(NULL, NULL, key_length_fn, NULL);
	if (!tree) return -1;

	int x_k = 5;
	int x_v = 15;
	bst_insert(tree, &x_k, &x_v);

	int y_k = 2;
	int y_v = 12;
	bst_insert(tree, &y_k, &y_v);

	int z_k = 3;
	int z_v = 13;
	bst_insert(tree, &z_k, &z_v);

	int a_k = 4;
	int a_v = 14;
	bst_insert(tree, &a_k, &a_v);

	int result;

	result = *(int *) bst_find(tree, &x_k);
	printf("k: %d, v: %d\n", x_k, result);

	result = *(int *) bst_find(tree, &y_k);
	printf("k: %d, v: %d\n", y_k, result);

	result = *(int *) bst_find(tree, &z_k);
	printf("k: %d, v: %d\n", z_k, result);

	result = *(int *) bst_find(tree, &a_k);
	printf("k: %d, v: %d\n", a_k, result);

	// FILE *f;
	// f = fopen("bst.gv", "w+");
	// if (!f) return -1;

	// bst_draw(tree, f);
	// fclose(f);

	int i;
	for (i = BST_TRAVERSE_PRE_ORDER; i <= BST_TRAVERSE_BREADTH_FIRST; i++) {
		bst_traverse(tree, i, display_node_key_int);
		putchar('\n');
	}

	bst_node_t *r = bst_find_interval(tree, &y_k, &x_k);

	bst_traverse_list(r, display_node_key_int);
	putchar('\n');

	bst_traverse_list(r, _free);

	bst_delete(tree, &a_k);

	result = bst_find(tree, &a_k) && *(int *) bst_find(tree, &a_k);
	printf("k: %d, v: %d\n", a_k, result);

	bst_destructor(tree);

	int *ks = malloc(sizeof(int) * 10);

	for (i = 0; i < 5; i++)
		*(ks + i) = i + 1;

	int *vs = malloc(sizeof(int) * 10);

	for (i = 0; i < 5; i++)
		*(vs + i) = (i + 1) * 10;

	bst_node_t *node_list = NULL;

	for (i = 4; i >= 0; i--) {
		node_list = bst_new_node(ks + i, vs + i, node_list);
	}

	bst_traverse_list(node_list, display_node_key_int);
	putchar('\n');

	bst_t *_tree = bst_constructor_list_bottom_up(node_list, NULL, NULL, key_length_fn, NULL);
	// bst_t *_tree = bst_constructor_list_top_to_bottom(node_list, NULL, NULL, key_length_fn, NULL);

	FILE *f;
	f = fopen("bst.gv", "w+");
	if (!f) return -1;

	bst_draw(_tree, f);
	fclose(f);

	result = *(int *) bst_find(_tree, ks + 1);
	printf("k: %d, v: %d\n", *(ks + 1), result);

	bst_node_t *_list = bst_flatten(_tree);

	bst_traverse_list(_list, display_node_key_int);
	putchar('\n');

	bst_traverse_list(_list, _free);
	// bst_destructor(_tree);

	free(ks);
	free(vs);
}

static size_t key_length_fn(const void *key) {
	return sizeof(key);
}

static void display_node_key_int(bst_node_t *node) {
	printf("%d ", *(int *) bst_get_node_key(node));
}

static void _free(bst_node_t *node) {
	free((void *) node);
	node = NULL;
}