#include <stdio.h>
#include <assert.h>
#include "hash-table.h"
#include "uint32_t-djb2-hash.h"
#include "uint32_t-sdbm-hash.h"
#include "_random.h"

static void test_hash_table(void);
static int uint32_equal_function(void *a, void *b);

int main(int argc, char **argv) {
	// printf("hash table open addressing\n");

	// hash_table *table = hash_table_constructor(10, uint32_t_djb2_hash, uint32_t_sdbm_hash, uint32_equal_function);

	// int *x = malloc(sizeof(int));
	// int *y = malloc(sizeof(int));

	// *x = 3;
	// *y = 8;

	// hash_table_insert(table, x, y);

	// *x = 5;
	// *y = 15;

	// hash_table_insert(table, x, y);

	// *x = 99;
	// *y = 10;

	// hash_table_insert(table, x, y);

	// free(x);
	// free(y);
	// hash_table_destructor(table);

	test_hash_table();
}

static void test_hash_table(void) {
	// Hash Table size, use power of 2 (hash function requirement)
	uint32_t m = 1U << 5;

	// Amount of items to test against
	uint32_t n = 10;

	// Generate numbers to test table against
	uint32_t *xs = malloc(sizeof(uint32_t) * n);
	uint32_t *ys = malloc(sizeof(uint32_t) * n);
	uint32_t i;

	// random values
	for (i = 0; i < n; i++)
		*(xs + i) = _random(1000);

	// keys
	for (i = 0; i < n; i++)
		*(ys + i) = i;

	hash_table *table = hash_table_constructor(m, uint32_t_djb2_hash, uint32_t_sdbm_hash, uint32_equal_function);

	// Perform n insertions to the hash table
	for (i = 0; i < n; i++)
		hash_table_insert(table, ys + i, xs + i);

	// Perform n lookups in the hash table
	for (i = 0; i < n; i++)
		assert(*(uint32_t *) hash_table_search(table, ys + i) == *(xs + *(ys + i)));

	// delete entries with even keys
	// for (i = 0; i < n; i = i + 2)
	// 	hash_table_delete(table, ys + i);

	// assert absence of even keyed entries
	// for (i = 0; i < n; i = i + 2)
	// 	assert(hash_table_search(table, ys + i) == NULL);

	// assert presence of odd keyed entries
	for (i = 1; i < n; i = i + 2)
		// assert(hash_table_search(table, ys + i) != NULL);
		assert(*(uint32_t *) hash_table_search(table, ys + i) == *(xs + *(ys + i)));

	free(xs);
	free(ys);
	hash_table_destructor(table);
}

static int uint32_equal_function(void *a, void *b) {
	return (*(uint32_t *) a - *(uint32_t *) b) == 0;
}