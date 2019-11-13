#include "hash-table.h"

struct hf_options {
	void *option;
};

struct hash_table {
	hash_bucket_t **table;
	key_len_fn_t	key_len_fn;
	hash_fn_t	hash_fn;
	uint32_t 	size;
};

struct hash_bucket {
	hash_bucket_t *next;
	void          *key;
	void 	      *val;
};

hash_table_t *hash_table_constructor(uint32_t size, hash_fn_t hash_fn, key_len_fn_t key_len_fn) {
	if (!size) return NULL;
	if (!hash_fn) return NULL;
	if (!key_len_fn) return NULL;
	hash_table_t *hash_table = malloc(sizeof(hash_table_t));
	if (!hash_table) return NULL;

	hash_table->key_len_fn = key_len_fn;
	hash_table->hash_fn    = hash_fn;
	hash_table->size       = size;

	hash_table->table = malloc(hash_table->size * sizeof(hash_bucket_t *));
	if (!hash_table->table) {
		free(hash_table);
		return NULL;
	}

	uint32_t i;

	for (i = 0; i < hash_table->size; i++)
		hash_table->table[i] = NULL;

	return hash_table;
}

void hash_table_destructor(hash_table_t *hash_table) {
	if (!hash_table) return ;
	if (!hash_table->table) goto adt_free;

	uint32_t i;
	hash_bucket_t *cursor, *_cursor;

	for (i = 0; i < hash_table->size; i++) {
		cursor = hash_table->table[i];

		while (cursor) {
			_cursor = cursor->next;
			free(cursor);
			cursor = _cursor;
		}
	}

	free(hash_table->table);

adt_free:
	free(hash_table);
}

// void *hash_table_find(hash_table_t *hash_table, void *key) {
// 	if (!hash_table) return NULL;
// 	uint32_t hash = hash_table->hash_fn(key, hash_table->key_len_fn(key)) & (hash_table->size - 1);

// 	hash_bucket_t *bucket = hash_table->table[hash];

// 	while (bucket && memcmp(bucket->key, key, hash_table->key_len_fn(key)))
// 		bucket = bucket->next;

// 	if (!bucket) return NULL;

// 	return bucket->val;
// }

/* self adjusting hash-table strategy */
void *hash_table_find(hash_table_t *hash_table, void *key) {
	if (!hash_table) return NULL;
	uint32_t hash = hash_table->hash_fn(key, hash_table->key_len_fn(key)) & (hash_table->size - 1);

	hash_bucket_t *bucket, *front, *_bucket;
	bucket = front = hash_table->table[hash];

	while (bucket && memcmp(bucket->key, key, hash_table->key_len_fn(key))) {
		_bucket = bucket;
		bucket  = bucket->next;
	}

	if (!bucket) return NULL;

	if (bucket != front) {
		_bucket->next = bucket->next;
		bucket->next = front;
		hash_table->table[hash] = bucket;
	}

	return bucket->val;
}

void hash_table_insert(hash_table_t *hash_table, void *key, void *val) {
	if (!hash_table) return ;
	uint32_t hash = hash_table->hash_fn(key, hash_table->key_len_fn(key)) & (hash_table->size - 1);

	hash_bucket_t *bucket = hash_table->table[hash];

	hash_table->table[hash] = malloc(sizeof(hash_bucket_t));
	hash_table->table[hash]->next = bucket;
	hash_table->table[hash]->key = key;
	hash_table->table[hash]->val = val;
}

void hash_table_delete(hash_table_t *hash_table, void *key) {
	if (!hash_table) return ;
	uint32_t hash = hash_table->hash_fn(key, hash_table->key_len_fn(key)) & (hash_table->size - 1);

	hash_bucket_t *bucket = hash_table->table[hash];

	if (!bucket) return ;

	if (!memcmp(bucket->key, key, hash_table->key_len_fn(key))) {
		hash_table->table[hash] = bucket->next;
		free(bucket);
		return;
	}

	while (bucket->next && memcmp(bucket->next->key, key, hash_table->key_len_fn(key)))
		bucket = bucket->next;

	if (!bucket) return ;

	hash_bucket_t *_bucket = bucket->next;
	bucket->next = _bucket->next;

	free(_bucket);
}