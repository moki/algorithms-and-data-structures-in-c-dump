/**
 * Generic Direct Address Table
 *
 * Jun 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include "direct-address-table.h"
#include <string.h>
#include <stdio.h>

struct _da_table {
	size_t 	type_size;
	size_t 	universe;
	void 	*table;

};

static int _da_is_key_valid(da_table, size_t);

da_table da_table_constructor(size_t m, size_t s) {
	if (!s) return NULL;
	if (!m) return NULL;
	da_table table = malloc(sizeof(*table));
	table->table = malloc(s * m);
	table->type_size = s;
	table->universe  = m;

	return table;
}

void *da_table_search(da_table table, int key) {
	if (!table) 				return NULL;
	if (!table->table) 			return NULL;
	if (!_da_is_key_valid(table, key))  	return NULL;

	return ((char *) table->table) + key * table->type_size;
}

void da_table_insert(da_table table, int key, void *data) {
	if (!table) 		 		return;
	if (!_da_is_key_valid(table, key))  	return;

	memcpy(((char *) table->table) + key * table->type_size, data, table->type_size);
}

void da_table_delete(da_table table, int key) {
	if (!table) 				return;
	if (!table->table) 			return;
	if (!_da_is_key_valid(table, key))  	return;

	memset(((char *) table->table) + key * table->type_size, 0, table->type_size);
}

void da_table_destructor(da_table table) {
	if (table){
		if (table->table)
			free(table->table);
		free(table);
	}
}

static int _da_is_key_valid(da_table table, size_t key) {
	return key >= 0 && key < table->universe;
}