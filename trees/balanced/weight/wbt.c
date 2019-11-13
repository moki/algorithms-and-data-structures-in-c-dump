#include "wbt.h"

static void wbt_traverse_post_order(wbt_node_t *root, void (*fn)(wbt_node_t *node));
static int compare(void *a, void *b, size_t (*len_fn)(const void *a));
static void wbt_rotation(wbt_node_t *node, unsigned char dir);
static void wbt_draw_r(wbt_node_t *cursor, FILE *stream);
static void wbt_rebalance(wbt_t *wbt, stack_t *path);
static wbt_node_t *cast_void_to_node_t(void *node);
static void free_node(wbt_node_t *node);

struct wbt {
	wbt_node_t     *root;
	double      	alpha;
	double		epsilon;
	key_length_fn_t	key_length_fn;
};

struct wbt_node {
	uint32_t weight;
	void 	*edge[2];
	void 	*key;
};

wbt_t *wbt_constructor(key_length_fn_t key_length_fn, void *alpha, void *epsilon) {
	if (!key_length_fn) return NULL;

	wbt_t *wbt = malloc(sizeof(*wbt));
	if (!wbt) return NULL;

	wbt->root = wbt_new_node(NULL, NULL, NULL, 0);
	if (!wbt->root) {
		free(wbt);
		wbt = NULL;
		return NULL;
	}

	if (!alpha || !epsilon) {
		wbt->epsilon = WBT_EPSILON;
		wbt->alpha   = WBT_ALPHA;
	} else {
		wbt->alpha = *(double *) alpha;
		wbt->alpha = *(double *) epsilon;
	}

	wbt->key_length_fn = key_length_fn;

	return wbt;
}

void wbt_destructor(wbt_t *wbt) {
	if (!wbt) return;

	if (!wbt->root) {
		goto free_wbt;
	}

	if (!wbt->root->edge[0]) {
		free_node(wbt->root);
		goto free_wbt;
	}

	wbt_traverse_post_order(wbt->root, free_node);

free_wbt:
	free(wbt);
	wbt = NULL;
}

void *wbt_find(wbt_t *wbt, void *key) {
	if (!wbt->root->edge[0])
		return NULL;

	wbt_node_t *cursor = wbt->root;
	int keys_difference;

	while (cursor->edge[1]) {
		keys_difference = compare(key, cursor->key, wbt->key_length_fn);

		cursor = keys_difference < 0 ? cursor->edge[0] : cursor->edge[1];
	}

	keys_difference = compare(key, cursor->key, wbt->key_length_fn);

	return keys_difference == 0 ? cursor->edge[0] : NULL;
}

void wbt_insert(wbt_t *wbt, void *key, void *val) {
	if (!wbt) return;

	if (!wbt->root->edge[0]) {
		wbt->root->edge[0] = val;
		wbt->root->key 	   = key;
		wbt->root->weight  = 1;
		return;
	}

	stack_t *path = stack_constructor();
	wbt_node_t *cursor = wbt->root;
	int keys_difference;

	while (cursor->edge[1]) {
		stack_push(path, cursor);

		keys_difference = compare(key, cursor->key, wbt->key_length_fn);

		cursor = keys_difference < 0 ? cursor->edge[0] : cursor->edge[1];
	}

	keys_difference = compare(key, cursor->key, wbt->key_length_fn);

	if (keys_difference == 0)
		return;

	/* insert */

	wbt_node_t *old_leaf, *new_leaf;

	old_leaf = wbt_new_node(cursor->key, cursor->edge[0], NULL, 1);

	new_leaf = wbt_new_node(key, val, NULL, 1);

	if (keys_difference > 0) {
		cursor->edge[0] = old_leaf;
		cursor->edge[1] = new_leaf;
		cursor->key 	= key;
	} else {
		cursor->edge[0] = new_leaf;
		cursor->edge[1] = old_leaf;
	}

	cursor->weight = 2;

	/* rebalance */

	wbt_rebalance(wbt, path);
	stack_destructor(path);
}

void wbt_delete(wbt_t *wbt, void *key) {
	if (!wbt) return;

	if (!wbt->root->edge[1]) {
		if (compare(wbt->root->key, key, wbt->key_length_fn) == 0) {
			wbt->root->key = NULL;
			wbt->root->edge[0] = NULL;
			wbt->root->weight = 0;
		}

		return;
	}

	stack_t *path = stack_constructor();
	wbt_node_t *cursor, *parent, *sibling;
	cursor = wbt->root;
	int keys_difference;

	while (cursor->edge[1]) {
		stack_push(path, cursor);
		parent = cursor;

		keys_difference = compare(key, cursor->key, wbt->key_length_fn);

		cursor = parent->edge[keys_difference >= 0];
		sibling = parent->edge[keys_difference < 0];
	}

	if (compare(cursor->key, key, wbt->key_length_fn) != 0)
		goto clean;

	parent->key     = sibling->key;
	parent->edge[0] = sibling->edge[0];
	parent->edge[1] = sibling->edge[1];
	parent->weight  = sibling->weight;

	free_node(cursor);

	free_node(sibling);

	(void) stack_pop(path);
	wbt_rebalance(wbt, path);
clean:
	stack_destructor(path);
}

static void wbt_traverse_post_order(wbt_node_t *root, void (*fn)(wbt_node_t *node)) {
	stack_t *stack = stack_constructor();
	if (!stack) return;

	wbt_node_t *cursor = root;
	wbt_node_t *last;
	wbt_node_t *peek;

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

static void wbt_rebalance(wbt_t *wbt, stack_t *path) {
	wbt_node_t *cursor;

	while (!stack_is_empty(path)) {
		unsigned char dir;

		cursor 	   = stack_pop(path);
		cursor->weight = cast_void_to_node_t(cursor->edge[0])->weight + cast_void_to_node_t(cursor->edge[1])->weight;

		if (cast_void_to_node_t(cursor->edge[1])->weight < wbt->alpha * cursor->weight)
			dir = 0;
		else if (cast_void_to_node_t(cursor->edge[0])->weight < wbt->alpha * cursor->weight)
			dir = 1;
		else
			continue;

		if (cast_void_to_node_t(cast_void_to_node_t(cursor->edge[dir])->edge[dir])->weight >
			(wbt->alpha + wbt->epsilon) * cursor->weight) {
				wbt_rotation(cursor, !dir);
				cast_void_to_node_t(cursor->edge[!dir])->weight =
					cast_void_to_node_t(cast_void_to_node_t(cursor->edge[!dir])->edge[0])->weight +
					cast_void_to_node_t(cast_void_to_node_t(cursor->edge[!dir])->edge[1])->weight;
		} else {
			wbt_rotation(cursor->edge[dir], dir);
			wbt_rotation(cursor, !dir);

			cast_void_to_node_t(cursor->edge[1])->weight =
				cast_void_to_node_t(cast_void_to_node_t(cursor->edge[1])->edge[0])->weight +
				cast_void_to_node_t(cast_void_to_node_t(cursor->edge[1])->edge[1])->weight;

			cast_void_to_node_t(cursor->edge[0])->weight =
				cast_void_to_node_t(cast_void_to_node_t(cursor->edge[0])->edge[0])->weight +
				cast_void_to_node_t(cast_void_to_node_t(cursor->edge[0])->edge[1])->weight;
		}
	}
}

static void wbt_rotation(wbt_node_t *node, unsigned char dir) {
	wbt_node_t *t_n = node->edge[dir];
	void	   *t_k = node->key;

	node->edge[dir]  = node->edge[!dir];
	node->key	 = ((wbt_node_t *) node->edge[!dir])->key;
	node->edge[!dir] = ((wbt_node_t *) node->edge[dir])->edge[!dir];
	((wbt_node_t *)node->edge[dir])->edge[!dir] = ((wbt_node_t *) node->edge[dir])->edge[dir];


	((wbt_node_t *)node->edge[dir])->edge[dir] = t_n;
	((wbt_node_t *)node->edge[dir])->key       = t_k;
}

void wbt_draw(wbt_t *tree, FILE *stream) {
	fprintf(stream, "digraph hbt {\n");
	fprintf(stream, "	node [fontname=\"Monospace\"];\n");

	if (!tree->root)
		fprintf(stream, "\n");
	else if (!tree->root->edge[1])
		fprintf(stream, "	%d;\n", *(int *) tree->root->key);
	else
		wbt_draw_r(tree->root, stream);

	fprintf(stream, "}\n");
}

static void wbt_draw_r(wbt_node_t *cursor, FILE *stream) {
	if (!cursor->edge[1]) {
		fprintf(stream, "	%d [shape=box];\n", *(int *) cursor->edge[0]);
		fprintf(stream, "	%d -> %d;\n", *(int *) cursor->key, *(int *) cursor->edge[0]);
		return;
	}

	fprintf(stream, "	%d -> %d;\n", *(int *) cursor->key, *(int *)((wbt_node_t *) cursor->edge[0])->key);
	wbt_draw_r(cursor->edge[0], stream);

	fprintf(stream, "	%d -> %d;\n", *(int *) cursor->key, *(int *)((wbt_node_t *) cursor->edge[1])->key);
	wbt_draw_r(cursor->edge[1], stream);
}

void *wbt_get_node_key(wbt_node_t *node) {
	return node->key;
}

wbt_node_t *wbt_new_node(void *key, void *left, void *right, uint32_t weight) {
	wbt_node_t *new_node = malloc(sizeof(*new_node));

	new_node->weight  = weight;
	new_node->edge[1] = right;
	new_node->edge[0] = left;
	new_node->key 	  = key;

	return new_node;
}

static wbt_node_t *cast_void_to_node_t(void *node) {
	return (wbt_node_t *) node;
}

static int compare(void *a, void *b, size_t (*len_fn)(const void *a)) {
	unsigned char keys_are_same_size;

	keys_are_same_size = len_fn(a) == len_fn(b);
	if (!keys_are_same_size) exit(EXIT_FAILURE);

	return memcmp(a, b, len_fn(a));
}

static void free_node(wbt_node_t *node) {
	free(node);
	node = NULL;
}