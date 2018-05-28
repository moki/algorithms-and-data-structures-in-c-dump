/*
 * Generic Binary Search Tree
 *
 * May 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include <stddef.h>

typedef struct _binary_search_tree *binary_search_tree;

typedef struct _bst_node *bst_node;

binary_search_tree
binary_search_tree_constructor(int (*)(const void *, const void *));

void
binary_search_tree_insert(const binary_search_tree, const void *);

void
binary_search_tree_traverse(const binary_search_tree, void (*)(const void *));

bst_node
binary_search_tree_search(const binary_search_tree, const void *);

void
binary_search_tree_apply_fn_to_node(binary_search_tree,
				    bst_node node, void (*)(const void *));


bst_node
binary_search_tree_minimum(binary_search_tree);

bst_node
binary_search_tree_maximum(binary_search_tree);

bst_node
binary_search_tree_successor(bst_node);

bst_node
binary_search_tree_predecessor(bst_node);

void
binary_search_tree_transplant(binary_search_tree, bst_node, bst_node);

void
_binary_search_tree_delete(const binary_search_tree, bst_node);

void
binary_search_tree_delete(const binary_search_tree, void *);

void
binary_search_tree_destructor(binary_search_tree);