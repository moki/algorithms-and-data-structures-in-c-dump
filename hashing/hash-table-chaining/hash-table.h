/**
 * Generic Hash Table
 *
 * Jun 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#ifndef HASHTABLE
#define HASHTABLE 1

#ifndef STDLIB
#define STDLIB 1
#include <stdlib.h>
#endif

#ifndef STDINT
#define STDINT 1
#include <stdint.h>
#endif

#ifndef uint32_t
#define uint32_t unsigned int
#endif

#ifndef NULL
#define NULL ((void *) 0)
#endif

struct hash_bucket {
	void *key;
	void *value;
	struct hash_bucket *next;
};

typedef struct hash_table {
	uint32_t 	   (*hash_function)(void *key);
	int 	     	   (*equal_function)(void *key_a, void *key_b);
	struct hash_bucket **table;
	uint32_t 	   size;
} hash_table;

extern hash_table *
hash_table_constructor(
	uint32_t size,
	uint32_t (*hash_function)(void *key),
	int (*equal_function)(void *a, void *b));

extern void hash_table_insert(hash_table *table, void *key, void *value);

extern void *hash_table_search(hash_table *table, void *key);

extern void hash_table_delete(hash_table *table, void *key);

extern void hash_table_destructor(hash_table *table);

#endif