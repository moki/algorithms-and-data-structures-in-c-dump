#include "trie.h"

static trie_node_t *new_trie_node();

struct trie_node {
	trie_node_t *edge[256];
};

trie_node_t *trie_constructor() {
	trie_node_t *trie = new_trie_node();
	if(!trie) return NULL;

	return trie;
}

void trie_destructor(trie_node_t *trie) {
	if (!trie)
		return;

	stack_t *path = stack_constructor(NULL);
	trie_node_t *cursor = trie;

	stack_push(path, cursor);

	while (!stack_is_empty(path)) {
		cursor = stack_pop(path);

		for (int i = 0; i < 256; i++) {
			if (cursor->edge[i] != NULL && i != '\0')
				stack_push(path, cursor->edge[i]);
		}

		free(cursor);
	}

	stack_destructor(path);
}
char *trie_find(trie_node_t *trie, char *key) {
	trie_node_t *cursor;
	char 	    *qcursor;

	for (cursor = trie, qcursor = key; *qcursor != '\0'; qcursor++) {
		if (!cursor->edge[(unsigned char) (*qcursor)])
			return NULL;
		else
			cursor = cursor->edge[(unsigned char) (*qcursor)];
	}

	return (char *) cursor->edge[(unsigned char) '\0'];
}

void trie_insert(trie_node_t *trie, char *key, char *val) {
	trie_node_t *cursor, *new_node;
	char 	    *qcursor;

	for (cursor = trie, qcursor = key; *qcursor != '\0'; qcursor++) {
		if (!cursor->edge[(unsigned char) (*qcursor)]) {
			new_node = new_trie_node();
			if(!new_node) exit(EXIT_FAILURE);

			cursor->edge[(unsigned char) (*qcursor)] = new_node;
		}

		cursor = cursor->edge[(unsigned char) (*qcursor)];
	}

	if (cursor->edge[(unsigned char) '\0'])
		return;
	else
		cursor->edge[(unsigned char) '\0'] = (trie_node_t *) val;
}

void trie_delete(trie_node_t *trie, char *key) {
	trie_node_t *cursor;
	char 	    *qcursor;

	stack_t *path = stack_constructor(NULL);

	for (cursor = trie, qcursor = key; *qcursor != '\0';) {
		if (!cursor->edge[(unsigned char) (*qcursor)])
			return;
		else {
			cursor = cursor->edge[(unsigned char) (*qcursor)];
			qcursor++;
			stack_push(path, cursor);
		}
	}

	unsigned char finished = 0;

	while (!stack_is_empty(path) && !finished) {
		cursor = stack_pop(path);
		cursor->edge[(unsigned char) (*qcursor)] = NULL;

		int i;
		for (i = 0; i < 256; i++)
			finished = finished || (cursor->edge[(unsigned char) i] != NULL);

		if (!finished) {
			free(cursor);
			cursor = NULL;
			qcursor--;
		}
	}

	stack_destructor(path);
}

static trie_node_t *new_trie_node() {
	trie_node_t *node = malloc(sizeof(*node));
	if(!node) return NULL;

	int i;
	for (i = 0; i < 256; i++)
		node->edge[i] = NULL;

	return node;
}