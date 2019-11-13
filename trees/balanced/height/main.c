#include "hbt.h"
#include <assert.h>

static size_t key_length_fn(const void *key);

int main(int argc, char **argv) {
	hbt_t *tree = hbt_constructor(key_length_fn);
	if (!tree) return -1;
	int i;
	int *ks = malloc(sizeof(int *) * 10);

	for (i = 0; i < 10; i++)
		*(ks + i) = i + 1;

	int *vs = malloc(sizeof(int *) * 10);

	for (i = 0; i < 10; i++)
		*(vs + i) = (i + 1) * 10;

	for (i = 0; i < 10; i++)
		hbt_insert(tree, ks + i, vs + i);

	for (i = 0; i < 10; i++)
		assert((void *) (vs + i) == hbt_find(tree, ks+i));

	for (i = 0; i < 10; i += 2)
		hbt_delete(tree, (ks + i));

	FILE *f;
	f = fopen("hbt.gv", "w+");
	if (!f) return -1;

	hbt_draw(tree, f);
	fclose(f);

	free(ks);
	free(vs);
	hbt_destructor(tree);
}

static size_t key_length_fn(const void *key) {
	return sizeof(key);
}

