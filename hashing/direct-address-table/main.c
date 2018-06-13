#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "direct-address-table.h"

/* Maximum value in the unviverse of keys [0, ..., M) */
#define M 10

static void _display_int(void *);
static void direct_access_table_int_test(void);

int main (int argc, char **argv) {
	direct_access_table_int_test();
}

static void direct_access_table_int_test(void) {
	da_table da_int_table = da_table_constructor(M, sizeof(int));

	int *xs = malloc(sizeof(int) * M);

	int i;
	for (i = 0; i < M; i++)
		*(xs + i) = i * i;

	for (i = 0; i < M; i++)
		da_table_insert(da_int_table, i, xs + i);

	for (i = 0; i < M; i++)
		_display_int(da_table_search(da_int_table, i));

	putchar('\n');

	free(xs);

	da_table_destructor(da_int_table);

}

static void _display_int(void *data) {
	printf("%d ", *(int *) data);
}