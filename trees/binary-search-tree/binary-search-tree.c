/*
 * Generic Binary Search Tree
 *
 * May 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include <stdlib.h>
#include "__debug__.h"
#include "binary-search-tree.h"

struct _binary_search_tree {
	int (*cmp)(const void *, const void *);	/* Compare function for the key's type */
	bst_node root;				/* Pointer to the tree's root node */
};

struct _bst_node {
	const void *	key;	/* Pointer to a value of a node */
	bst_node 	parent;	/* Pointer to a parent node */
	bst_node 	right;	/* Pointer to a right node */
	bst_node 	left;	/* Pointer to a left node */
};

static bst_node
_binary_search_tree_new_node(binary_search_tree, const void *, void *, void *, void *);

static bst_node
_binary_search_tree_search(const binary_search_tree, const bst_node, const void *);

static void
_binary_search_tree_traverse(bst_node, void (*)(const void *));

static bst_node
_binary_search_tree_minimum(bst_node cursor);

static bst_node
_binary_search_tree_maximum(bst_node cursor);

static void
_binary_search_tree_destructor(bst_node);

/**
 * binary_search_tree_constructor - creates and returns pointer to a binary search tree.
 * @cmp: compare function for the type of which bst is going to be
 *
 * This function creates new generic BST, initializes, and returns pointer to it.
 */

binary_search_tree
binary_search_tree_constructor(int (*cmp)(const void *, const void *))
{
	check(cmp, -1);

	binary_search_tree bst = malloc(sizeof(*bst));
	check(bst, 12);
	bst->root = NULL;
	bst->cmp = cmp;

	return bst;
error:
	binary_search_tree_destructor(bst);

	return NULL;
}

/**
 * binary_search_tree_insert - inserts value in appropriate place in the bst.
 * @bst: pointer to a binary_search_tree structure
 * @key: pointer to the value to insert
 *
 * This function finds appropriate place to insert new node,
 * creates it with appropriate key and inserts it into bst.
 */

void
binary_search_tree_insert(const binary_search_tree bst, const void *key)
{
	bst_node cursor = bst->root;
	bst_node trailing_cursor = NULL;

	while (cursor != NULL) {
		trailing_cursor = cursor;

		if (bst->cmp(key, cursor->key) < 0)
			cursor = cursor->left;
		else
			cursor = cursor->right;

	}

	bst_node new_node = _binary_search_tree_new_node(bst, key, NULL, NULL, trailing_cursor);
	check(new_node, 12);

	if (trailing_cursor == NULL)
		bst->root = new_node;
	else if (bst->cmp(key, trailing_cursor->key) < 0)
		trailing_cursor->left = new_node;
	else
		trailing_cursor->right = new_node;

	return;
error:
	binary_search_tree_destructor(bst);
}

/**
 * _binary_search_tree_new_node - creates new node with given key.
 * @bst: pointer to a binary_search_tree we are working with
 * @key: pointer to a key value
 * @left: pointer to the left node
 * @right: pointer to the right node
 * @parent: pointer to the parent node
 *
 * This is helper function that create new node with appropriate
 * contents i.e. parent, left, right pointers and key pointer.
 */

static bst_node
_binary_search_tree_new_node(binary_search_tree bst, const void *key,
			     void *left, void *right, void *parent)
{
	check(bst, -1);
	check(key, -1);

	bst_node new_node = malloc(sizeof(*new_node));
	check(new_node, 12);

	new_node->key = key;
	new_node->parent = parent;
	new_node->right = right;
	new_node->left = left;

	return new_node;
error:
	binary_search_tree_destructor(bst);

	return NULL;
}

/**
 * binary_search_tree_traverse - traverses bst, applying provided function.
 * @bst: pointer to a binary_search_tree we want to traverse
 * @fn: pointer to the callback function
 *
 * This function does inorder traversal the binary_search_tree
 * applying provided function to each node.
 *
 */

void
binary_search_tree_traverse(const binary_search_tree bst,
			    void (*fn)(const void *key))
{
	check(bst, -1);
	check(fn, -1);

	_binary_search_tree_traverse(bst->root, fn);

	return;
error:
	binary_search_tree_destructor(bst);
}

static void
_binary_search_tree_traverse(bst_node cursor, void (*fn)(const void *key))
{
	if(!cursor) return;

	_binary_search_tree_traverse(cursor->left, fn);
	fn(cursor->key);
	_binary_search_tree_traverse(cursor->right, fn);
}

/**
 * binary_search_tree_search - searches bst for a node with specified key set.
 * @bst: pointer to a binary_search_tree we are searching
 * @key: pointer to a key we are looking to be set in the node
 *
 * This function searches binary_search_tree bst for a node with key
 * set to specified key
 */

bst_node
binary_search_tree_search(const binary_search_tree bst, const void *key)
{
	check(bst, -1);
	check(key, -1);

	return _binary_search_tree_search(bst, bst->root, key);
error:
	binary_search_tree_destructor(bst);
	return NULL;
}

static bst_node
_binary_search_tree_search(const binary_search_tree bst,
			   const bst_node cursor, const void *key)
{
	bst_node _cursor = cursor;
	int cmp_res;

	while (_cursor != NULL && ((cmp_res = bst->cmp(key, _cursor->key)) != 0)) {
		if (cmp_res < 0)
			_cursor = _cursor->left;
		else
			_cursor = _cursor->right;
	}

	return _cursor;
}

/**
 * binary_search_tree_apply_fn_to_node - searches bst for a node with specified key set.
 * @bst: pointer to a binary_search_tree we are working with
 * @node: pointer to a node we want to apply function to
 * @fn: pointer to a function we want to apply to node
 *
 * This function applies specified function to a node
 */

void
binary_search_tree_apply_fn_to_node(binary_search_tree bst, bst_node node,
					 void (*fn)(const void *key))
{
	check(bst, -1);
	check(fn, -1);

	fn(node->key);
	return;
error:
	binary_search_tree_destructor(bst);
}

/**
 * binary_search_tree_minimum - finds and returns minimum node in the bst.
 * @bst: pointer to a binary_search_tree we are working with
 *
 * This function finds node with smallest key set and returns it
 */

bst_node
binary_search_tree_minimum(binary_search_tree bst)
{
	check(bst, -1);
	check(bst->root, -1);

	return _binary_search_tree_minimum(bst->root);
error:
	binary_search_tree_destructor(bst);
	return NULL;
}

bst_node
_binary_search_tree_minimum(bst_node cursor)
{
	while (cursor->left)
		cursor = (cursor->left);

	return cursor;
}

/**
 * binary_search_tree_maximum - finds and returns maximum node in the bst.
 * @bst: pointer to a binary_search_tree we are working with
 *
 * This function finds node with biggest key set and returns it
 */

bst_node
binary_search_tree_maximum(binary_search_tree bst)
{
	check(bst, -1);
	check(bst->root, -1);

	return _binary_search_tree_maximum(bst->root);
error:
	binary_search_tree_destructor(bst);
	return NULL;
}

static bst_node
_binary_search_tree_maximum(bst_node cursor)
{
	while (cursor->right)
		cursor = (cursor->right);

	return cursor;
}

/**
 * binary_search_tree_successor - finds and returns successor node of the node
 * @node: pointer to a bst_node we want to know successor of
 *
 * This function finds node with the smallest greater key
 * than set in the provided node.
 */

bst_node
binary_search_tree_successor(bst_node node)
{
	if(!node) return NULL;

	if (node->right)
		return _binary_search_tree_minimum(node->right);

	bst_node cursor = node->parent;

	while (cursor && node == cursor->right) {
		node = cursor;
		cursor = cursor->parent;
	}

	return cursor;
}

/**
 * binary_search_tree_predecessor - finds and returns predecessor node of the node
 * @node: pointer to a bst_node we want to know predecessor of
 *
 * This function finds node with the biggest smaller key
 * than set in the provided node.
 */

bst_node
binary_search_tree_predecessor(bst_node node)
{
	if(!node) return NULL;

	if (node->left)
		return _binary_search_tree_maximum(node->left);

	bst_node cursor = node->parent;

	while (cursor && node == cursor->left) {
		node = cursor;
		cursor = cursor->parent;
	}

	return cursor;
}

/**
 * binary_search_tree_delete - deletes node with provided key from the bst
 * @bst: pointer to a binary_search_tree we want to delete node from
 * @key: pointer to the key with which node should be set to be deleted
 *
 * This function finds node which has the provided key set, and deletes it
 * restoring the binary search tree structure afterwards
 */

void
binary_search_tree_delete(const binary_search_tree bst, void *key)
{
	_binary_search_tree_delete(bst, binary_search_tree_search(bst, key));
}

void
_binary_search_tree_delete(const binary_search_tree bst, bst_node cursor)
{
	if (!cursor->left)
		binary_search_tree_transplant(bst, cursor, cursor->right);
	else if (!cursor->right)
		binary_search_tree_transplant(bst, cursor, cursor->left);
	else {
		bst_node t = _binary_search_tree_minimum(cursor->right);

		if (t->parent != cursor) {
			binary_search_tree_transplant(bst, t, t->right);
			t->right = cursor->right;
			t->right->parent = t;
		}

		binary_search_tree_transplant(bst, cursor, t);
		t->left = cursor->left;
		t->left->parent = t;
	}

	free(cursor);
}

/**
 * binary_search_tree_transplant - replaces substree x with substree y
 * @bst: pointer to a binary_search_tree we are working with
 * @x: pointer to the bst_node which we want to replace
 * @y: pointer to the bst_node with which we want to replace x
 *
 * This function replaces one subtree with another one,
 * freeing memory left to the caller
 */

void
binary_search_tree_transplant(binary_search_tree bst, bst_node x, bst_node y)
{
	if (!x->parent)
		bst->root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	if (y)
		y->parent = x->parent;
}

/**
 * destroy_binary_search_tree - frees memmory used by a binary search tree
 * @bst: pointer to a binary_search_tree structure
 *
 * This function cleans up memmory used by a binary search tree,
 * by first freeing all nodes, and then bst struct itself.
 */

void
binary_search_tree_destructor(binary_search_tree bst)
{
	if (!bst) return;

	_binary_search_tree_destructor(bst->root);
	free(bst);
	bst = NULL;
}

static void
_binary_search_tree_destructor(bst_node cursor)
{
	if (!cursor) return;

	_binary_search_tree_destructor(cursor->left);
	_binary_search_tree_destructor(cursor->right);
	free(cursor);
	cursor = NULL;
}