/**
 * Generic Hash Table
 *
 * Jun 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include "hash-table.h"
#include <stdio.h>

static void _free(void *__ptr);
static uint32_t double_hash(hash_table *table, void *key, uint32_t probe);
static void display_raw(hash_table *table);

hash_table *
hash_table_constructor(
	uint32_t size,
	uint32_t (*hash_function_a)(void *key),
	uint32_t (*hash_function_b)(void *key),
	int (*equal_function)(void *a, void *b))
{
	hash_table *table = malloc(sizeof(hash_table));
	if (!table) return NULL;

	table->hash_function_a = hash_function_a;
	table->hash_function_b = hash_function_b;

	table->size = size;
	table->filled = 0;
	table->table = calloc(table->size, sizeof(struct hash_bucket));
	if (!table->table) return NULL;

	table->null_sentinel = malloc(sizeof(struct hash_bucket));
	if (!table->null_sentinel) return NULL;

	table->null_sentinel->key = NULL;
	table->null_sentinel->value = NULL;

	return table;
}

void hash_table_insert(hash_table *table, void *key, void *value) {
	struct hash_bucket *cursor = table->table[double_hash(table, key, 0)];

	uint32_t i = 0;
	for (i = 1; cursor && cursor != table->null_sentinel; i++) {
		cursor = table->table[double_hash(table, key, i)];
	}

	struct hash_bucket *bucket = malloc(sizeof(struct hash_bucket));
	if (!bucket) return;

	bucket->key = key;
	bucket->value = value;

	printf("insert at index: %u, bucket k: %u, v: %u\n", double_hash(table, key, i - 1), *(uint32_t *) bucket->key, *(uint32_t *) bucket->value);
	table->table[double_hash(table, key, i - 1)] = bucket;
	table->filled++;

	display_raw(table);
}

void *hash_table_search(hash_table *table, void *key) {
	if (!table) return NULL;
	struct hash_bucket *cursor = table->table[double_hash(table, key, 0)];
	uint32_t i;
	for (i = 1; cursor; cursor = table->table[double_hash(table, key, i)], i++) {

		printf("compare k:%u k:%u, cursor->v: %u\n", *(uint32_t *) cursor->key, *(uint32_t *) key, *(uint32_t *) cursor->value);
		if (cursor != table->null_sentinel && table->equal_function(cursor->key, key))
			return cursor->value;
	}

	return NULL;
}


void hash_table_delete(hash_table *table, void *key) {
	uint32_t i = double_hash(table, key, 0);
	struct hash_bucket *cursor = table->table[i];

	uint32_t j;
	for (j = 1; cursor; i = double_hash(table, key, j), cursor = table->table[i], j++) {
		if (cursor != table->null_sentinel && table->equal_function(cursor->key, key)) {
			_free(cursor);
			table->table[i] = table->null_sentinel;
		}
	}

	table->filled--;
}

void hash_table_destructor(hash_table *table) {
	uint32_t i;
	for (i = 0; i < table->size; i++)
		_free(table->table[i]);

	_free(table->table);
	_free(table->null_sentinel);
	_free(table);
}

static void _free(void *__ptr) {
	free(__ptr);
	__ptr = NULL;
}

static uint32_t double_hash(hash_table *table, void *key, uint32_t probe) {
	return (table->hash_function_a(key) + (probe * (table->hash_function_b(key) + 1))) & (table->size - 1);
}

static void display_raw(hash_table *table) {
	putchar('\n');
	uint32_t i;
	for (i = 0; i < table->size; i++)
		printf("%u ", table->table[i] ? *(uint32_t *) table->table[i]->value : 0);
	putchar('\n');
}