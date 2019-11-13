#include "trie.h"

struct trie_node {
	trie_node_t *next[16];
	void 	    *val;
	int 	     refs;
};

trie_node_t *trie_constructor() {
	trie_node_t *trie = malloc(sizeof(*trie));
	if (!trie) return NULL;

	uint32_t i;
	for (i = 0; i < 16; i++)
		trie->next[i] = NULL;

	trie->refs = 1;
	trie->val  = NULL;

	return trie;
}

void trie_destructor(trie_node_t *trie) {
	if (!trie) return;

	trie_node_t *cursor = trie;
	stack_t *path = stack_constructor(NULL);

	stack_push(path, cursor);

	while (!stack_is_empty(path)) {
		uint32_t i;
		cursor = stack_pop(path);

		for (i = 0; i < 16; i++) {
			if (cursor->next[i])
				stack_push(path, cursor->next[i]);
		}

		free(cursor);
	}

	stack_destructor(path);
}

void *trie_find(trie_node_t *trie, char *key, key_length_fn_t key_length_fn) {
	if (!trie) return NULL;

	trie_node_t *cursor = trie;
	trie_node_t *_cursor;

	uint32_t i;
	size_t key_len = key_length_fn(key);

	for (i = 0; i < key_len; i++) {
		unsigned char msfour = ((((unsigned char) key[i]) & 0xF0) >> 4);
		_cursor = cursor->next[msfour];

		if (_cursor)
			cursor = _cursor;
		else
			return NULL;

		unsigned char lsfour = ((((unsigned char) key[i]) & 0x0F));
		_cursor = cursor->next[lsfour];

		if (_cursor)
			cursor = _cursor;
		else
			return NULL;
	}

	return cursor->val;
}

void trie_insert(trie_node_t *trie, char *key, char *val, key_length_fn_t key_length_fn) {
	if (!trie) return;

	trie_node_t *cursor = trie;
	trie_node_t *_cursor;

	uint32_t i;
	unsigned char c;
	size_t key_len = key_length_fn(key);

	for (i = 0; i < 2 * key_len; i++) {
		if (i & 1)
			c = ((unsigned char) key[i / 2]) & 0x0F;
		else
			c = (((unsigned char) key[i / 2]) & 0xF0) >> 4;

		_cursor = cursor->next[c];

		if (_cursor) {
			cursor = _cursor;
			continue;
		}

		uint32_t j;
		_cursor = malloc(sizeof(*_cursor));

		for (j = 0; j < 16; j++)
			_cursor->next[j] = NULL;

		_cursor->val = NULL;
		_cursor->refs = 0;

		cursor->next[c] = _cursor;
		cursor->refs++;
		cursor = _cursor;
	}

	if (cursor->val)
		return;

	cursor->val = val;
	cursor->refs++;
}

void trie_delete(trie_node_t *trie, char *key, key_length_fn_t key_length_fn) {
	if (!trie) return;

	trie_node_t *cursor, *_cursor, *dcursor;

	uint32_t i, j;
	unsigned char c;

	size_t key_len = key_length_fn(key);

	cursor = dcursor = trie;
	j = 0;

	for (i = 0; i < 2 * key_len; i++) {
		if (i & 1)
			c = ((unsigned char) key[i / 2]) & 0x0F;
		else
			c = (((unsigned char) key[i / 2]) & 0xF0) >> 4;

		_cursor = cursor->next[c];

		if (_cursor) {
			if (cursor->refs > 1) {
				dcursor = cursor;
				j = i;
			}

			cursor = _cursor;
		} else
			return;
	}

	if (!cursor->val)
		return;

	cursor->refs--;
	cursor->val = NULL;

	if (cursor->refs)
		return;

	cursor = dcursor;

	for (i = j; i < 2 * key_len; i++) {
		if (i & 1)
			c = ((unsigned char) key[i / 2]) & 0x0F;
		else
			c = (((unsigned char) key[i / 2]) & 0xF0) >> 4;

		_cursor = cursor->next[c];
		cursor->next[c] = NULL;
		cursor->refs--;

		if (!cursor->refs)
			free(cursor);

		cursor = _cursor;
	}

	free(cursor);
}