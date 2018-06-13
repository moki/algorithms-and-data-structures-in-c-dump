/**
 * Generic Direct Address Table
 *
 * Jun 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include <stdlib.h>

typedef struct _da_table *da_table;

da_table da_table_constructor(size_t m, size_t s);

void *da_table_search(da_table table, int key);

void da_table_insert(da_table table, int key, void *data);

void da_table_delete(da_table table, int key);

void da_table_destructor(da_table table);