/**
 * Generic Splay Tree
 *
 * May 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include <stddef.h>

typedef struct _splay_tree *splay_tree;

typedef struct _splay_tree_node *splay_tree_node;

splay_tree
splay_tree_constructor(int (*)(const void *, const void *));

void
splay_tree_insert(const splay_tree, const void *);

void
splay_tree_splay(splay_tree, splay_tree_node);

void
splay_tree_right_rotation(splay_tree_node);

void
splay_tree_left_rotation(splay_tree_node);

void
splay_tree_traverse(const splay_tree, void (*)(const void *));

splay_tree_node
splay_tree_search(const splay_tree, const void *);

void
splay_tree_apply_fn_to_node(splay_tree,
				    splay_tree_node node, void (*)(const void *));


splay_tree_node
splay_tree_minimum(splay_tree);

splay_tree_node
splay_tree_maximum(splay_tree);

splay_tree_node
splay_tree_successor(splay_tree_node);

splay_tree_node
splay_tree_predecessor(splay_tree_node);

void
splay_tree_transplant(splay_tree, splay_tree_node, splay_tree_node);

void
_splay_tree_delete(const splay_tree, splay_tree_node);

void
splay_tree_delete(const splay_tree, void *);

void
splay_tree_destructor(splay_tree);