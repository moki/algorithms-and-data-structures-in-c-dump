#include "hbt.h"

static void hbt_traverse_post_order(hbt_node_t *root, void (*fn)(hbt_node_t *node));
static int compare(void *a, void *b, size_t (*len_fn)(const void *a));
static void hbt_rotation(hbt_node_t *node, unsigned char dir);
static void hbt_draw_r(hbt_node_t *cursor, FILE *stream);
static void hbt_rebalance(stack_t *stack);
static void free_node(hbt_node_t *node);

struct hbt {
	hbt_node_t *root;
	key_length_fn_t	 key_length_fn;
};

struct hbt_node {
	void *key;
	void *edge[2];
	unsigned char height;
};

hbt_t *hbt_constructor(key_length_fn_t key_length_fn) {
	hbt_t *hbt = malloc(sizeof(*hbt));
	if (!hbt) return NULL;

	hbt->key_length_fn = key_length_fn;

	hbt->root = hbt_new_node(NULL, NULL, NULL, 0);
	if (!hbt->root) return NULL;

	return hbt;
}

void hbt_destructor(hbt_t *hbt) {
	if (!hbt) return;

	if (!hbt->root) {
		goto free_hbt;
	}

	if (!hbt->root->edge[0]) {
		free(hbt->root);
		hbt->root = NULL;
		goto free_hbt;
	}

	hbt_traverse_post_order(hbt->root, free_node);

free_hbt:
	free(hbt);
	hbt = NULL;
}

void *hbt_find(hbt_t *hbt, void *key) {
	if (!hbt->root->edge[0])
		return NULL;

	hbt_node_t *cursor = hbt->root;
	int keys_difference;

	while (cursor->edge[1]) {
		keys_difference = compare(key, cursor->key, hbt->key_length_fn);

		cursor = keys_difference < 0 ? cursor->edge[0] : cursor->edge[1];
	}

	keys_difference = compare(key, cursor->key, hbt->key_length_fn);
	return keys_difference == 0 ? cursor->edge[0] : NULL;
}

void hbt_insert(hbt_t *hbt, void *key, void *val) {
	if (!hbt) return;

	if (!hbt->root->edge[0]) {
		hbt->root->edge[0] = val;
		hbt->root->key 	   = key;
		return;
	}

	stack_t    *path  = stack_constructor();
	hbt_node_t *cursor = hbt->root;

	int keys_difference;

	while (cursor->edge[1]) {
		stack_push(path, cursor);
		keys_difference = compare(key, cursor->key, hbt->key_length_fn);

		cursor = keys_difference < 0 ? cursor->edge[0] : cursor->edge[1];
	}

	keys_difference = compare(key, cursor->key, hbt->key_length_fn);

	if (keys_difference == 0)
		return;

	/* insert */

	hbt_node_t *old_leaf, *new_leaf;

	old_leaf = hbt_new_node(cursor->key, cursor->edge[0], NULL, 0);

	new_leaf = hbt_new_node(key, val, NULL, 0);

	if (keys_difference > 0) {
		cursor->edge[0] = old_leaf;
		cursor->edge[1] = new_leaf;
		cursor->key 	= key;
	} else {
		cursor->edge[0] = new_leaf;
		cursor->edge[1] = old_leaf;
	}

	cursor->height = 1;

	hbt_rebalance(path);
	stack_destructor(path);
}

void hbt_delete(hbt_t *hbt, void *key) {
	if (!hbt->root->edge[0])
		return;

	if (!hbt->root->edge[1]) {
		if (compare(hbt->root->key, key, hbt->key_length_fn) == 0) {
			hbt->root->key = NULL;
			hbt->root->edge[0] = NULL;
		}

		return;
	}

	stack_t *path  = stack_constructor();

	hbt_node_t *cursor, *parent, *sibling;
	int keys_difference;

	cursor = hbt->root;

	while (cursor->edge[1]) {
		stack_push(path, cursor);

		parent = cursor;

		keys_difference = compare(key, cursor->key, hbt->key_length_fn);

		cursor = parent->edge[keys_difference >= 0];
		sibling = parent->edge[keys_difference < 0];
	}

	if (compare(cursor->key, key, hbt->key_length_fn) != 0)
		goto clean;

	parent->key     = sibling->key;
	parent->edge[0] = sibling->edge[0];
	parent->edge[1] = sibling->edge[1];
	parent->height  = sibling->height;

	free(cursor);
	cursor = NULL;

	free(sibling);
	sibling = NULL;

	(void) stack_pop(path);
	hbt_rebalance(path);
clean:
	stack_destructor(path);
}

void *hbt_get_node_key(hbt_node_t *node) {
	return node->key;
}

hbt_node_t *hbt_new_node(void *key, void *left, void *right, unsigned char height) {
	hbt_node_t *new_node = malloc(sizeof(*new_node));
	if (!new_node) return NULL;

	new_node->edge[1] = right;
	new_node->edge[0] = left;
	new_node->height = height;
	new_node->key = key;

	return new_node;
}

static void hbt_rotation(hbt_node_t *node, unsigned char dir) {
	hbt_node_t *t_n = node->edge[dir];
	void	   *t_k = node->key;

	node->edge[dir]  = node->edge[!dir];
	node->key	 = ((hbt_node_t *) node->edge[!dir])->key;
	node->edge[!dir] = ((hbt_node_t *) node->edge[dir])->edge[!dir];
	((hbt_node_t *)node->edge[dir])->edge[!dir] = ((hbt_node_t *) node->edge[dir])->edge[dir];


	((hbt_node_t *)node->edge[dir])->edge[dir] = t_n;
	((hbt_node_t *)node->edge[dir])->key       = t_k;
}

static void hbt_rebalance(stack_t *path) {
	hbt_node_t *cursor;
	unsigned char finished = 0;
	int lr_subtree_difference;

	while (!stack_is_empty(path) && !finished) {
		unsigned char old_height;
		unsigned char dir;

		cursor 	   = stack_pop(path);
		old_height = cursor->height;

		lr_subtree_difference = ((hbt_node_t *) cursor->edge[0])->height - ((hbt_node_t *) cursor->edge[1])->height;

		if (lr_subtree_difference <= 1 && lr_subtree_difference >= -1) {
			cursor->height = ((hbt_node_t *) cursor->edge[0])->height > ((hbt_node_t *) cursor->edge[1])->height
				? ((hbt_node_t *) cursor->edge[0])->height + 1
				: ((hbt_node_t *) cursor->edge[1])->height + 1;
		} else {
			dir = lr_subtree_difference == 2;

			if (((hbt_node_t *) ((hbt_node_t *) cursor->edge[!dir])->edge[!dir])->height -
				((hbt_node_t *) cursor->edge[dir])->height == 1) {
				hbt_rotation(cursor, dir);
	       			((hbt_node_t *) cursor->edge[dir])->height = ((hbt_node_t *) ((hbt_node_t *) cursor->edge[dir])->edge[!dir])->height + 1;
				cursor->height = ((hbt_node_t *) cursor->edge[dir])->height + 1;
			} else {
				hbt_rotation(cursor->edge[!dir], !dir);
				hbt_rotation(cursor, dir);
				((hbt_node_t *) cursor->edge[0])->height = ((hbt_node_t *) ((hbt_node_t *) cursor->edge[!dir])->edge[!dir])->height + 1;
				((hbt_node_t *) cursor->edge[1])->height = ((hbt_node_t *) ((hbt_node_t *) cursor->edge[!dir])->edge[!dir])->height + 1;
				cursor->height = ((hbt_node_t *) ((hbt_node_t *) cursor->edge[!dir])->edge[!dir])->height + 2;
			}
		}

		if (cursor->height == old_height)
				finished = 1;
	}
}

void hbt_draw(hbt_t *tree, FILE *stream) {
	fprintf(stream, "digraph hbt {\n");
	fprintf(stream, "	node [fontname=\"Monospace\"];\n");

	if (!tree->root)
		fprintf(stream, "\n");
	else if (!tree->root->edge[1])
		fprintf(stream, "	%d;\n", *(int *) tree->root->key);
	else
		hbt_draw_r(tree->root, stream);

	fprintf(stream, "}\n");
}

static void hbt_draw_r(hbt_node_t *cursor, FILE *stream) {
	if (!cursor->edge[1]) {
		fprintf(stream, "	%d [shape=box];\n", *(int *) cursor->edge[0]);
		fprintf(stream, "	%d -> %d;\n", *(int *) cursor->key, *(int *) cursor->edge[0]);
		return;
	}

	fprintf(stream, "	%d -> %d;\n", *(int *) cursor->key, *(int *)((hbt_node_t *) cursor->edge[0])->key);
	hbt_draw_r(cursor->edge[0], stream);

	fprintf(stream, "	%d -> %d;\n", *(int *) cursor->key, *(int *)((hbt_node_t *) cursor->edge[1])->key);
	hbt_draw_r(cursor->edge[1], stream);
}

void hbt_traverse_post_order(hbt_node_t *root, void (*fn)(hbt_node_t *node)) {
	stack_t *stack = stack_constructor();
	if (!stack) return;

	hbt_node_t *cursor = root;
	hbt_node_t *last;
	hbt_node_t *peek;

	while (!stack_is_empty(stack) || cursor) {
		if (cursor) {
			stack_push(stack, cursor);
			cursor = cursor->edge[0] && cursor->edge[1] ? cursor->edge[0] : NULL;
		} else {
			peek = stack_peek(stack);

			if (peek->edge[1] && last != peek->edge[1])
				cursor = peek->edge[1];
			else {
				fn(peek);
				last = stack_pop(stack);
			}
		}
	}

	stack_destructor(stack);
}

static int compare(void *a, void *b, size_t (*len_fn)(const void *a)) {
	unsigned char keys_are_same_size;

	keys_are_same_size = len_fn(a) == len_fn(b);
	if (!keys_are_same_size) exit(EXIT_FAILURE);

	return memcmp(a, b, len_fn(a));
}

static void free_node(hbt_node_t *node) {
	free(node);
	node = NULL;
}