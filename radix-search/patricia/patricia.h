#ifndef PATRICIA_H
#define PATRICIA_H

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "item.h"
#include "string.h"

typedef struct patricia      *patricia_t;
typedef struct patricia_node *patricia_node_t;

typedef void   (patricia_destroy_key_fn_t)(void *key);

typedef void   (patricia_destroy_value_fn_t)(void *value);

typedef size_t (patricia_length_key_fn_t)(const void *key);

extern patricia_t patricia_constructor(
	const void *default_value,
	patricia_destroy_key_fn_t,
	patricia_destroy_value_fn_t,
	patricia_length_key_fn_t
);

extern void patricia_destructor(patricia_t trie);

extern void patricia_insert(patricia_t trie, const void *key, const void *value);

extern void patricia_delete();

extern void *patricia_lookup();

/* Gets a byte or returns zero if it's out of range.
 * Copes with negative indices. */
#define GET_BYTE(KEY, KEYEND, INDEX) \
  (((size_t) (INDEX) < (size_t) ((KEYEND) - (KEY))) ? (KEY)[INDEX] : 0)

/* Extract the specified indexed binary direction from the key. */
#define GET_DIR(KEY, KEYEND, INDEX) \
  (KEY ? (GET_BYTE(KEY, KEYEND, INDEX >> 3) & (1 << (7 - ((INDEX) & 7)))) != 0 : 0)

#endif