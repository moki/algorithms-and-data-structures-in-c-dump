#ifndef HBT_H
#define HBT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lib/stack/stack.h"

typedef struct hbt 	hbt_t;
typedef struct hbt_node hbt_node_t;

typedef size_t (*key_length_fn_t)(const void *key);

extern hbt_t *hbt_constructor(key_length_fn_t key_length_fn);

extern void hbt_destructor(hbt_t *hbt);

extern void *hbt_find(hbt_t *hbt, void *key);

extern void hbt_insert(hbt_t *hbt, void *key, void *val);

extern void hbt_delete(hbt_t *hbt, void *key);

extern void *hbt_get_node_key(hbt_node_t *node);

extern hbt_node_t *hbt_new_node(void *key, void *left, void *right, unsigned char height);

extern void hbt_draw(hbt_t *tree, FILE *stream);

#endif