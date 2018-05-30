/*
 * Generic Binary Search Tree
 *
 * May 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include <stdlib.h>
#include "__debug__.h"
#include "splay-tree.h"

struct _splay_tree {
	int (*cmp)(const void *, const void *);	/* Compare function for the key's type */
	splay_tree_node root;				/* Pointer to the tree's root node */
};

struct _splay_tree_node {
	const void *	key;	/* Pointer to a value of a node */
	splay_tree_node parent;	/* Pointer to a parent node */
	splay_tree_node right;	/* Pointer to a right node */
	splay_tree_node left;	/* Pointer to a left node */
};

static splay_tree_node
_splay_tree_new_node(splay_tree, const void *, void *, void *, void *);

static splay_tree_node
_splay_tree_search(const splay_tree, const splay_tree_node, const void *);

static void
_splay_tree_traverse(splay_tree_node, void (*)(const void *));

static splay_tree_node
_splay_tree_minimum(splay_tree_node cursor);

static splay_tree_node
_splay_tree_maximum(splay_tree_node cursor);

static void
_splay_tree_destructor(splay_tree_node);

/**
 * splay_tree_constructor - creates and returns pointer to a binary search tree.
 * @cmp: compare function for the type of which st is going to be
 *
 * This function creates new generic st, initializes, and returns pointer to it.
 */

splay_tree
splay_tree_constructor(int (*cmp)(const void *, const void *))
{
	check(cmp, -1);

	splay_tree st = malloc(sizeof(*st));
	check(st, 12);
	st->root = NULL;
	st->cmp = cmp;

	return st;
error:
	splay_tree_destructor(st);

	return NULL;
}

/**
 * splay_tree_insert - inserts value in appropriate place in the st.
 * @st: pointer to a splay_tree structure
 * @key: pointer to the value to insert
 *
 * This function finds appropriate place to insert new node,
 * creates it with appropriate key and inserts it into st.
 */

void
splay_tree_insert(const splay_tree st, const void *key)
{
	splay_tree_node cursor = st->root;
	splay_tree_node trailing_cursor = NULL;

	while (cursor != NULL) {
		trailing_cursor = cursor;

		if (st->cmp(key, cursor->key) < 0)
			cursor = cursor->left;
		else
			cursor = cursor->right;

	}

	splay_tree_node new_node = _splay_tree_new_node(st, key, NULL, NULL, trailing_cursor);
	check(new_node, 12);

	if (trailing_cursor == NULL)
		st->root = new_node;
	else if (st->cmp(key, trailing_cursor->key) < 0)
		trailing_cursor->left = new_node;
	else
		trailing_cursor->right = new_node;

	return;
error:
	splay_tree_destructor(st);
}

void
splay_tree_splay(splay_tree st, splay_tree_node node) {
	check(st, -1);
	check(node, -1);

	while (1) {
		splay_tree_node node_parent = node->parent;

		if (!node_parent) break;

		splay_tree_node node_grandparent = node_parent->parent;

		/* Zig situation */
		if (!node_grandparent) {
			if (node_parent->left == node)
				splay_tree_right_rotation(node_parent);
			else
				splay_tree_left_rotation(node_parent);
			break;
		}

		if (node_grandparent->left == node_parent) {
			/* ZigZig situation*/
			if (node_parent->left == node) {
				splay_tree_right_rotation(node_grandparent);
				splay_tree_right_rotation(node_parent);
			}
			/* ZigZag situation*/
			else {
				splay_tree_left_rotation(node_parent);
				splay_tree_right_rotation(node_grandparent);
			}
		} else {
			/* ZigZag situation */
			if (node_parent->left == node) {
				splay_tree_right_rotation(node_parent);
				splay_tree_left_rotation(node_grandparent);
			}
			/* ZigZig situation */
			else {
				splay_tree_left_rotation(node_grandparent);
				splay_tree_left_rotation(node_parent);
			}
		}
	}

	st->root = node;
	return;
error:
	splay_tree_destructor(st);
}

void
splay_tree_right_rotation(splay_tree_node node) {
	splay_tree_node node_left 	= node->left;
	splay_tree_node node_left_right = node_left->right;
	splay_tree_node node_parent 	= node->parent;

	if (node_parent) {
		if (node_parent->right == node)
			node_parent->right = node_left;
		else
			node_parent->left = node_left;
	}

	if (node_left_right)
		node_left_right->parent = node;

	node_left->parent = node_parent;
	node_left->right = node;

	node->parent = node_left;
	node->left = node_left_right;
}

void
splay_tree_left_rotation(splay_tree_node node) {
	splay_tree_node node_right 	= node->right;
	splay_tree_node node_right_left = node_right->left;
	splay_tree_node node_parent	= node->parent;

	if (node_parent) {
		if (node_parent->right == node)
			node_parent->right = node_right;
		else
			node_parent->left = node_right;
	}

	if (node_right_left)
		node_right_left->parent = node;

	node_right->parent = node_parent;
	node_right->left = node;

	node->parent = node_right;
	node->right = node_right_left;
}

/**
 * _splay_tree_new_node - creates new node with given key.
 * @st: pointer to a splay_tree we are working with
 * @key: pointer to a key value
 * @left: pointer to the left node
 * @right: pointer to the right node
 * @parent: pointer to the parent node
 *
 * This is helper function that create new node with appropriate
 * contents i.e. parent, left, right pointers and key pointer.
 */

static splay_tree_node
_splay_tree_new_node(splay_tree st, const void *key,
			     void *left, void *right, void *parent)
{
	check(st, -1);
	check(key, -1);

	splay_tree_node new_node = malloc(sizeof(*new_node));
	check(new_node, 12);

	new_node->key = key;
	new_node->parent = parent;
	new_node->right = right;
	new_node->left = left;

	return new_node;
error:
	splay_tree_destructor(st);

	return NULL;
}

/**
 * splay_tree_traverse - traverses st, applying provided function.
 * @st: pointer to a splay_tree we want to traverse
 * @fn: pointer to the callback function
 *
 * This function does inorder traversal the splay_tree
 * applying provided function to each node.
 *
 */

void
splay_tree_traverse(const splay_tree st,
			    void (*fn)(const void *key))
{
	check(st, -1);
	check(fn, -1);

	_splay_tree_traverse(st->root, fn);

	return;
error:
	splay_tree_destructor(st);
}

static void
_splay_tree_traverse(splay_tree_node cursor, void (*fn)(const void *key))
{
	if(!cursor) return;

	_splay_tree_traverse(cursor->left, fn);
	fn(cursor->key);
	_splay_tree_traverse(cursor->right, fn);
}

/**
 * splay_tree_search - searches st for a node with specified key set.
 * @st: pointer to a splay_tree we are searching
 * @key: pointer to a key we are looking to be set in the node
 *
 * This function searches splay_tree st for a node with key
 * set to specified key
 */

splay_tree_node
splay_tree_search(const splay_tree st, const void *key)
{
	check(st, -1);
	check(key, -1);

	splay_tree_node t = _splay_tree_search(st, st->root, key);

	splay_tree_splay(st, t);
	return t;
error:
	splay_tree_destructor(st);
	return NULL;
}

static splay_tree_node
_splay_tree_search(const splay_tree st,
			   const splay_tree_node cursor, const void *key)
{
	splay_tree_node _cursor = cursor;
	int cmp_res;

	while (_cursor != NULL && ((cmp_res = st->cmp(key, _cursor->key)) != 0)) {
		if (cmp_res < 0)
			_cursor = _cursor->left;
		else
			_cursor = _cursor->right;
	}

	return _cursor;
}

/**
 * splay_tree_apply_fn_to_node - searches st for a node with specified key set.
 * @st: pointer to a splay_tree we are working with
 * @node: pointer to a node we want to apply function to
 * @fn: pointer to a function we want to apply to node
 *
 * This function applies specified function to a node
 */

void
splay_tree_apply_fn_to_node(splay_tree st, splay_tree_node node,
					 void (*fn)(const void *key))
{
	check(st, -1);
	check(fn, -1);

	fn(node->key);
	return;
error:
	splay_tree_destructor(st);
}

/**
 * splay_tree_minimum - finds and returns minimum node in the st.
 * @st: pointer to a splay_tree we are working with
 *
 * This function finds node with smallest key set and returns it
 */

splay_tree_node
splay_tree_minimum(splay_tree st)
{
	check(st, -1);
	check(st->root, -1);

	return _splay_tree_minimum(st->root);
error:
	splay_tree_destructor(st);
	return NULL;
}

splay_tree_node
_splay_tree_minimum(splay_tree_node cursor)
{
	while (cursor->left)
		cursor = (cursor->left);

	return cursor;
}

/**
 * splay_tree_maximum - finds and returns maximum node in the st.
 * @st: pointer to a splay_tree we are working with
 *
 * This function finds node with biggest key set and returns it
 */

splay_tree_node
splay_tree_maximum(splay_tree st)
{
	check(st, -1);
	check(st->root, -1);

	return _splay_tree_maximum(st->root);
error:
	splay_tree_destructor(st);
	return NULL;
}

static splay_tree_node
_splay_tree_maximum(splay_tree_node cursor)
{
	while (cursor->right)
		cursor = (cursor->right);

	return cursor;
}

/**
 * splay_tree_successor - finds and returns successor node of the node
 * @node: pointer to a splay_tree_node we want to know successor of
 *
 * This function finds node with the smallest greater key
 * than set in the provided node.
 */

splay_tree_node
splay_tree_successor(splay_tree_node node)
{
	if(!node) return NULL;

	if (node->right)
		return _splay_tree_minimum(node->right);

	splay_tree_node cursor = node->parent;

	while (cursor && node == cursor->right) {
		node = cursor;
		cursor = cursor->parent;
	}

	return cursor;
}

/**
 * splay_tree_predecessor - finds and returns predecessor node of the node
 * @node: pointer to a splay_tree_node we want to know predecessor of
 *
 * This function finds node with the biggest smaller key
 * than set in the provided node.
 */

splay_tree_node
splay_tree_predecessor(splay_tree_node node)
{
	if(!node) return NULL;

	if (node->left)
		return _splay_tree_maximum(node->left);

	splay_tree_node cursor = node->parent;

	while (cursor && node == cursor->left) {
		node = cursor;
		cursor = cursor->parent;
	}

	return cursor;
}

/**
 * splay_tree_delete - deletes node with provided key from the st
 * @st: pointer to a splay_tree we want to delete node from
 * @key: pointer to the key with which node should be set to be deleted
 *
 * This function finds node which has the provided key set, and deletes it
 * restoring the binary search tree structure afterwards
 */

void
splay_tree_delete(const splay_tree st, void *key)
{
	_splay_tree_delete(st, splay_tree_search(st, key));
}

void
_splay_tree_delete(const splay_tree st, splay_tree_node cursor)
{
	if (!cursor->left)
		splay_tree_transplant(st, cursor, cursor->right);
	else if (!cursor->right)
		splay_tree_transplant(st, cursor, cursor->left);
	else {
		splay_tree_node t = _splay_tree_minimum(cursor->right);

		if (t->parent != cursor) {
			splay_tree_transplant(st, t, t->right);
			t->right = cursor->right;
			t->right->parent = t;
		}

		splay_tree_transplant(st, cursor, t);
		t->left = cursor->left;
		t->left->parent = t;
	}

	free(cursor);
}

/**
 * splay_tree_transplant - replaces sustree x with sustree y
 * @st: pointer to a splay_tree we are working with
 * @x: pointer to the splay_tree_node which we want to replace
 * @y: pointer to the splay_tree_node with which we want to replace x
 *
 * This function replaces one subtree with another one,
 * freeing memory left to the caller
 */

void
splay_tree_transplant(splay_tree st, splay_tree_node x, splay_tree_node y)
{
	if (!x->parent)
		st->root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	if (y)
		y->parent = x->parent;
}

/**
 * destroy_splay_tree - frees memmory used by a binary search tree
 * @st: pointer to a splay_tree structure
 *
 * This function cleans up memmory used by a binary search tree,
 * by first freeing all nodes, and then st struct itself.
 */

void
splay_tree_destructor(splay_tree st)
{
	if (!st) return;

	_splay_tree_destructor(st->root);
	free(st);
	st = NULL;
}

static void
_splay_tree_destructor(splay_tree_node cursor)
{
	if (!cursor) return;

	_splay_tree_destructor(cursor->left);
	_splay_tree_destructor(cursor->right);
	free(cursor);
	cursor = NULL;
}