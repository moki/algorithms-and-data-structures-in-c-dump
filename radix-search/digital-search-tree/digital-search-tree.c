#include "digital-search-tree.h"
#include <stdio.h>

static void 	  dst_postorder_traverse(dst_t tree, dst_node_t cursor, void (*cb)(dst_t tree, dst_node_t node));
static dst_node_t dst_create_node(dst_t tree, const void *key, const void *value);
static void 	  dst_destroy_node(dst_t tree, dst_node_t node);

struct _dst {
	dst_node_t 	 	 root;
	uint32_t 	 	 size;
	const void		*default_value;
	dst_destroy_key_fn_t	*destroy_key_fn;
	dst_destroy_value_fn_t	*destroy_val_fn;
	dst_length_key_fn_t	*length_key_fn;
};

struct _dst_node {
	item_t 	   item;
	dst_node_t edge[2];				/* edge[0] left, edge[1] right */
};

extern dst_t dst_constructor(
	const void 		*default_value,
	dst_destroy_key_fn_t	*destroy_key_fn,
	dst_destroy_value_fn_t	*destroy_value_fn,
	dst_length_key_fn_t	*length_key_fn
) {
	dst_t tree = malloc(sizeof(*tree));
	if (!tree) return NULL;

	tree->destroy_val_fn = destroy_value_fn;
	tree->destroy_key_fn = destroy_key_fn;
	tree->length_key_fn  = length_key_fn;
	tree->default_value  = default_value;
	tree->root 	     = NULL;
	tree->size 	     = 0;

	return tree;
}

extern void dst_destructor(dst_t tree) {
	if (!tree)
		return;

	if (!tree->root)
		goto final;

	dst_postorder_traverse(tree, tree->root, dst_destroy_node);
final:
	free(tree);
	tree = NULL;
}

extern void *dst_lookup(dst_t tree, const void *key) {
	size_t key_len = tree->length_key_fn(key);
	const unsigned char *u_key     = (const unsigned char *) key;
	const unsigned char *u_key_end = (const unsigned char *) (u_key + key_len);

	uint32_t 	     traverse_depth;
	dst_node_t	     cursor;
	int 		     dir;
	unsigned char	     c;

	c = traverse_depth = 0;

	for (cursor = tree->root; cursor; cursor = cursor->edge[dir]) {
		if (cursor->item.key_len == key_len && memcmp(cursor->item.key, key, key_len) == 0)
			return (void *) cursor->item.value;

		if ((traverse_depth++ & 7) == 0)
			c = (u_key == u_key_end) ? 0 : *u_key++;

		dir = (c & 0x80) != 0;
		c <<= 1;
	}

	return (void *) tree->default_value;
}

extern void dst_delete(dst_t tree, const void *key) {
	size_t key_len = tree->length_key_fn(key);
	const unsigned char *u_key     = (const unsigned char *) key;
	const unsigned char *u_key_end = (const unsigned char *) (u_key + key_len);

	uint32_t 	     traverse_depth;
	dst_node_t	     trailing_cursor;
	dst_node_t	     cursor;
	int 		     dir;
	unsigned char	     c;

	c = traverse_depth = 0;

	for (cursor = trailing_cursor = tree->root; cursor; cursor = cursor->edge[dir]) {
		if (cursor->item.key_len == key_len && memcmp(cursor->item.key, key, key_len) == 0)
			break;

		if ((traverse_depth++ & 7) == 0)
			c = (u_key == u_key_end) ? 0 : *u_key++;

		dir = (c & 0x80) != 0;
		c <<= 1;

		trailing_cursor = cursor;
	}

	/* target node doesnt exists */
	if (!cursor) return;

	/* leaf node case */
	if (!cursor->edge[0] && !cursor->edge[1]) {
		if (trailing_cursor)
			trailing_cursor->edge[dir] = NULL;

		if (cursor == tree->root)
			tree->root = NULL;

		dst_destroy_node(tree, cursor);
		return;
	}

	/* have children case */
	dst_node_t	     _cursor;
	dst_node_t	     _trailing_cursor;
	int 		     sub_dir;

	traverse_depth = 0;
	u_key = (const unsigned char *) key;

	_cursor = cursor;

	while (_cursor->edge[0] || _cursor->edge[1]) {
		if (!_cursor->edge[1])
			sub_dir = 0;
		else if (!_cursor->edge[0])
			sub_dir = 1;
		else {
			if ((traverse_depth & 7) == 0)
				c = (u_key == u_key_end) ? 0 : *u_key++;

			sub_dir = (c & 0x80) != 0;
			c <<= 1;
		}

		_trailing_cursor = _cursor;
		_cursor = _cursor->edge[sub_dir];
		traverse_depth++;
	}

	_trailing_cursor->edge[sub_dir] = NULL;
	_cursor->edge[0] = cursor->edge[0];
	_cursor->edge[1] = cursor->edge[1];

	if (cursor == tree->root)
		tree->root = _cursor;
	else if (trailing_cursor)
		trailing_cursor->edge[dir] = _cursor;

	dst_destroy_node(tree, cursor);
}

extern void  dst_insert(dst_t tree, const void *key, const void *value) {
	size_t key_len = tree->length_key_fn(key);
	const unsigned char *u_key     = (const unsigned char *) key;
	const unsigned char *u_key_end = (const unsigned char *) (u_key + key_len);

	uint32_t 	     traverse_depth;
	dst_node_t	     trailing_cursor;
	dst_node_t	     cursor;
	int 		     dir;
	unsigned char	     c;

	c = traverse_depth = 0;

	if (!tree->root) {
		tree->root = dst_create_node(tree, key, value);
		return;
	}

	for (cursor = trailing_cursor = tree->root; cursor; cursor = cursor->edge[dir]) {
		if (cursor->item.key_len == key_len && memcmp(cursor->item.key, key, key_len) == 0)
			return;

		if ((traverse_depth++ & 7) == 0)
			c = (u_key == u_key_end) ? 0 : *u_key++;

		dir = (c & 0x80) != 0;
		c <<= 1;

		trailing_cursor = cursor;
	}

	trailing_cursor->edge[dir] = dst_create_node(tree, key, value);
}

static void dst_postorder_traverse(dst_t tree, dst_node_t cursor, void (*cb)(dst_t tree, dst_node_t node)) {
	if (!cursor) return;

	dst_postorder_traverse(tree, cursor->edge[0], cb);
	dst_postorder_traverse(tree, cursor->edge[1], cb);

	cb(tree, cursor);
}

static dst_node_t dst_create_node(dst_t tree, const void *key, const void *value) {
	dst_node_t node = malloc(sizeof(*node));
	if (!node) return NULL;
	size_t key_len = tree->length_key_fn(key);

	node->item.key_len = key_len;
	node->item.value   = value;
	node->edge[0] 	   = NULL;
	node->edge[1] 	   = NULL;
	node->item.key 	   = key;

	tree->size++;

	return node;
}

static void dst_destroy_node(dst_t tree, dst_node_t node) {
	if (tree->destroy_key_fn)
		tree->destroy_key_fn((void *) node->item.key);
	if (tree->destroy_val_fn)
		tree->destroy_val_fn((void *) node->item.value);

	free(node);
	node = NULL;

	tree->size--;
}