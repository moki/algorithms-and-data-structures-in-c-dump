#ifndef TRIE_H
#define TRIE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lib/stack/stack.h"

typedef struct trie_node trie_node_t;

extern trie_node_t *trie_constructor();

extern void trie_destructor(trie_node_t *trie);

extern char *trie_find(trie_node_t *trie, char *key);

extern void trie_insert(trie_node_t *trie, char *key, char *val);

extern void trie_delete(trie_node_t *trie, char *key);

#endif