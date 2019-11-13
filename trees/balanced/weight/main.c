#include "wbt.h"
#include <assert.h>

static size_t key_length_fn(const void *key);

int main(int argc, char **argv) {
	double epsilon = 0.005;
	double alpha = 0.288;

	wbt_t *tree = wbt_constructor(key_length_fn, &alpha, &epsilon);
	if (!tree) return -1;
	int i;
	int *ks = malloc(sizeof(int *) * 10);

	for (i = 0; i < 10; i++)
		*(ks + i) = i + 1;

	int *vs = malloc(sizeof(int *) * 10);

	for (i = 0; i < 10; i++)
		*(vs + i) = (i + 1) * 10;

	for (i = 0; i < 10; i++)
		wbt_insert(tree, ks + i, vs + i);

	for (i = 0; i < 10; i++)
		assert((void *) (vs + i) == wbt_find(tree, ks+i));

	for (i = 0; i < 10; i += 2)
		wbt_delete(tree, (ks + i));

	FILE *f;
	f = fopen("wbt.gv", "w+");
	if (!f) return -1;

	wbt_draw(tree, f);
	fclose(f);

	free(ks);
	free(vs);
	wbt_destructor(tree);
}

static size_t key_length_fn(const void *key) {
	return sizeof(key);
}
