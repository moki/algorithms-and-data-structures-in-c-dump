#ifndef TRIE_H
#define TRIE_H

#include <stdint.h>
#include <stdlib.h>
#include "item.h"
#include "string.h"

typedef struct _trie *trie_t;
typedef struct _trie_node *trie_node_t;

typedef void   (trie_destroy_key_fn_t)(void *key);

typedef void   (trie_destroy_value_fn_t)(void *value);

typedef size_t (trie_length_key_fn_t)(const void *key);

extern trie_t trie_constructor(
	const void *default_value,
	trie_destroy_key_fn_t,
	trie_destroy_value_fn_t,
	trie_length_key_fn_t
);

extern void trie_destructor(trie_t trie);

extern void trie_insert(trie_t trie, const void *key, const void *value);

extern void trie_delete();

extern void *trie_lookup();

#endif