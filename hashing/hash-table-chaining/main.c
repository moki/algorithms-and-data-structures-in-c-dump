#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include "hash-table.h"
#include "uint32-djb2-hash.h"
#include "_random.h"

#define N 1000

// 0000 0000 0111 1100 124
// 0000 0000 0111 1101 125
// 0000 0000 0111 1110 126
// 				     | 1111 1111 - (2^8  - 1) 1st byte
// 			   1111 1111 | 1111 1111 - (2^16 - 1) 2nd byte
// 	     | 1111 1111 | 1111 1111 | 1111 1111 - (2^24 - 1) 3rd byte
// 1111 1111 | 1111 1111 | 1111 1111 | 1111 1111 - (2^32 - 1) 4th byte
// 2^32 - 1    2^24 - 1    2^16 - 1    2^8 - 1

// iterating over bytes blueprint.
// static unsigned toy_hash_fn(unsigned *key) {
// 	const unsigned char *k = (const unsigned char *) key;
// 	unsigned v = 0;
// 	unsigned i;
// 	unsigned j;

// 	for (i = 0, j = 0; i < sizeof(unsigned); i++, j+= 8) {
// 		v += k[i] << j;
// 	}

// 	// printf("%d\n", v += k[0]);
// 	// printf("%d\n", v += k[1] << 8);
// 	printf("%u\n", v);
// 	return v;
// }

static void test_hash_fn(void);
static void test_hash_table(void);

static int uint32_equal_function(void *a, void *b);

int main(int argc, char **argv) {
	test_hash_fn();
	test_hash_table();
}

static void test_hash_table(void) {
	// Hash Table size, use power of 2 (hash function requirement)
	uint32_t m = 1U << 8;

	// Amount of items to test against
	uint32_t n = 1000;

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

	hash_table *table = hash_table_constructor(m, uint32_t_djb2_hash, uint32_equal_function);

	// Perform n insertions to the hash table
	for (i = 0; i < n; i++)
		hash_table_insert(table, ys + i, xs + i);

	// Perform n lookups in the hash table
	for (i = 0; i < n; i++)
		assert(*(uint32_t *) hash_table_search(table, ys + i) == *(xs + *(ys + i)));

	// delete entries with even keys
	for (i = 0; i < n; i = i + 2)
		hash_table_delete(table, ys + i);

	// assert absence of even keyed entries
	for (i = 0; i < n; i = i + 2)
		assert(hash_table_search(table, ys + i) == NULL);

	// assert presence of odd keyed entries
	for (i = 1; i < n; i = i + 2)
		// assert(hash_table_search(table, ys + i) != NULL);
		assert(*(uint32_t *) hash_table_search(table, ys + i) == *(xs + *(ys + i)));

	free(xs);
	free(ys);
	hash_table_destructor(table);
}

static void test_hash_fn(void) {
	uint32_t i;
	uint32_t m = 1U << 4;
	uint32_t xs[m];

	for (i = 0; i < m; i++) {
		xs[i] = 0;
	}

	for (i = 0; i < N; i++) {
		uint32_t hash = uint32_t_djb2_hash(&i);
		xs[hash & (m - 1)]++;
	}

	for (i = 0; i < m; i++)
		assert(xs[i] <= (N/m + 1));
}

static int uint32_equal_function(void *a, void *b) {
	return (*(uint32_t *) a - *(uint32_t *) b) == 0;
}