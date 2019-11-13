#ifndef TRIE_H
#define TRIE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lib/stack/stack.h"

typedef size_t (*key_length_fn_t)(const void *key);

typedef struct trie_node trie_node_t;

extern trie_node_t *trie_constructor();

extern void trie_destructor(trie_node_t *trie);

extern void *trie_find(trie_node_t *trie, char *key, key_length_fn_t key_length_fn);

extern void trie_insert(trie_node_t *trie, char *key, char *val, key_length_fn_t key_length_fn);

extern void trie_delete(trie_node_t *trie, char *key, key_length_fn_t key_length_fn);

#endif