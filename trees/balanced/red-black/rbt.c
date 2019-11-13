#include "rbt.h"

static void 	   rbt_traverse_post_order (rbt_t *tree, void (*ts_fn)(rbt_t *tree, rbt_node_t *node));
static void 	   rbt_insert_rebalance_bottom_up(rbt_t *tree, void *key, void *val);
static void 	   rbt_insert_rebalance_top_down(rbt_t *tree, void *key, void *val);
static void 	   rbt_delete_rebalance_bottom_up(rbt_t *tree, void *key);
static void 	   rbt_delete_rebalance_top_down(rbt_t *tree, void *key);
static void 	   rbt_rotation(rbt_node_t *node, unsigned char dir);
static void 	   dp_free(void (*free_fn)(void *_ptr), void *ptr);
static int 	   key_cmp(void *k_a, void *k_b, size_t k_s);
static void	   node_free(rbt_t *tree, rbt_node_t *node);
static rbt_node_t *vp_np(void *node);

struct rbt {
	rbt_node_t *root;
	key_destroy_fn_t key_destroy_fn;
	val_destroy_fn_t val_destroy_fn;
	unsigned char 	 rebalance_mode;
	size_t k_s;
	size_t v_s;
};

struct rbt_node {
	void *key;
	void *edge[2];
	unsigned char color : 1;
};

rbt_t *rbt_constructor(size_t k_s, size_t v_s, void *key_destroy_fn, void *val_destroy_fn, unsigned char rebalance_mode) {
	rbt_t *tree = malloc(sizeof(*tree));
	if (!tree) return NULL;

	tree->root = NULL;

	tree->k_s = k_s;
	tree->v_s = v_s;

	tree->key_destroy_fn = (key_destroy_fn_t) key_destroy_fn;
	tree->val_destroy_fn = (val_destroy_fn_t) val_destroy_fn;

	tree->rebalance_mode = rebalance_mode;

	return tree;
}

void rbt_destructor(rbt_t *tree) {
	if (!tree)
		return;

	rbt_traverse(tree, RBT_TRAVERSE_POST_ORDER, node_free);

	dp_free(free, tree);
}

void *rbt_find(rbt_t *tree, void *key) {
	rbt_node_t *cursor = tree->root;

	if (!cursor)
		return NULL;

	while (cursor->edge[1]) {
		cursor = key_cmp(key, cursor->key, tree->k_s) < 0 ? cursor->edge[0] : cursor->edge[1];
	}

	return !key_cmp(key, cursor->key, tree->k_s) ? cursor->edge[0] : NULL;
}

void rbt_insert(rbt_t *tree, void *key, void *val) {
	switch (tree->rebalance_mode) {
	case RBT_REBALANCE_MODE_BOTTOM_UP:
		rbt_insert_rebalance_bottom_up(tree, key, val);
		break;

	case RBT_REBALANCE_MODE_TOP_DOWN:
		rbt_insert_rebalance_top_down(tree, key, val);
		break;

	default:
		break;

	}
}

void rbt_delete(rbt_t *tree, void *key) {
	switch(tree->rebalance_mode) {
	case RBT_REBALANCE_MODE_BOTTOM_UP:
		rbt_delete_rebalance_bottom_up(tree, key);
		break;

	case RBT_REBALANCE_MODE_TOP_DOWN:
		rbt_delete_rebalance_top_down(tree, key);
		break;

	default:
		break;
	}
}

static void rbt_insert_rebalance_bottom_up(rbt_t *tree, void *key, void *val) {
	rbt_node_t *cursor = tree->root;

	/* empty tree case */
	if (!cursor) {
		cursor = malloc(sizeof(*tree->root));

		cursor->edge[0] = val;
		cursor->edge[1] = NULL;
		cursor->key     = key;
		cursor->color   = 0;

		tree->root = cursor;

		return;
	}

	stack_t *path = stack_constructor(NULL);

	while (cursor->edge[1]) {
		stack_push(path, cursor);

		cursor = key_cmp(key, cursor->key, tree->k_s) < 0 ? cursor->edge[0] : cursor->edge[1];
	}

	int key_diff = key_cmp(key, cursor->key, tree->k_s);

	/* if duplicate */
	if (key_diff == 0) {
		stack_destructor(path);
		return;
	}

	/* insert */
	rbt_node_t *old_leaf = malloc(sizeof(*old_leaf));
	rbt_node_t *new_leaf = malloc(sizeof(*new_leaf));

	old_leaf->key     = cursor->key;
	old_leaf->edge[0] = cursor->edge[0];
	old_leaf->edge[1] = NULL;
	old_leaf->color   = 1;

	new_leaf->key     = key;
	new_leaf->edge[0] = val;
	new_leaf->edge[1] = NULL;
	new_leaf->color   = 1;

	cursor->key 	= key_diff > 0 ? key 	  : cursor->key;
	cursor->edge[0] = key_diff > 0 ? old_leaf : new_leaf;
	cursor->edge[1] = key_diff > 0 ? new_leaf : old_leaf;

	/* rebalance */
	unsigned char finished = !cursor->color || cursor == tree->root;
	unsigned char dir      = 0;

	while (!stack_is_empty(path) && !finished) {
		rbt_node_t *parent = stack_pop(path);

		/* both red, with black parent */
		if (!(vp_np(parent->edge[0])->color ^ vp_np(parent->edge[1])->color)) {
			vp_np(parent->edge[0])->color ^= 0;
			vp_np(parent->edge[1])->color ^= 0;
			parent->color ^= 1;
		} else {
			dir = cursor == parent->edge[0];

			if (!vp_np(cursor->edge[dir])->color) {
				rbt_rotation(parent, dir);
				vp_np(parent->edge[dir])->color = 1;
				parent->color = 0;
				finished = 1;
			} else {
				rbt_rotation(cursor, !dir);
				rbt_rotation(parent, dir);
				vp_np(parent->edge[1])->color = 0;
				vp_np(parent->edge[0])->color = 0;
				parent->color = 1;
			}

			cursor = parent;
		}

		if (!finished && !stack_is_empty(path)) {
			cursor = stack_pop(path);

			if (!cursor->color)
				finished = 1;
		}
	}

	tree->root->color = 0;

	stack_destructor(path);
}

static void rbt_insert_rebalance_top_down(rbt_t *tree, void *key, void *val) {
	rbt_node_t *cursor = tree->root;

	/* empty tree case */
	if (!cursor) {
		cursor = malloc(sizeof(*tree->root));

		cursor->edge[0] = val;
		cursor->edge[1] = NULL;
		cursor->key     = key;
		cursor->color   = 0;

		tree->root = cursor;

		return;
	}

	rbt_node_t *future_cursor, *parent;
	int key_diff;
	parent = NULL;

	/* move down & balance */
	while (cursor->edge[1]) {
		key_diff = key_cmp(key, cursor->key, tree->k_s);

		future_cursor = key_diff < 0 ? cursor->edge[0] : cursor->edge[1];

		if (!cursor->color) {
			if (!vp_np(cursor->edge[0])->color | !vp_np(cursor->edge[0])->color) {
				parent = cursor;
				cursor = future_cursor;
			} else {
				if (!parent) {
					vp_np(cursor->edge[0])->color = 0;
					vp_np(cursor->edge[1])->color = 0;

					parent = cursor;
				} else {
					key_diff = key_cmp(cursor->key, parent->key, tree->k_s);
					unsigned char dir = key_diff >= 0;

					if (cursor == parent->edge[dir]) {
						vp_np(cursor->edge[0])->color = 0;
						vp_np(cursor->edge[1])->color = 0;
						cursor->color = 1;
					} else if (cursor == vp_np(parent->edge[dir])->edge[dir]) {
						rbt_rotation(parent, !dir);

						vp_np(parent->edge[0])->color = 1;
						vp_np(parent->edge[1])->color = 1;
						vp_np(vp_np(parent->edge[dir])->edge[0])->color = 0;
						vp_np(vp_np(parent->edge[dir])->edge[1])->color = 0;
					} else {
						rbt_rotation(parent->edge[dir], dir);
						rbt_rotation(parent, !dir);

						vp_np(parent->edge[0])->color = 1;
						vp_np(parent->edge[1])->color = 1;
						vp_np(vp_np(parent->edge[1])->edge[0])->color = 0;
						vp_np(vp_np(parent->edge[0])->edge[1])->color = 0;
					}
				}

				cursor = future_cursor;
				parent = cursor;
			}
		} else {
			cursor = future_cursor;
		}
	}
	key_diff = key_cmp(key, cursor->key, tree->k_s);

	/* if duplicate */
	if (key_diff == 0)
		return;

	/* insert */
	rbt_node_t *old_leaf = malloc(sizeof(*old_leaf));
	rbt_node_t *new_leaf = malloc(sizeof(*new_leaf));

	old_leaf->key     = cursor->key;
	old_leaf->edge[0] = cursor->edge[0];
	old_leaf->edge[1] = NULL;
	old_leaf->color   = 1;

	new_leaf->key     = key;
	new_leaf->edge[0] = val;
	new_leaf->edge[1] = NULL;
	new_leaf->color   = 1;

	cursor->key 	= key_diff > 0 ? key 	  : cursor->key;
	cursor->edge[0] = key_diff > 0 ? old_leaf : new_leaf;
	cursor->edge[1] = key_diff > 0 ? new_leaf : old_leaf;
}

static void rbt_delete_rebalance_bottom_up(rbt_t *tree, void *key) {
	rbt_node_t *cursor = tree->root;
	unsigned char key_diff;

	if (!cursor) return;

	/* delete from root */
	if (!cursor->edge[1]) {
		key_diff = key_cmp(cursor->key, key, tree->k_s);

		if (key_diff == 0) {
			node_free(tree, cursor);
			tree->root = NULL;
		}

		return;
	}

	stack_t *path = stack_constructor(NULL);
	rbt_node_t *parent, *sibling;
	unsigned char finished = 0;

	while (cursor->edge[1]) {
		stack_push(path, cursor);

		parent	 = cursor;
		key_diff = key_cmp(key, cursor->key, tree->k_s);

		cursor  = parent->edge[key_diff >= 0];
		sibling = parent->edge[key_diff < 0];
	}

	if (key_cmp(key, cursor->key, tree->k_s) != 0) {
		stack_destructor(path);
		return;
	}

	parent->key = sibling->key;
	parent->edge[0] = sibling->edge[0];
	parent->edge[1] = sibling->edge[1];

	if (parent->color ^ sibling->color)
		finished = 1;

	parent->color = 0;

	node_free(tree, cursor);

	node_free(tree, sibling);

	/* rebalance */
	while (!stack_is_empty(path) && !finished) {
		cursor = stack_pop(path);

		if (cursor->color || cursor == tree->root) {
			cursor->color = 0;
			break;
		}

		parent = stack_pop(path);

		unsigned char dir = cursor == parent->edge[0];

		sibling = parent->edge[dir];

		if (!parent->color) {
			if (!sibling->color) {
				if (!vp_np(sibling->edge[!dir])->color) {
					rbt_rotation(parent, !dir);

					vp_np(parent->edge[!dir])->color = 1;
					parent->color = 0;
				} else {
					rbt_rotation(sibling, dir);
					rbt_rotation(parent, !dir);

					vp_np(parent->edge[0])->color = 0;
					vp_np(parent->edge[1])->color = 0;
					parent->color = 0;

					finished = 1;
				}
			} else {
				if (!vp_np(vp_np(sibling->edge[!dir])->edge[!dir])->color) {
					rbt_rotation(parent, !dir);
					rbt_rotation(parent->edge[!dir], !dir);

					vp_np(vp_np(parent->edge[!dir])->edge[!dir])->color = !dir;
					vp_np(parent->edge[!dir])->color = 0;
					parent->color = 0;

					finished = 1;
				} else {
					rbt_rotation(parent, !dir);
					rbt_rotation(vp_np(parent->edge[!dir])->edge[dir], dir);
					rbt_rotation(parent->edge[!dir], !dir);

					vp_np(vp_np(parent->edge[!dir])->edge[!dir])->color = 0;
					vp_np(vp_np(parent->edge[!dir])->edge[dir])->color = 0;
					vp_np(parent->edge[!dir])->color = 1;
					parent->color = 0;

					finished = 1;
				}
			}
		} else {
			if (!vp_np(sibling->edge[!dir])->color) {
				rbt_rotation(parent, !dir);

				vp_np(parent->edge[!dir])->color = 1;
				parent->color = 0;

				finished = 1;
			} else {
				rbt_rotation(sibling, dir);
				rbt_rotation(parent, !dir);

				vp_np(parent->edge[0])->color = 0;
				vp_np(parent->edge[1])->color = 0;
				parent->color = 1;

				finished = 1;
			}
		}
	}

	stack_destructor(path);
}

static void rbt_delete_rebalance_top_down(rbt_t *tree, void *key) {
	if (!tree) return;
	if (!tree->root) return;

	rbt_node_t *cursor = tree->root;
	int key_diff;

	if (!cursor->edge[1]) {
		key_diff = key_cmp(cursor->key, key, tree->k_s);

		if (key_diff == 0) {
			node_free(tree, cursor);
			tree->root = NULL;
		}

		return;
	}

	rbt_node_t *parent = cursor;
	unsigned char dir;

	cursor->color = 0;

	if (!vp_np(parent->edge[0])->color & !vp_np(parent->edge[1])->color) {
		key_diff = key_cmp(key, parent->key, tree->k_s);
		dir = key_diff < 0;

		if (!vp_np(parent->edge[!dir])->edge[dir]) {
			if (!vp_np(parent->edge[dir])->edge[1]) {
				vp_np(parent->edge[0])->color = 1;
				vp_np(parent->edge[1])->color = 1;
			} else {
				vp_np(vp_np(parent->edge[dir])->edge[0])->color = 0;
				vp_np(vp_np(parent->edge[dir])->edge[1])->color = 0;
				vp_np(parent->edge[dir])->color = 1;
			}
		} else {
			if (vp_np(vp_np(parent->edge[!dir])->edge[0])->color |
				vp_np(vp_np(parent->edge[!dir])->edge[1])->color) {
				parent = parent->edge[!dir];
			} else if (vp_np(vp_np(parent->edge[dir])->edge[dir])->color) {
				rbt_rotation(parent, !dir);

				vp_np(parent->edge[1])->color = 0;
				vp_np(parent->edge[0])->color = 0;
				vp_np(vp_np(parent->edge[!dir])->edge[!dir])->color = 1;
				parent = parent->edge[!dir];
			} else if (vp_np(vp_np(parent->edge[dir])->edge[!dir])->color) {
				rbt_rotation(parent->edge[dir], dir);
				rbt_rotation(parent, !dir);

				vp_np(parent->edge[1])->color = 0;
				vp_np(parent->edge[0])->color = 0;
				vp_np(vp_np(parent->edge[!dir])->edge[!dir])->color = 1;
				parent = parent->edge[!dir];
			} else {
				vp_np(parent->edge[0])->color = 1;
				vp_np(parent->edge[1])->color = 1;
			}
		}

	}

	cursor = parent;

	while (cursor->edge[1]) {
		key_diff = key_cmp(key, cursor->key, tree->k_s);

		cursor = key_diff < 0 ? cursor->edge[0] : cursor->edge[1];

		if (cursor->color || !cursor->edge[1])
			continue;

		if (vp_np(cursor->edge[0])->color | vp_np(cursor->edge[1])->color) {
			parent = cursor;
			continue;
		}

		key_diff = key_cmp(cursor->key, parent->key, tree->k_s);
		dir = key_diff < 0; // 1

		if (cursor == parent->edge[!dir]) {
			if (!vp_np(vp_np(vp_np(parent->edge[dir])->edge[!dir])->edge[0])->color &
				!vp_np(vp_np(vp_np(parent->edge[dir])->edge[!dir])->edge[1])->color) {
				rbt_rotation(parent, !dir);

				vp_np(parent->edge[!dir])->color = 0;
				vp_np(vp_np(parent->edge[!dir])->edge[!dir])->color = 1;
				vp_np(vp_np(parent->edge[!dir])->edge[dir])->color = 1;

				cursor = parent = parent->edge[!dir];
			} else if (vp_np(vp_np(vp_np(parent->edge[dir])->edge[!dir])->edge[!dir])->color) {
				rbt_rotation(vp_np(parent->edge[dir])->edge[!dir], dir);
				rbt_rotation(parent->edge[dir], dir);
				rbt_rotation(parent, !dir);

				vp_np(parent->edge[!dir])->color = 0;
				vp_np(vp_np(parent->edge[dir])->edge[!dir])->color = 0;
				vp_np(parent->edge[dir])->color = 1;
				vp_np(vp_np(parent->edge[!dir])->edge[!dir])->color = 1;

				cursor = parent = parent->edge[!dir];
			} else {
				rbt_rotation(parent->edge[dir], dir);
				rbt_rotation(parent, !dir);

				vp_np(parent->edge[!dir])->color = 0;
				vp_np(vp_np(parent->edge[dir])->edge[!dir])->color = 0;
				vp_np(parent->edge[dir])->color = 1;
				vp_np(vp_np(parent->edge[!dir])->edge[!dir])->color = 1;

				cursor = parent = parent->edge[!dir];
			}
		} else if (cursor == vp_np(parent->edge[!dir])->edge[!dir]) {
			if (!vp_np(vp_np(vp_np(parent->edge[!dir])->edge[dir])->edge[0])->color &
				!vp_np(vp_np(vp_np(parent->edge[!dir])->edge[dir])->edge[1])->color) {
				vp_np(parent->edge[!dir])->color = 0;
				vp_np(vp_np(parent->edge[!dir])->edge[0])->color = 1;
				vp_np(vp_np(parent->edge[!dir])->edge[1])->color = 1;

				cursor = parent = parent->edge[!dir];
			} else if (vp_np(vp_np(parent->edge[!dir])->edge[dir])->edge[dir]) {
				rbt_rotation(parent->edge[!dir], !dir);

				vp_np(vp_np(parent->edge[!dir])->edge[0])->color = 0;
				vp_np(vp_np(parent->edge[!dir])->edge[1])->color = 0;
				vp_np(parent->edge[!dir])->color = 1;
				vp_np(vp_np(vp_np(parent->edge[!dir])->edge[!dir])->edge[!dir])->color = 1;

				cursor = parent = vp_np(parent->edge[!dir])->edge[!dir];
			} else {
				rbt_rotation(vp_np(parent->edge[!dir])->edge[dir], dir);
				rbt_rotation(parent->edge[!dir], !dir);

				vp_np(vp_np(parent->edge[!dir])->edge[0])->color = 0;
				vp_np(vp_np(parent->edge[!dir])->edge[1])->color = 0;
				vp_np(parent->edge[!dir])->color = 1;
				vp_np(vp_np(vp_np(parent->edge[!dir])->edge[!dir])->edge[!dir])->color = 1;

				cursor = parent = vp_np(parent->edge[!dir])->edge[!dir];
			}
		} else {
			if (!vp_np(vp_np(vp_np(parent->edge[!dir])->edge[!dir])->edge[0])->color &
				!vp_np(vp_np(vp_np(parent->edge[!dir])->edge[!dir])->edge[1])->color) {
				vp_np(parent->edge[!dir])->color = 0;
				vp_np(vp_np(parent->edge[!dir])->edge[0])->color = 1;
				vp_np(vp_np(parent->edge[!dir])->edge[1])->color = 1;

				cursor = parent = parent->edge[!dir];
			} else if (vp_np(vp_np(vp_np(parent->edge[!dir])->edge[!dir])->edge[!dir])->color) {
				rbt_rotation(parent->edge[!dir], dir);

				vp_np(vp_np(parent->edge[!dir])->edge[0])->color = 0;
				vp_np(vp_np(parent->edge[!dir])->edge[1])->color = 0;
				vp_np(parent->edge[!dir])->color = 1;
				vp_np(vp_np(vp_np(parent->edge[!dir])->edge[dir])->edge[dir])->color = 1;

				cursor = parent = vp_np(parent->edge[!dir])->edge[dir];
			} else {
				rbt_rotation(vp_np(parent->edge[!dir])->edge[!dir], !dir);
				rbt_rotation(parent->edge[!dir], dir);

				vp_np(vp_np(parent->edge[!dir])->edge[0])->color = 0;
				vp_np(vp_np(parent->edge[!dir])->edge[1])->color = 0;
				vp_np(parent->edge[!dir])->color = 1;
				vp_np(vp_np(vp_np(parent->edge[!dir])->edge[dir])->edge[dir])->color = 1;

				cursor = parent = vp_np(parent->edge[!dir])->edge[dir];
			}
		}
	}

	if (key_cmp(cursor->key, key, tree->k_s) != 0)
		return;

	rbt_node_t *doomed;

	key_diff = key_cmp(cursor->key, parent->key, tree->k_s);
	dir = key_diff < 0;

	if (cursor == parent->edge[!dir]) {
		doomed = parent->edge[dir];

		parent->key = doomed->key;
		parent->edge[0] = doomed->edge[0];
		parent->edge[1] = doomed->edge[1];
	} else if (cursor == vp_np(parent->edge[!dir])->edge[!dir]) {
		doomed = parent->edge[!dir];

		parent->edge[!dir] = doomed->edge[dir];
	} else {
		doomed = parent->edge[!dir];
		parent->edge[!dir] = doomed->edge[!dir];
	}

	node_free(tree, doomed);
	node_free(tree, cursor);
}

void rbt_traverse(rbt_t *tree, unsigned char mode, void (*ts_fn)(rbt_t *tree, rbt_node_t *node)) {
	if (!tree->root) return;

	switch (mode) {
	case RBT_TRAVERSE_POST_ORDER:
		rbt_traverse_post_order(tree, ts_fn);
		break;

	default:
		break;
	}
}

static void rbt_rotation(rbt_node_t *node, unsigned char dir) {
	rbt_node_t *t_n = node->edge[dir];
	void	   *t_k = node->key;

	node->edge[dir]  = node->edge[!dir];
	node->key	 = vp_np(node->edge[!dir])->key;
	node->edge[!dir] = vp_np(node->edge[dir])->edge[!dir];

	vp_np(node->edge[dir])->edge[!dir] = vp_np(node->edge[dir])->edge[dir];

	vp_np(node->edge[dir])->edge[dir] = t_n;
	vp_np(node->edge[dir])->key       = t_k;
}

static void rbt_traverse_post_order(rbt_t *tree, void (*ts_fn)(rbt_t *tree, rbt_node_t *node)) {
	stack_t *stack = stack_constructor(NULL);
	if (!stack) return;

	rbt_node_t *cursor = tree->root;
	rbt_node_t *last;
	rbt_node_t *peek;

	while (!stack_is_empty(stack) || cursor) {
		if (cursor) {
			stack_push(stack, cursor);
			cursor = cursor->edge[0] && cursor->edge[1] ? cursor->edge[0] : NULL;
		} else {
			peek = stack_peek(stack);

			if (peek->edge[1] && last != peek->edge[1])
				cursor = peek->edge[1];
			else {
				ts_fn(tree, peek);
				last = stack_pop(stack);
			}
		}
	}

	stack_destructor(stack);
}

static int key_cmp(void *k_a, void *k_b, size_t k_s) {
	return memcmp(k_a, k_b, k_s);
}

static rbt_node_t *vp_np(void *node) {
	return (rbt_node_t *) node;
}

void rbt_draw(rbt_t *tree, FILE *stream);

static void dp_free(void (*free_fn)(void *_ptr), void *ptr) {
	free_fn(ptr);
	ptr = NULL;
}

static void node_free(rbt_t *tree, rbt_node_t *node) {
	if (!tree) return;
	if (!node) return;

	if (tree->key_destroy_fn) dp_free(tree->key_destroy_fn, node->key);

	if (tree->val_destroy_fn) {
		if (!node->edge[1]) dp_free(tree->val_destroy_fn, node->edge[0]);
	}

	dp_free(free, node);
}
