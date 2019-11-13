#ifndef WBT_H
#define WBT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define WBT_ALPHA 0.288
#define WBT_EPSILON 0.005

#include "lib/stack/stack.h"

typedef struct wbt 	wbt_t;
typedef struct wbt_node wbt_node_t;

typedef size_t (*key_length_fn_t)(const void *key);

extern wbt_t *wbt_constructor(key_length_fn_t key_length_fn, void *alpha, void *epsilon);

extern void wbt_destructor(wbt_t *wbt);

extern void *wbt_find(wbt_t *wbt, void *key);

extern void wbt_insert(wbt_t *wbt, void *key, void *val);

extern void wbt_delete(wbt_t *wbt, void *key);

extern void *wbt_get_node_key(wbt_node_t *node);

extern wbt_node_t *wbt_new_node(void *key, void *left, void *right, uint32_t weight);

extern void *wbt_node_get_key(wbt_node_t *node);

extern void wbt_draw(wbt_t *tree, FILE *stream);

#endif