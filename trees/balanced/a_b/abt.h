#ifndef ABT_H
#define ABT_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lib/stack/stack.h"

typedef struct abt 	abt_t;
typedef struct abt_node abt_node_t;

typedef size_t (*key_length_fn_t)(const void *key);

extern abt_t *abt_constructor(size_t k_s, size_t v_s, uint32_t a, uint32_t b);

extern void abt_destructor(abt_t *tree);

extern void *abt_find(abt_t *tree, void *key);

extern void abt_insert(abt_t *tree, void *key, void *val);

extern void abt_delete(abt_t *tree, void *key);

extern void *abt_get_node_key(abt_node_t *node);

abt_node_t *abt_new_node(abt_t *tree, uint32_t height, uint32_t degree);

extern void *abt_node_get_key(abt_node_t *node);

extern void abt_draw(abt_t *tree, FILE *stream);

#endif