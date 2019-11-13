#ifndef RBT_H
#define RBT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lib/stack/stack.h"

#define RBT_TRAVERSE_POST_ORDER 	(0u << 0)

#define RBT_REBALANCE_MODE_BOTTOM_UP   (0u << 0)
#define RBT_REBALANCE_MODE_TOP_DOWN    (1u << 0)

typedef struct rbt 	rbt_t;
typedef struct rbt_node rbt_node_t;

typedef void (*key_destroy_fn_t)(void *key);
typedef void (*val_destroy_fn_t)(void *key);

extern rbt_t *rbt_constructor(size_t k_s, size_t v_s, void *key_destroy_fn, void *val_destroy_fn, unsigned char rebalance_mode);

extern void rbt_destructor(rbt_t *tree);

extern void *rbt_find(rbt_t *tree, void *key);

extern void rbt_insert(rbt_t *tree, void *key, void *val);

extern void rbt_delete(rbt_t *tree, void *key);

extern void rbt_traverse(rbt_t *tree, unsigned char mode, void (*ts_fn)(rbt_t *tree, rbt_node_t *node));

extern void rbt_draw(rbt_t *tree, FILE *stream);

#endif