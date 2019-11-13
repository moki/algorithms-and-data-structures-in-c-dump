#include "hash-table.h"
#include "lib/djb2/djb2.h"
#include "lib/_random/_random.h"
#include <assert.h>

int main(int argc, char **argv) {
	/* generating test string keys */
	uint32_t n = 1030;  // keys/vals amount
	uint32_t m = 100; // keys length

	char *ks[n];

	uint32_t i;
	uint32_t j;
	for (i = 0; i < n; i++) {
		ks[i] = malloc(sizeof(char) * m);

		for (j = 0; j < m - 1; j++)
			*(ks[i] + j) = (char) _random(65, 122);

		*(ks[i] + j) = '\0';
	}

	/* displaying them */
        for (uint32_t i = 0; i < n; i++)
          printf("%s\n", ks[i]);

        /* generating test uint32_t values */
	uint32_t vs[n];
	for (i = 0; i < n; i++)
		vs[i] = _random(0, UINT_MAX - 1);

	/* displaying them */
        printf("test uint32_t values\n");
        for (i = 0; i < n; i++)
          printf("%u\n", vs[i]);

        hash_table_t *hash_table = hash_table_constructor(1 << 7, djb2, (key_len_fn_t) strlen);

	for (i = 0; i < n; i++)
		hash_table_insert(hash_table, ks[i], &vs[i]);

	for (i = 0; i < n; i++)
		assert(hash_table_find(hash_table, ks[i]) == &vs[i]);

	for (i = 0; i < n; i += 2)
		hash_table_delete(hash_table, ks[i]);

	for (i = 0; i < n; i += 2) {
		assert(!hash_table_find(hash_table, ks[i]));
	}

	hash_table_destructor(hash_table);

	/* keys cleanup */
	for (i = 0; i < n; i++) {
		free(ks[i]);
	}
}
