#include "abt.h"

static void free_node(abt_node_t *node);
static void memswp(void *a, void *b, size_t size);

struct abt {
	abt_node_t *root;
	uint32_t    a;
	uint32_t    b;
	size_t	    k_s;
	size_t	    v_s;
};

struct abt_node {
	uint32_t  degree;
	uint32_t  height;
	void 	 *key;
	void	 *next;
};

abt_t *abt_constructor(size_t k_s, size_t v_s, uint32_t a, uint32_t b) {
	if (!k_s) return NULL;

	abt_t *tree = malloc(sizeof(*tree));
	if (!tree) return NULL;

	tree->root = malloc(sizeof(*tree->root));
	if (!tree->root) return NULL;

	tree->root->key = malloc(k_s * b);
	if (!tree->root->key) return NULL;

	tree->root->next = malloc(v_s * b);
	if (!tree->root->next) return NULL;

	tree->root->degree = 0;
	tree->root->height = 0;

	tree->a = a;
	tree->b = b;

	tree->k_s = k_s;
	tree->v_s = v_s;

	return tree;
}

void abt_destructor(abt_t *tree) {
	if (!tree) return;

	if (tree->root) {
		free_node(tree->root);
	}

	free(tree);
	tree = NULL;
}

void *abt_find(abt_t *tree, void *key) {
	// abt_node_t *cursor = malloc(sizeof(abt_node_t));
	// *cursor = *tree->root;
	abt_node_t *cursor = tree->root;

	void *t = malloc(sizeof(void *));

	while (cursor->height >= 0) {
		uint32_t right = cursor->degree;
		uint32_t left  = 0;
		int 	 keys_difference;
		uint32_t mid;

		while (right > left + 1) {
			mid = (right + left) / 2;

			keys_difference = memcmp(key, (unsigned char *) cursor->key + tree->k_s * mid, tree->k_s);

			if (keys_difference < 0)
				right = mid;
			else
				left = mid;
		}

		if (cursor->height > 0)
			// memcpy(cursor, (unsigned char *) cursor->next + sizeof(abt_node_t) * left, sizeof(abt_node_t));
			cursor = ((void *) ((unsigned char *) cursor->next + sizeof(abt_node_t) * left));
		else {
			if (memcmp(key, (unsigned char *) cursor->key + tree->k_s * left, tree->k_s) == 0) {
				memcpy(t, (unsigned char *) cursor->next + tree->v_s * left, tree->v_s);
			} else {
				t = NULL;
			}

			return t;
		}

	}

	// free_node(cursor);
}

void abt_insert(abt_t *tree, void *key, void *val) {
	if (!tree) return;
	abt_node_t *cursor = tree->root;

	if (tree->root->height == 0 && tree->root->degree == 0) {
		memcpy(tree->root->key, key, tree->k_s);
		memcpy(tree->root->next, val, tree->v_s);
		tree->root->degree = 1;
		return;
	}

	stack_t *path = stack_constructor(NULL);

	while (cursor->height > 0) {
		stack_push(path, cursor);

		uint32_t right = cursor->degree;
		uint32_t left  = 0;
		int 	 keys_difference;
		uint32_t mid;

		while (right > left + 1) {
			mid = (right + left) / 2;

			keys_difference = memcmp(key, (unsigned char *) cursor->key + tree->k_s * mid, tree->k_s);

			if (keys_difference < 0)
				right = mid;
			else
				left = mid;
		}

		cursor = ((void *) ((unsigned char *) cursor->next + sizeof(abt_node_t) * left));
	}

	unsigned char finished = 0;
	void *insert_pt  = val;
	void *insert_key = malloc(tree->k_s);
	memcpy(insert_key, key, tree->k_s);

	while (!finished) {
		int i, start;

		start = cursor->height > 0;

		if (cursor->degree < tree->b) {
			i = cursor->degree;

			while (i > start &&
				memcmp(insert_key, (unsigned char *) cursor->key + tree->k_s * (i - 1), tree->k_s ) < 0)
			{
				memswp((unsigned char *) cursor->key + i * tree->k_s,
				       (unsigned char *) cursor->key + (i - 1) * tree->k_s,
				       tree->k_s);

				memswp((unsigned char *) cursor->next + i * tree->v_s,
				       (unsigned char *) cursor->next + (i - 1) * tree->v_s,
				       tree->v_s);
				i--;
			}

			memcpy((unsigned char *) cursor->key + i * tree->k_s, insert_key, tree->k_s);

			memcpy((unsigned char *) cursor->next + i * (cursor->height ? sizeof(abt_node_t) : tree->v_s),
			       insert_pt,
			       cursor->height ? sizeof(abt_node_t) : tree->v_s);

			// if (cursor->height)
			// 	memcpy((unsigned char *) cursor->next + i * sizeof(abt_node_t), insert_pt, sizeof(abt_node_t));
			// else
			// 	memcpy((unsigned char *) cursor->next + i * tree->v_s, insert_pt, tree->v_s);

			cursor->degree++;
			finished = 1;

		} else {
			abt_node_t *new_node = malloc(sizeof(*new_node));
			if (!new_node) exit(EXIT_FAILURE);

			new_node->key = malloc(tree->k_s * tree->b);
			if (!new_node->key) exit(EXIT_FAILURE);

			new_node->next = malloc(tree->v_s * tree->b);
			if (!new_node->next) exit(EXIT_FAILURE);

			int j;
			unsigned char done = 0;

			i = tree->b - 1;
			j = (tree->b - 1) / 2;

			while (j >= 0) {
				if (done || memcmp(insert_key, (unsigned char *) cursor->key + tree->k_s * i, tree->k_s) < 0) {
					memcpy((unsigned char *) new_node->next + tree->v_s * j,
					       (unsigned char *) cursor->next + tree->v_s * i,
					       tree->v_s);

					memcpy((unsigned char *) new_node->key + tree->k_s * j--,
					       (unsigned char *) cursor->key + tree->k_s * i--,
					       tree->k_s);
				} else {
					memcpy((unsigned char *) new_node->next + tree->v_s * j,
					       insert_pt,
					       tree->v_s);

					memcpy((unsigned char *) new_node->key + tree->k_s * j--,
					       insert_key,
					       tree->k_s);

					done = 1;
				}
			}

			while (!done) {
				if (memcmp(insert_key, (unsigned char *) cursor->key + tree->k_s * i, tree->k_s) < 0 && i >= start) {
					memcpy((unsigned char *) cursor->next + tree->v_s * (i + 1),
					       (unsigned char *) cursor->next + tree->v_s * i,
					       tree->v_s);

					memcpy((unsigned char *) cursor->key + tree->k_s * (i + 1),
					       (unsigned char *) cursor->key + tree->k_s * i,
					       tree->k_s);

					i--;
				} else {
					memcpy((unsigned char *) cursor->next + tree->v_s * (i + 1),
					       insert_pt,
					       tree->v_s);

					memcpy((unsigned char *) cursor->key + tree->k_s * (i + 1),
					       insert_key,
					       tree->k_s);

					done = 1;
				}
			}

			cursor->degree = tree->b + 1 - ((tree->b + 1) / 2);
			new_node->degree = (tree->b + 1) / 2;
			new_node->height = cursor->height;

			insert_pt = new_node;
			memcpy(insert_key, new_node->key, tree->k_s);

			if (!stack_is_empty(path)) {
				cursor = stack_pop(path);
				continue;
			}

			abt_node_t *_new_node = malloc(sizeof(*_new_node));
			if (!_new_node) exit(EXIT_FAILURE);

			_new_node->key = malloc(tree->k_s * tree->b);
			if (!_new_node->key) exit(EXIT_FAILURE);

			_new_node->next = malloc(tree->v_s * tree->b);
			if (!_new_node->next) exit(EXIT_FAILURE);

			for (i = 0; i < cursor->degree; i++) {
				memcpy((unsigned char *) _new_node->next + tree->v_s * i,
				       (unsigned char *) cursor->next + tree->v_s * i,
				       tree->v_s);

				memcpy((unsigned char *) _new_node->key + tree->k_s * i,
				       (unsigned char *) cursor->key + tree->k_s * i,
				       tree->k_s);
			}

			_new_node->height = cursor->height;
			_new_node->degree = cursor->degree;

			cursor->height++;
			cursor->degree = 2;

			free(cursor->next);
			cursor->next = malloc(sizeof(abt_node_t) * tree->b);
			if (!cursor->next) exit(EXIT_FAILURE);

			memcpy(cursor->next, _new_node, sizeof(abt_node_t));
			memcpy((unsigned char *) cursor->next + sizeof(abt_node_t), insert_pt, sizeof(abt_node_t));

			memcpy((unsigned char *) cursor->key + tree->k_s, insert_key, tree->k_s);
			finished = 1;
		}
	}

	free(insert_key);
	insert_key = NULL;
	stack_destructor(path);
}

void abt_delete(abt_t *tree, void *key) {
	if (!tree) return;

	abt_node_t *cursor = tree->root;

	stack_t *bsi = stack_constructor((void *) free);
	stack_t *path = stack_constructor(NULL);

	while (cursor->height) {
		uint32_t right = cursor->degree;
		uint32_t left  = 0;
		int 	 keys_difference;
		uint32_t mid;

		while (right > left + 1) {
			mid = (right + left) / 2;

			keys_difference = memcmp(key, (unsigned char *) cursor->key + tree->k_s * mid, tree->k_s);

			if (keys_difference < 0)
				right = mid;
			else
				left = mid;
		}

		uint32_t *idx = malloc(sizeof(uint32_t));
		*idx = left;
		stack_push(bsi, idx);
		stack_push(path, cursor);

		cursor = ((void *) ((unsigned char *) cursor->next + sizeof(abt_node_t) * left));
	}

	uint32_t i = 0;
	for (i = 0; i < cursor->degree; i++) {
		if (memcmp(key, (unsigned char *) cursor->key + tree->k_s * i, tree->k_s) == 0)
			break;
	}

	if (i == cursor->degree)
		return;

	for (cursor->degree--; i < cursor->degree; i++) {
		memswp((unsigned char *) cursor->next + i * (cursor->height ? sizeof(abt_node_t) : tree->v_s),
		       (unsigned char *) cursor->next + (i + 1) * (cursor->height ? sizeof(abt_node_t) : tree->v_s),
		       tree->v_s);

		memswp((unsigned char *) cursor->key + i * tree->k_s,
		       (unsigned char *) cursor->key + (i + 1) * tree->k_s,
		       tree->k_s);
	}

	unsigned char finished = 0;
	abt_node_t *temp_node_pt;

	while (!finished) {
		if (cursor->degree >= tree->a) {
			finished = 1;
			break;
		}

		if (stack_is_empty(path)) {
			if (cursor->degree >= 2 || cursor->height == 0)
				finished = 1;
			else {
				// a
				temp_node_pt = cursor->next;

				for (i = 0; i < temp_node_pt->degree; i++) {
					memcpy((unsigned char *) cursor->next + (temp_node_pt->height ? sizeof(abt_node_t) : tree->v_s) * i,
					       (unsigned char *) temp_node_pt->next + (temp_node_pt->height ? sizeof(abt_node_t) : tree->v_s) * i,
					       tree->v_s);

					memcpy((unsigned char *) cursor->key + tree->k_s * i,
					       (unsigned char *) temp_node_pt->key + tree->k_s * i,
					       tree->k_s);
				}

				cursor->degree = temp_node_pt->degree;
				cursor->height = temp_node_pt->height;

				free_node(temp_node_pt);
				finished = 1;
			}
		} else {
			abt_node_t *parent, *sibling;
			uint32_t *index = malloc(sizeof(uint32_t));
			uint32_t j;

			parent = stack_pop(path);
			index = stack_pop(bsi);
			unsigned char cursor_is_leaf = 0;

			if (*index < parent->degree - 1) {
				sibling = ((void *) ((unsigned char *) parent->next + sizeof(abt_node_t) * ((*index) + 1)));

				if (sibling->degree > tree->a) {
					i = cursor->degree;

					if (cursor->height) {
						memcpy((unsigned char *) cursor->key + tree->k_s * i,
						       (unsigned char *) parent->key + tree->k_s * ((*index) + 1),
						       tree->k_s);

					} else {
						memcpy((unsigned char *) cursor->key + tree->k_s * i,
						       (unsigned char *) sibling->key,
						       tree->k_s);

						memcpy((unsigned char *) sibling->key,
						       (unsigned char *) sibling->key + tree->k_s,
						       tree->k_s);

						cursor_is_leaf = !cursor_is_leaf;
					}

					memcpy((unsigned char *) cursor->next + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)) * i,
						(unsigned char *) sibling->next,
						(cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)));

					memcpy((unsigned char *) parent->key + tree->k_s * ((*index) + 1),
						(unsigned char *) sibling->key + tree->k_s,
						tree->k_s);

					memcpy((unsigned char *) sibling->next,
						(unsigned char *) sibling->next + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)),
						(cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)));

					for (j = 2; j < sibling->degree; j++) {
						memcpy((unsigned char *) sibling->next + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)) * (j - 1),
						       (unsigned char *) sibling->next + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)) * j,
						       (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)));

						memcpy((unsigned char *) sibling->key + tree->k_s * (j - 1),
						       (unsigned char *) sibling->key + tree->k_s * j,
						       tree->k_s);
					}

					sibling->degree--;
					cursor->degree++;
					finished = 1;
				} else {
					i = cursor->degree;

					if (cursor->height) {
						memcpy((unsigned char *) cursor->key + tree->k_s * i,
						       (unsigned char *) parent->key + tree->k_s * ((*index) + 1),
						       tree->k_s);
					} else {
						memcpy((unsigned char *) cursor->key + tree->k_s * i,
						       (unsigned char *) sibling->key,
						       tree->k_s);

						cursor_is_leaf = 1;
					}

					memcpy((unsigned char *) cursor->next + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)) * i,
					       sibling->next,
					       cursor_is_leaf ? tree->v_s : sizeof(abt_node_t));

					for (j = 1; j < sibling->degree; j++) {
						memcpy((unsigned char *) cursor->next + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)) * ++i,
						       (unsigned char *) sibling->next + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)) * j,
						       (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)));

						memcpy((unsigned char *) cursor->key + tree->k_s * i,
						       (unsigned char *) sibling->key + tree->k_s * j,
						       tree->k_s);
					}

					cursor->degree = i + 1;
					free_node(sibling);
					parent->degree--;

					for (i = (*index) + 1; i < parent->degree; i++) {
						memswp((unsigned char *) parent->next + sizeof(abt_node_t) * i,
						       (unsigned char *) parent->next + sizeof(abt_node_t) * (i + 1),
						       sizeof(abt_node_t));

						memswp((unsigned char *) parent->key + tree->k_s * i,
						       (unsigned char *) parent->key + tree->k_s * (i + 1),
						       tree->k_s);

						i++;
					}

					cursor = parent;
				}
			} else {
				sibling = ((void *) ((unsigned char *) parent->next + sizeof(abt_node_t) * ((*index) - 1)));

				cursor_is_leaf = !cursor->height;

				if (sibling->degree > tree->a) {
					for (j = cursor->degree; j > 1; j--) {
						memcpy((unsigned char *) cursor->next + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)) * j,
						       (unsigned char *) cursor->next + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)) * (j - 1),
						       (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)));

						memcpy((unsigned char *) cursor->key + tree->k_s * j,
						       (unsigned char *) cursor->key + tree->k_s * (j - 1),
						       tree->k_s);
					}

					memcpy((unsigned char *) cursor->next + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)),
					       (unsigned char *) cursor->next,
					       (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)));

					i = sibling->degree;

					memcpy((unsigned char *) cursor->next,
					       (unsigned char *) sibling->next + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)) * (i - 1),
					       (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)));

					if (cursor->height) {
						memcpy((unsigned char *) cursor->key + tree->k_s,
						       (unsigned char *) parent->key + tree->k_s * *index,
						       tree->k_s);
					} else {
						memcpy((unsigned char *) cursor->key + tree->k_s,
						       (unsigned char *) cursor->key,
						       tree->k_s);

						memcpy((unsigned char *) cursor->key,
						       (unsigned char *) sibling->key + tree->k_s * (i - 1),
						       tree->k_s);
					}

					memcpy((unsigned char *) parent->key + tree->k_s * *index,
					       (unsigned char *) sibling->key + tree->k_s * (i - 1),
					       tree->k_s);

					sibling->degree--;
					cursor->degree++;

					finished = 1;
				} else {
					i = sibling->degree;

					if (cursor->height) {
						memcpy((unsigned char *) sibling->key + tree->k_s * i,
						       (unsigned char *) parent->key + tree->k_s * *index,
						       tree->k_s);
					} else {
						memcpy((unsigned char *) sibling->key + tree->k_s * i,
						       (unsigned char *) cursor->key,
						       tree->k_s);
					}

					memcpy((unsigned char *) sibling->next  + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)) * i,
					       (unsigned char *) cursor->next,
					       (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)));

					for (j = 1; j < cursor->degree; j++) {
						memcpy((unsigned char *) sibling->next  + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)) * ++i,
						       (unsigned char *) cursor->next + (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)) * j,
						       (cursor_is_leaf ? tree->v_s : sizeof(abt_node_t)));

						memcpy((unsigned char *) sibling->key + tree->k_s * i,
						       (unsigned char *) cursor->next + tree->k_s * j,
						       tree->k_s);
					}

					sibling->degree = i + 1;
					free_node(cursor);
					parent->degree--;

					cursor = parent;
				}
			}
			free(index);
		}
	}

	stack_destructor(path);
	stack_destructor(bsi);
}

void *abt_get_node_key(abt_node_t *node);

abt_node_t *abt_new_node(abt_t *tree, uint32_t height, uint32_t degree) {
	abt_node_t *new_node = malloc(sizeof(*new_node));
	if (!new_node) return NULL;

	new_node->height = 0;
	new_node->degree = 0;

	new_node->key = malloc(4 * 4);
	new_node->next = malloc(8 * 4);

	return new_node;
}

static void free_node(abt_node_t *node) {
	if (node && node->key) {
		free(node->key);
		node->key = NULL;
	}

	if (node && node->next) {
		free(node->next);
		node->next = NULL;
	}

	if (node) {
		free(node);
		node = NULL;
	}
}

void *abt_node_get_key(abt_node_t *node);

void abt_draw(abt_t *tree, FILE *stream);

static void memswp(void *a, void *b, size_t size) {
	if (a == b) return;
	register unsigned char t;

	while (size--) {
		t = *(unsigned char *)a;
		*(unsigned char *)a++ = *(unsigned char *) b;
		*(unsigned char *)b++ = t;
	}
}