#ifndef BST_H
#define BST_H

#define BST_TRAVERSE_PRE_ORDER (0)
#define BST_TRAVERSE_IN_ORDER (1)
#define BST_TRAVERSE_POST_ORDER (2)
#define BST_TRAVERSE_BREADTH_FIRST (3)

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lib/queue/queue.h"
#include "lib/stack/stack.h"

typedef struct bst	bst_t;
typedef struct bst_node bst_node_t;

/* interface to implement key and value destroy/free functions
 * if they were allocated dynamically
 */
typedef void   (*key_destroy_fn_t)(const void *key);
typedef void   (*val_destroy_fn_t)(const void *val);

/* interface to implement key and value length/width/size functions
 * which retuns amount of bytes in key/val
 */
typedef size_t (*key_length_fn_t)(const void *key);
typedef size_t (*val_length_fn_t)(const void *val);

extern bst_t *bst_constructor(
	key_destroy_fn_t key_destroy_fn,
	val_destroy_fn_t val_destroy_fn,
	key_length_fn_t key_length_fn,
	val_length_fn_t val_length_fn);

extern bst_t *bst_constructor_list_bottom_up(
	bst_node_t *node_list,
	key_destroy_fn_t key_destroy_fn,
	val_destroy_fn_t val_destroy_fn,
	key_length_fn_t key_length_fn,
	val_length_fn_t val_length_fn);

// extern bst_t *bst_constructor_list_top_to_bottom(
// 	bst_node_t *node_list,
// 	key_destroy_fn_t key_destroy_fn,
// 	val_destroy_fn_t val_destroy_fn,
// 	key_length_fn_t key_length_fn,
// 	val_length_fn_t val_length_fn);

extern void bst_destructor(bst_t *bst);

extern bst_node_t *bst_flatten(bst_t *bst);

extern void *bst_find(bst_t *bst, void *key);

extern bst_node_t *bst_find_interval(bst_t *bst, void *key_a, void *key_b);

extern void bst_insert(bst_t *bst, void *key, void *val);

extern void bst_delete(bst_t *bst, void *key);

extern void bst_traverse(bst_t *bst, unsigned char mode, void (*fn)(bst_node_t *node));

extern void bst_traverse_list(bst_node_t *cursor, void (*fn)(bst_node_t *node));

extern void *bst_get_node_key(bst_node_t *node);

extern void bst_draw(bst_t *tree, FILE *stream);

extern bst_node_t *bst_new_node(void *key, void *left, void *right);

#endif