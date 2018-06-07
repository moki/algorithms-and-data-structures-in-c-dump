/*
 * Generic Red Black Tree
 *
 * June 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include <stddef.h>

typedef struct _red_black_tree *red_black_tree;

typedef struct _red_black_tree_node *red_black_tree_node;

red_black_tree
rb_constructor(int (*)(const void *, const void *));

void
rb_insert(red_black_tree, void *);

void
rb_delete(red_black_tree, void *);

int
rb_assert(red_black_tree rbt);

void
rb_traverse(const red_black_tree rbt, void (*fn)(const void *key));

void
rb_destructor(red_black_tree);