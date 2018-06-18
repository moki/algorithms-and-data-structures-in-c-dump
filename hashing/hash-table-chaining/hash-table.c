/**
 * Generic Hash Table
 *
 * Jun 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include "hash-table.h"
#include <stdio.h>

static void hash_table_traverse(hash_table *table, void (*fn)(void *));
static void _free(void *__ptr);

hash_table *
hash_table_constructor(
	uint32_t size,
	uint32_t (*hash_function)(void *key),
	int (*equal_function)(void *a, void *b))
{
	if (!hash_function || !equal_function) return NULL;

	hash_table *table = malloc(sizeof(hash_table));
	if (!table) return NULL;

	table->table = malloc(sizeof(struct hash_bucket) * size);
	if (!table->table) return NULL;

	uint32_t i;
	for (i = 0; i < size; i++) {
		table->table[i] = NULL;
	}

	table->size = size;
	table->hash_function = hash_function;
	table->equal_function = equal_function;

	return table;
}

void hash_table_insert(hash_table *table, void *key, void *value) {
	if (!table) return;

	uint32_t i = table->hash_function(key) & (table->size - 1);

	struct hash_bucket *bucket = malloc(sizeof(struct hash_bucket));
	if (!bucket) return;

	bucket->next = table->table[i];
	bucket->key = key;
	bucket->value = value;

	table->table[i] = bucket;
}

void *hash_table_search(hash_table *table, void *key) {
	if (!table) return NULL;

	uint32_t i = table->hash_function(key) & (table->size - 1);
	struct hash_bucket *cursor = table->table[i];

	while (cursor) {
		if (table->equal_function(cursor->key, key))
			return cursor->value;
			// printf("lookup key: %u, items key: %u\n", *(uint32_t *) key, *(uint32_t *) cursor->key);

		cursor = cursor->next;
	}

	return NULL;
}

void hash_table_delete(hash_table *table, void *key) {
	if (!table) return;

	uint32_t i = table->hash_function(key) & (table->size - 1);
	struct hash_bucket *cursor = table->table[i];

	if (table->equal_function(cursor->key, key)){
		table->table[i] = cursor->next;
		_free(cursor);
		return;
	}

	struct hash_bucket *cursor_trailing = cursor;

	while (cursor) {
		if (table->equal_function(cursor->key, key)) {
			cursor_trailing->next = cursor->next;
			_free(cursor);
			return;
		}

		cursor_trailing = cursor;
		cursor = cursor->next;
	}
}

void hash_table_destructor(hash_table *table) {

	hash_table_traverse(table, _free);

	_free(table->table);
	_free(table);
}

static void hash_table_traverse(hash_table *table, void (*fn)(void *)) {
	if (!table) return;
	if (!table->table) return;

	uint32_t i;
	for (i = 0; i < table->size; i++) {
		struct hash_bucket *cursor = table->table[i];
		if (!cursor) continue;

		struct hash_bucket *cursor_trailing;

		while (cursor) {
			cursor_trailing = cursor;
			cursor = cursor->next;
			fn(cursor_trailing);
		}
	}
}

static void _free(void *__ptr) {
	free(__ptr);
	__ptr = NULL;
}
