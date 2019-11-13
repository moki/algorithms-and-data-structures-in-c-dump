#include "bst.h"
#include <stdio.h>

static void bst_traverse_pre_order(bst_node_t *root, void (*fn)(bst_node_t *node));
static void bst_traverse_in_order(bst_node_t *root, void (*fn)(bst_node_t *node));
static void bst_traverse_post_order(bst_node_t *root, void (*fn)(bst_node_t *node));
static void bst_traverse_breadth_first(bst_node_t *root, void (*fn)(bst_node_t *node));

static void free_node(bst_node_t *node);

static void bst_draw_r(bst_node_t *cursor, FILE *stream);

struct bst {
	bst_node_t 	*root;
	key_destroy_fn_t key_destroy_fn;
	val_destroy_fn_t val_destroy_fn;
	key_length_fn_t	 key_length_fn;
	val_length_fn_t  val_length_fn;
};

struct bst_node {
	void *key;
	void *edge[2]; /* bst_node_t *edge[2] internal, void *edge[2] external */
};

bst_t *bst_constructor(
	key_destroy_fn_t key_destroy_fn,
	val_destroy_fn_t val_destroy_fn,
	key_length_fn_t key_length_fn,
	val_length_fn_t val_length_fn)
{
	/* Key length function is mandatory for a bst. */
	if (!key_length_fn) return NULL;

	bst_t *bst = malloc(sizeof(*bst));
	if (!bst) return NULL;

	bst->root = bst_new_node(NULL, NULL, NULL);
	if (!bst->root) return NULL;

	bst->key_destroy_fn = key_destroy_fn;
	bst->val_destroy_fn = val_destroy_fn;
	bst->key_length_fn  = key_length_fn;
	bst->val_length_fn  = val_length_fn;

	return bst;
}

// bst_t *bst_constructor_list_top_to_bottom(
// 	bst_node_t *node_list,
// 	key_destroy_fn_t key_destroy_fn,
// 	val_destroy_fn_t val_destroy_fn,
// 	key_length_fn_t key_length_fn,
// 	val_length_fn_t val_length_fn)
// {
// 	/* Key length function is mandatory for a bst. */
// 	if (!key_length_fn) return NULL;

// 	bst_t *bst = malloc(sizeof(*bst));
// 	if (!bst) return NULL;

// 	bst->key_destroy_fn = key_destroy_fn;
// 	bst->val_destroy_fn = val_destroy_fn;
// 	bst->key_length_fn  = key_length_fn;
// 	bst->val_length_fn  = val_length_fn;

// 	if (!node_list) {
// 		bst->root = bst_new_node(NULL, NULL, NULL);
// 		if (!bst->root) return NULL;

// 		return bst;
// 	}

// 	if (!node_list->edge[1]) {
// 		bst->root = node_list;

// 		return bst;
// 	}

// 	typedef struct {
// 		bst_node_t *node_1;
// 		bst_node_t *node_2;
// 		uint32_t   number;
// 	} bst_traverse_node_t;

// 	bst_traverse_node_t cursor, left, right;
// 	bst_node_t *temp, *root;
// 	uint32_t list_size = 0;

// 	for (temp = node_list; temp; temp = temp->edge[1])
// 		list_size++;

// 	stack_t *stack = stack_constructor();

// 	root = bst_new_node(NULL, NULL, NULL);
// 	if (!root) return NULL;

// 	cursor.node_1 = root;
// 	cursor.node_2 = NULL;
// 	cursor.number = list_size;

// 	stack_push(stack, &cursor);

// 	while (!stack_is_empty(stack)) {
// 		cursor = *(bst_traverse_node_t *) stack_pop(stack);

// 		if (cursor.number > 1) {
// 			left.node_1 = bst_new_node(NULL, NULL, NULL);
// 			left.node_2 = cursor.node_2;
// 			left.number = cursor.number / 2;

// 			right.node_1 = bst_new_node(NULL, NULL, NULL);
// 			right.node_2 = cursor.node_1;
// 			right.number = cursor.number - left.number;

// 			(cursor.node_1)->edge[0] = left.node_1;
// 			(cursor.node_1)->edge[1] = right.node_1;

// 			stack_push(stack, &right);
// 			stack_push(stack, &left);
// 		} else {
// 			(cursor.node_1)->edge[0] = node_list->edge[0];
// 			(cursor.node_1)->key     = node_list->key;
// 			(cursor.node_1)->edge[1] = NULL;

// 			if (cursor.node_2)
// 				(cursor.node_2)->key = node_list->key;

// 			temp = node_list;

// 			node_list = node_list->edge[1];

// 			free_node(temp);
// 		}
// 	}

// 	bst->root = root;

// 	stack_destructor(stack);

// 	return bst;
// }

bst_t *bst_constructor_list_bottom_up(
	bst_node_t *node_list,
	key_destroy_fn_t key_destroy_fn,
	val_destroy_fn_t val_destroy_fn,
	key_length_fn_t key_length_fn,
	val_length_fn_t val_length_fn)
{
	/* Key length function is mandatory for a bst. */
	if (!key_length_fn) return NULL;

	bst_t *bst = malloc(sizeof(*bst));
	if (!bst) return NULL;

	bst->key_destroy_fn = key_destroy_fn;
	bst->val_destroy_fn = val_destroy_fn;
	bst->key_length_fn  = key_length_fn;
	bst->val_length_fn  = val_length_fn;

	if (!node_list) {
		bst->root = bst_new_node(NULL, NULL, NULL);
		if (!bst->root) return NULL;

		return bst;
	}

	if (!node_list->edge[1]) {
		bst->root = node_list;

		return bst;
	}

	bst_node_t *root, *end;

	root = end = bst_new_node(NULL, NULL, NULL);

	end->edge[0] = node_list;
	end->key     = node_list->key;

	node_list = node_list->edge[1];

	((bst_node_t *) end->edge[0])->edge[1] = NULL;

	while (node_list) {
		end->edge[1] = bst_new_node(node_list->key, node_list, NULL);
		end = end->edge[1];

		node_list = node_list->edge[1];
		((bst_node_t *) end->edge[0])->edge[1] = NULL;
	}

	end->edge[1] = NULL;

	bst_node_t *old_list, *new_list, *sub_1, *sub_2;

	old_list = root;

	while (old_list->edge[1]) {
		sub_1 = old_list;
		sub_2 = old_list->edge[1];
		old_list = ((bst_node_t *) old_list->edge[1])->edge[1];

		sub_2->edge[1] = sub_2->edge[0];
		sub_2->edge[0] = sub_1->edge[0];

		sub_1->edge[0] = sub_2;
		sub_1->edge[1] = NULL;

		new_list = end = sub_1;

		while (old_list) {
			if (!old_list->edge[1]) {
				end->edge[1] = old_list;
				old_list = NULL;
			} else {
				sub_1 = old_list;
				sub_2 = old_list->edge[1];

				old_list = ((bst_node_t *) old_list->edge[1])->edge[1];

				sub_2->edge[1] = sub_2->edge[0];
				sub_2->edge[0] = sub_1->edge[0];

				sub_1->edge[0] = sub_2;
				sub_1->edge[1] = NULL;

				end->edge[1] = sub_1;
				end = end->edge[1];
			}
		}

		old_list = new_list;
	}
	root = old_list->edge[0];
	bst->root = root;
	free_node(old_list);

	return bst;
}

void bst_destructor(bst_t *bst) {
	if (!bst) return;

	if (!bst->root) {
		goto free_bst;
	}

	if (!bst->root->edge[0]) {
		free(bst->root);
		bst->root = NULL;
		goto free_bst;
	}

	bst_traverse_post_order(bst->root, free_node);

free_bst:
	free(bst);
	bst = NULL;
}

bst_node_t *bst_flatten(bst_t *bst) {
	if (!bst) return NULL;

	if (!bst->root->edge[1] || !bst->root->edge[0])
		return bst->root;

	bst_node_t *list, *node;

	stack_t *stack = stack_constructor();

	stack_push(stack, bst->root);
	list = NULL;

	while (!stack_is_empty(stack)) {
		node = stack_pop(stack);

		if (!node->edge[1]) {
			node->edge[1] = list;

			list = node;
		} else {
			stack_push(stack, node->edge[0]);
			stack_push(stack, node->edge[1]);

			free_node(node);
		}
	}

	stack_destructor(stack);

	free(bst);
	bst = NULL;

	return list;
}

void *bst_find(bst_t *bst, void *key) {
	if (!bst) return NULL;

	bst_node_t *cursor = bst->root;
	if (!cursor->edge[0]) return NULL;

	unsigned char keys_are_same_size;
	int	      keys_difference;

	while (cursor->edge[1]) {
		keys_are_same_size = bst->key_length_fn(key) == bst->key_length_fn(cursor->key);
		if (!keys_are_same_size) return NULL;

		keys_difference	= memcmp(key, cursor->key, bst->key_length_fn(key));

		cursor = keys_are_same_size && keys_difference < 0
			 ? cursor->edge[0]
			 : cursor->edge[1];
	}

	keys_are_same_size = bst->key_length_fn(key) == bst->key_length_fn(cursor->key);
	if (!keys_are_same_size) return NULL;

	keys_difference	= memcmp(key, cursor->key, bst->key_length_fn(key));

	return keys_are_same_size && keys_difference == 0
	       ? (void *) cursor->edge[0]
	       : NULL;
}

bst_node_t *bst_find_interval(bst_t *bst, void *key_a, void *key_b) {
	if (!bst) return NULL;

	stack_t *stack = stack_constructor();
	if (!stack) return NULL;

	bst_node_t   *results, *cursor, *temp;
	unsigned char keys_are_same_size;
	int	      keys_difference_a;
	int	      keys_difference_b;

	results = NULL;

	stack_push(stack, bst->root);

	while (!stack_is_empty(stack)) {
		cursor = stack_pop(stack);
		keys_are_same_size = (bst->key_length_fn(key_a) == bst->key_length_fn(key_b)) && bst->key_length_fn(key_b) == bst->key_length_fn(cursor->key);
		if (!keys_are_same_size) return NULL;

		keys_difference_a = memcmp(key_a, cursor->key, bst->key_length_fn(key_a));
		keys_difference_b = memcmp(key_b, cursor->key, bst->key_length_fn(key_b));

		if (!cursor->edge[1]) {
			if (keys_difference_a <= 0 && keys_difference_b > 0) {
				temp = bst_new_node(cursor->key, cursor->edge[0], results);
				results = temp;
			}
		} else if (keys_difference_b <= 0) {
			stack_push(stack, cursor->edge[0]);
		} else if (keys_difference_a >= 0) {
			stack_push(stack, cursor->edge[1]);
		} else {
			stack_push(stack, cursor->edge[0]);
			stack_push(stack, cursor->edge[1]);
		}
	}

	stack_destructor(stack);

	return results;
}

void bst_insert(bst_t *bst, void *key, void *val) {
	if (!bst) return;
	if (!val) return;
	bst_node_t *cursor = bst->root;

	if (!cursor->edge[0]) {
		cursor->edge[1] = NULL;
		cursor->edge[0] = val;
		cursor->key = key;
		return;
	}

	unsigned char keys_are_same_size;
	int	      keys_difference;

	while (cursor->edge[1]) {
		keys_are_same_size = bst->key_length_fn(key) == bst->key_length_fn(cursor->key);
		if (!keys_are_same_size) return;

		keys_difference	= memcmp(key, cursor->key, bst->key_length_fn(key));

		cursor = keys_are_same_size && keys_difference < 0
			 ? cursor->edge[0]
			 : cursor->edge[1];
	}

	keys_are_same_size = bst->key_length_fn(key) == bst->key_length_fn(cursor->key);
	if (!keys_are_same_size) return;

	keys_difference = memcmp(cursor->key, key, bst->key_length_fn(key));

	if (keys_difference == 0) return;

	bst_node_t *old_leaf = bst_new_node(cursor->key, cursor->edge[0], NULL);

	bst_node_t *new_leaf = bst_new_node(key, val, NULL);

	if (keys_difference < 0) {
		cursor->edge[0] = old_leaf;
		cursor->edge[1] = new_leaf;
		cursor->key	= key;
		return;
	}

	cursor->edge[0] = new_leaf;
	cursor->edge[1] = old_leaf;
}

void bst_delete(bst_t *bst, void *key) {
	bst_node_t *cursor, *trailing, *sibling;
	unsigned char keys_are_same_size;
	int	      keys_difference;

	cursor = bst->root;

	if (!cursor->edge[0]) return;

	if (!cursor->edge[1]) {
		keys_are_same_size = bst->key_length_fn(key) == bst->key_length_fn(cursor->key);
		if (!keys_are_same_size) return;
		keys_difference = memcmp(key, cursor->key, bst->key_length_fn(key));

		if (keys_difference == 0) {
			cursor->edge[0] = NULL;
		}
		return;
	}

	while (cursor->edge[1]) {
		trailing = cursor;

		keys_are_same_size = bst->key_length_fn(key) == bst->key_length_fn(cursor->key);
		if (!keys_are_same_size) return;
		keys_difference = memcmp(key, cursor->key, bst->key_length_fn(key));

		if (keys_difference < 0) {
			cursor = trailing->edge[0];
			sibling = trailing->edge[1];
		} else {
			cursor = trailing->edge[1];
			sibling = trailing->edge[0];
		}
	}

	if (keys_difference != 0) return;

	trailing->key	  = sibling->key;
	trailing->edge[0] = sibling->edge[0];
	trailing->edge[1] = sibling->edge[1];

	free(cursor);
	free(sibling);
}

void bst_traverse(bst_t *bst, unsigned char mode, void (*fn)(bst_node_t *node)) {
	if (!bst->root->edge[0]) return;

	switch (mode) {
	case BST_TRAVERSE_PRE_ORDER:
		bst_traverse_pre_order(bst->root, fn);
		break;

	case BST_TRAVERSE_IN_ORDER:
		bst_traverse_in_order(bst->root, fn);
		break;

	case BST_TRAVERSE_POST_ORDER:
		bst_traverse_post_order(bst->root, fn);
		break;

	case BST_TRAVERSE_BREADTH_FIRST:
		bst_traverse_breadth_first(bst->root, fn);
		break;

	default:
		break;
	}
}

static void bst_traverse_pre_order(bst_node_t *root, void (*fn)(bst_node_t *node)) {
	stack_t *stack = stack_constructor();
	if (!stack) return;

	bst_node_t *cursor;

	stack_push(stack, root);
	while (!stack_is_empty(stack)) {
		cursor = stack_pop(stack);
		fn(cursor);
		if (cursor->edge[1])
			stack_push(stack, cursor->edge[1]);
		if (cursor->edge[0] && cursor->edge[1])
			stack_push(stack, cursor->edge[0]);
	}

	stack_destructor(stack);
}

static void bst_traverse_in_order(bst_node_t *root, void (*fn)(bst_node_t *node)) {
	stack_t *stack = stack_constructor();
	if (!stack) return;

	bst_node_t *cursor = root;

	while (!stack_is_empty(stack) || cursor) {
		if (cursor) {
			stack_push(stack, cursor);
			cursor = cursor->edge[0] && cursor->edge[1] ? cursor->edge[0] : NULL;
		} else {
			cursor = stack_pop(stack);
			fn(cursor);
			cursor = cursor->edge[1];
		}
	}

	stack_destructor(stack);
}

static void bst_traverse_post_order(bst_node_t *root, void (*fn)(bst_node_t *node)) {
	stack_t *stack = stack_constructor();
	if (!stack) return;

	bst_node_t *cursor = root;
	bst_node_t *last;
	bst_node_t *peek;

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

static void bst_traverse_breadth_first(bst_node_t *root, void (*fn)(bst_node_t *node)) {
	queue_t *q = queue_constructor();
	if (!q) return;

	bst_node_t *node;

	queue_enqueue(q, root);

	while (!queue_is_empty(q)) {
		node = queue_dequeue(q);
		fn(node);
		if (node->edge[0] && node->edge[1])
			queue_enqueue(q, node->edge[0]);
		if (node->edge[1])
			queue_enqueue(q, node->edge[1]);
	}

	queue_destructor(q);
}

/*
 * function do write int keys to the file in the dot lang format,
 * to display it later with grapgviz
 */
void bst_draw(bst_t *tree, FILE *stream) {
	fprintf(stream, "digraph bst {\n");
	fprintf(stream, "	node [fontname=\"Monospace\"];\n");

	if (!tree->root)
		fprintf(stream, "\n");
	else if (!tree->root->edge[1])
		fprintf(stream, "	%d;\n", *(int *) tree->root->key);
	else
		bst_draw_r(tree->root, stream);

	fprintf(stream, "}\n");
}

static void bst_draw_r(bst_node_t *cursor, FILE *stream) {
	if (!cursor->edge[1]) {
		fprintf(stream, "	%d [shape=box];\n", *(int *) cursor->edge[0]);
		fprintf(stream, "	%d -> %d;\n", *(int *) cursor->key, *(int *) cursor->edge[0]);
		return;
	}

	fprintf(stream, "	%d -> %d;\n", *(int *) cursor->key, *(int *)((bst_node_t *) cursor->edge[0])->key);
	bst_draw_r(cursor->edge[0], stream);

	fprintf(stream, "	%d -> %d;\n", *(int *) cursor->key, *(int *)((bst_node_t *) cursor->edge[1])->key);
	bst_draw_r(cursor->edge[1], stream);
}

void bst_traverse_list(bst_node_t *cursor, void (*fn)(bst_node_t *node)) {
	bst_node_t *next;

	while (cursor) {
		next = cursor->edge[1];
		fn(cursor);
		cursor = next;
	}
}

void *bst_get_node_key(bst_node_t *node) {
	return node->key;
}

bst_node_t *bst_new_node(void *key, void *left, void *right) {
	bst_node_t *node = malloc(sizeof(bst_node_t));
	if (!node) return NULL;

	node->key = key;
	node->edge[0] = left;
	node->edge[1] = right;

	return node;
}

static void free_node(bst_node_t *node) {
	free(node);
	node = NULL;
}