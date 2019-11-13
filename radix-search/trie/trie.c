#include "trie.h"
#include <stdio.h>

static void 	   trie_postorder_traverse(trie_t trie, trie_node_t cursor, void (*cb)(trie_t trie, trie_node_t node));
static trie_node_t trie_split(trie_t trie, trie_node_t m, trie_node_t n, uint32_t traverse_depth);
static trie_node_t trie_create_node(trie_t trie, const void *key, const void *value);
static void 	   trie_destroy_node(trie_t trie, trie_node_t node);

struct _trie {
	trie_node_t		 root;
	uint32_t 	 	 size;
	const void		*default_value;
	trie_destroy_value_fn_t	*destroy_value_fn;
	trie_destroy_key_fn_t	*destroy_key_fn;
	trie_length_key_fn_t	*length_key_fn;
};

struct _trie_node {
	item_t item;
	trie_node_t edge[2]; /* edge[0] left, edge[1] right */
};

trie_t trie_constructor(
	const void *default_value,
	trie_destroy_key_fn_t destroy_key_fn,
	trie_destroy_value_fn_t destroy_value_fn,
	trie_length_key_fn_t length_key_fn
) {
	trie_t trie = malloc(sizeof(*trie));
	if (!trie) return NULL;

	trie->destroy_value_fn = destroy_value_fn;
	trie->destroy_key_fn   = destroy_key_fn;
	trie->length_key_fn    = length_key_fn;
	trie->default_value    = default_value;
	trie->root	       = NULL;
	trie->size 	       = 0;

	return trie;
}

void trie_destructor(trie_t trie) {
	if (!trie)
		return;

	if (!trie->root)
		goto final;

	trie_postorder_traverse(trie, trie->root, trie_destroy_node);
final:
	free(trie);
	trie = NULL;
}

void trie_insert(trie_t trie, const void *key, const void *value) {
	size_t key_len 		       = trie->length_key_fn(key);
	const unsigned char *u_key     = (const unsigned char *) key;
	const unsigned char *u_key_end = (const unsigned char *) (u_key + key_len);

	uint32_t 	     traverse_depth;
	trie_node_t	     trailing_cursor;
	trie_node_t	     new_node;
	trie_node_t	     cursor;
	uint32_t	     dir;
	unsigned char	     c;

	c = traverse_depth = 0;

	if (!trie->root) {
		trie->root = trie_create_node(trie, key, value);
		return;
	}

	for (cursor = trailing_cursor = trie->root; cursor; cursor = cursor->edge[dir]) {
		if (!cursor->edge[0] && !cursor->edge[1])
			break;

		if ((traverse_depth++ & 7) == 0)
			c = (u_key == u_key_end) ? 0 : *u_key++;

		dir = (c & 0x80) != 0;
		c <<= 1;

		trailing_cursor = cursor;
	}

	if (cursor && cursor->item.key_len == key_len && memcmp(cursor->item.key, key, key_len) == 0)
		return;

	new_node = trie_create_node(trie, key, value);
	if (!new_node) return;

	if (cursor) {
		trie_node_t _new_node = new_node;

		new_node = trie_split(trie, new_node, cursor, traverse_depth);

		if (!new_node) {
			trie_destroy_node(trie, _new_node);
			return;
		}
	}

	// printf("direction: %u\n", dir);

	trailing_cursor->edge[dir] = new_node;
}

void trie_delete();

void *trie_lookup();

static trie_node_t trie_split(trie_t trie, trie_node_t m, trie_node_t n, uint32_t traverse_depth) {
	trie_node_t x;
	const unsigned char *u_key_m;
	const unsigned char *u_key_m_end;

	const unsigned char *u_key_n;
	const unsigned char *u_key_n_end;

	uint32_t m_dir;
	uint32_t n_dir;

	x = trie_create_node(trie, NULL, NULL);
	if (!x)
		return NULL;

	u_key_m	    = m->item.key;
	u_key_m_end = u_key_m + m->item.key_len;

	m_dir = 0;

	if (u_key_m + (traverse_depth >> 3) < u_key_m_end)
		m_dir = (u_key_m[traverse_depth >> 3] >> (7 - (traverse_depth & 7))) & 1;

	u_key_n	    = n->item.key;
	u_key_n_end = u_key_n + n->item.key_len;

	n_dir = 0;

	if (u_key_n + (traverse_depth >> 3) < u_key_n_end)
		n_dir = (u_key_n[traverse_depth >> 3] >> (7 - (traverse_depth & 7))) & 1;

	switch (m_dir * 2 + n_dir) {
		case 0:
			x->edge[0] = trie_split(trie, m, n, traverse_depth + 1);
			break;
		case 1:
			x->edge[0] = m;
			x->edge[1] = n;
			break;
		case 2:
			x->edge[0] = n;
			x->edge[1] = m;
			break;
		case 3:
			x->edge[1] = trie_split(trie, m, n, traverse_depth + 1);
			break;
	}

	return x;
}

static trie_node_t trie_create_node(trie_t trie, const void *key, const void *value) {
	trie_node_t node = malloc(sizeof(*node));
	if (!node) return NULL;

	size_t key_len = key ? trie->length_key_fn(key) : 0;

	node->item.key_len = key_len;
	node->item.value   = value;
	node->edge[0] 	   = NULL;
	node->edge[1] 	   = NULL;
	node->item.key 	   = key;

	trie->size++;

	return node;
}

static void trie_destroy_node(trie_t trie, trie_node_t node) {
	if (trie->destroy_key_fn)
		trie->destroy_key_fn((void *) node->item.key);
	if (trie->destroy_value_fn)
		trie->destroy_value_fn((void *) node->item.value);

	free(node);
	node = NULL;

	trie->size--;
}

static void trie_postorder_traverse(trie_t trie, trie_node_t cursor, void (*cb)(trie_t trie, trie_node_t node)) {
	if (!cursor) return;

	trie_postorder_traverse(trie, cursor->edge[0], cb);
	trie_postorder_traverse(trie, cursor->edge[1], cb);

	cb(trie, cursor);
}