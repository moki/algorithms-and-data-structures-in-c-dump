/*
 * Generic Red Black Tree
 *
 * June 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include <stdlib.h>
#include "__debug__.h"
#include "red-black-tree.h"

struct _red_black_tree {
	int (*cmp)(const void *, const void *);	/* Key Type Compare function */
	red_black_tree_node root;		/* Tree's root */
};

struct _red_black_tree_node {
	int red;			/* Node's color 1 - red, 0 otherwise */
	const void *key;		/* Pointer to a value of a node */
	red_black_tree_node link[2];	/* Array of Pointers, link[0] left, link[1] right */
};

static int
_rb_is_node_red(red_black_tree_node);

static red_black_tree_node
_rb_create_new_node(red_black_tree, void *);

static red_black_tree_node
_rb_rotation_single(red_black_tree_node, int);

static red_black_tree_node
_rb_rotation_double(red_black_tree_node, int);

static red_black_tree_node
_rb_insert(red_black_tree, red_black_tree_node, void *);

static red_black_tree_node
_rb_delete(red_black_tree, red_black_tree_node, void *, int *);

static red_black_tree_node
_rb_delete_balance(red_black_tree_node, int, int *);

static void
_rb_traverse(red_black_tree_node cursor, void (*fn)(const void *key));

static int
_rb_assert(red_black_tree, red_black_tree_node);

static void
_rb_destructor(red_black_tree_node);

/**
 * red_black_tree_is_node_red - checks node color => 1 if red, 0 otherwise.
 * @node: node which color we are checking
 *
 * This function checks nodes color, returns 1 if it is red, and 0 otherwise,
 * assumes that null pointers are black.
 */
static int
_rb_is_node_red(red_black_tree_node node)
{

	return node ? node->red : 0;
}

static red_black_tree_node
_rb_create_new_node(red_black_tree rbt, void *key) {
	red_black_tree_node new_node = malloc(sizeof(*new_node));

	new_node->key = key;
	new_node->red = 1;
	new_node->link[0] = new_node->link[1] = NULL;

	return new_node;
}

red_black_tree
rb_constructor(int (*cmp)(const void *, const void *)) {
	check(cmp, -1);

	red_black_tree rbt = malloc(sizeof(*rbt));
	check(rbt, 12);

	rbt->cmp = cmp;
	rbt->root = NULL;

	return rbt;
error:
	rb_destructor(rbt);
	return NULL;
}

/**
 * red_black_tree_rotation_single - rotates the node, in the provided direction.
 * @node: node on which we want to perform the rotation
 * @direction: direction of the rotation 0 for left, 1 for right
 *
 * This function rotates the tree rooted at node <node>
 * in the provided direction <direction>, 0 for left and 1 for right.
 * It also sets old root to be red and the new one to be black.
 */
static red_black_tree_node
_rb_rotation_single(red_black_tree_node node, int direction)
{
	red_black_tree_node t = node->link[!direction];

	node->link[!direction] = t->link[direction];
	t->link[direction] = node;

	node->red = 1;
	t->red = 0;

	return t;
}

static red_black_tree_node
_rb_rotation_double(red_black_tree_node node, int direction)
{

	node->link[!direction] = _rb_rotation_single(
					node->link[!direction],
					!direction);

	return _rb_rotation_single(node, direction);
}

void
rb_insert(red_black_tree rbt, void *key) {
	check(rbt, -1);
	check(key, -1);

	rbt->root = _rb_insert(rbt, rbt->root, key);
	rbt->root->red = 0;

	return;
error:
	rb_destructor(rbt);
}

static red_black_tree_node
_rb_insert(red_black_tree rbt, red_black_tree_node node, void *key) {
	if (!node) {
		node = _rb_create_new_node(rbt, key);
	} else if (rbt->cmp(node->key, key) != 0) {
		int dir = rbt->cmp(node->key, key) < 0;

		node->link[dir] = _rb_insert(rbt, node->link[dir], key);

		if (_rb_is_node_red(node->link[dir])) {
			if (_rb_is_node_red(node->link[!dir])) {
				node->red = 1;
				node->link[0]->red = 0;
				node->link[1]->red = 0;
			} else {
				if (_rb_is_node_red(node->link[dir]->link[dir])) {
					node = _rb_rotation_single(node, !dir);
				} else if (_rb_is_node_red(node->link[dir]->link[!dir])) {
					node = _rb_rotation_double(node, !dir);
				}
			}
		}
	}

	return node;
}

void
rb_delete(red_black_tree rbt, void *key) {
	int done = 0;

	rbt->root = _rb_delete(rbt, rbt->root, key, &done);

	if (rbt->root)
		rbt->root->red = 0;
}

static red_black_tree_node
_rb_delete(red_black_tree rbt, red_black_tree_node node, void *key, int *done) {
	if (!node) {
		*done = 1;
	} else {
		int dir;

		if (rbt->cmp(node->key, key) == 0) {
			if (!node->link[0] || !node->link[1]) {
				red_black_tree_node t = node->link[node->link[0] == NULL];

				if (_rb_is_node_red(node))
					*done = 1;
				else if (_rb_is_node_red(t)) {
					t->red = 0;
					*done = 1;
				}

				free(node);

				return t;
			} else {
				red_black_tree_node heir = node->link[0];

				while (heir->link[1] != NULL)
					heir = heir->link[1];

				node->key = heir->key;
				key = heir->key;
			}
		}

		dir = rbt->cmp(node->key, key) < 0;
		node->link[dir] = _rb_delete(rbt, node->link[dir], key, done);

		if (!*done)
			node = _rb_delete_balance(node, dir, done);
	}

	return node;
}

static red_black_tree_node
_rb_delete_balance(red_black_tree_node node, int dir, int *done) {
	red_black_tree_node p = node;
	red_black_tree_node s = node->link[!dir];

	if (s && !_rb_is_node_red(s)) {
		/* Black sibling case */
		if (!_rb_is_node_red(s->link[0]) && !_rb_is_node_red(s->link[1])) {
			if (_rb_is_node_red(p))
				*done = 1;

			p->red = 0;
			s->red = 1;
		} else {
			int _c = node->red;

			if (_rb_is_node_red(s->link[!dir]))
				p = _rb_rotation_single(p, dir);
			else
				p = _rb_rotation_double(p, dir);

			p->red = _c;
			p->link[0]->red = 0;
			p->link[1]->red = 0;
			*done = 1;
		}
	} else if (s->link[dir]) {
		/* Red sibling case */
		red_black_tree_node r = s->link[dir];

		if (!_rb_is_node_red(r->link[0]) && !_rb_is_node_red(r->link[1])) {
			p = _rb_rotation_single(p, dir);
			p->link[dir]->link[!dir]->red = 1;
		} else {
			if (_rb_is_node_red(r->link[dir]))
				s->link[dir] = _rb_rotation_single(r, !dir);

			p = _rb_rotation_double(p, dir);
			s->link[dir]->red = 0;
			p->link[!dir]->red = 1;
		}

		p->red = 0;
		p->link[dir]->red = 0;
		*done = 1;
	}

	return p;
}

/**
 * rb_traverse - traverses red black tree, applying provided function.
 * @rb: pointer to a red_blackl_tree we are traversing
 * @fn: pointer to the callback function
 *
 * This function does inorder traversal of the red_black_tree
 * calling provided function on each node's key.
 */

void
rb_traverse(const red_black_tree rbt, void (*fn)(const void *key))
{
	check(rbt, -1);
	check(fn, -1);

	_rb_traverse(rbt->root, fn);

	return;
error:
	rb_destructor(rbt);
}

static void
_rb_traverse(red_black_tree_node cursor, void (*fn)(const void *key))
{
	if(!cursor) return;

	_rb_traverse(cursor->link[0], fn);
	fn(cursor->key);
	_rb_traverse(cursor->link[1], fn);
}

int
rb_assert(red_black_tree rbt) {
	return _rb_assert(rbt, rbt->root);
}

int
_rb_assert(red_black_tree rbt, red_black_tree_node root) {
    int lh, rh;

    if (root == NULL)
    {
        return 1;
    }
    else
    {
        red_black_tree_node ln = root->link[0];
        red_black_tree_node rn = root->link[1];

        /* Consecutive red links */
        if (_rb_is_node_red(root))
        {
            if (_rb_is_node_red(ln) || _rb_is_node_red(rn))
            {
                puts("Red violation");
                return 0;
            }
        }

        lh = _rb_assert(rbt, ln);
        rh = _rb_assert(rbt, rn);

        /* Invalid binary search tree */
	int ln_root_cmp = ln ? rbt->cmp(ln->key, root->key) : 0;
	int rn_root_cmp = rn ? rbt->cmp(rn->key, root->key) : 0;
        if ((ln != NULL && ln_root_cmp >= 0) || (rn != NULL && rn_root_cmp <= 0))
        {
        	puts("Binary tree violation");
        	return 0;
        }

        /* Black height mismatch */
        if (lh != 0 && rh != 0 && lh != rh)
        {
            puts("Black violation");
            return 0;
        }

        /* Only count black links */
        if (lh != 0 && rh != 0)
        {
            return _rb_is_node_red(root) ? lh : lh + 1;
        }
        else
        {
            return 0;
        }
    }
}

void
rb_destructor(red_black_tree rbt)
{
	if (!rbt) return;

	_rb_destructor(rbt->root);
	if (rbt) free(rbt);
	rbt = NULL;
}

static void
_rb_destructor(red_black_tree_node cursor)
{
	if (!cursor) return;

	_rb_destructor(cursor->link[0]);
	_rb_destructor(cursor->link[1]);
	if (cursor) free(cursor);
	cursor = NULL;
}