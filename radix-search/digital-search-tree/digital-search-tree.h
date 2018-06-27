#ifndef DST_H
#define DST_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "string.h"

typedef struct _dst 	 *dst_t;

typedef struct _dst_node *dst_node_t;

typedef void (dst_destroy_key_fn_t)(void *key);

typedef void (dst_destroy_value_fn_t)(void *value);

typedef size_t (dst_length_key_fn_t)(const void *key);

extern dst_t dst_constructor(
	const void 		*default_value,
	dst_destroy_key_fn_t	*destroy_key_fn,
	dst_destroy_value_fn_t	*destroy_value_fn,
	dst_length_key_fn_t	*length_key_fn
);

extern void  dst_destructor(dst_t tree);

extern void *dst_lookup(dst_t tree, const void *key);

extern void  dst_delete(dst_t tree, const void *key);

extern void  dst_insert(dst_t tree, const void *key, const void *value);

#endif