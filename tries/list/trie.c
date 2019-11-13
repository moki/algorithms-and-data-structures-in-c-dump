#include "trie.h"

struct trie_node {
	trie_node_t *next;
	trie_node_t *list;
	char ch;
};

trie_node_t *trie_constructor() {
	trie_node_t *trie = malloc(sizeof(*trie));

	trie->next = trie->list = NULL;
	trie->ch = '\0';

	return trie;
}

void trie_destructor(trie_node_t *trie) {
	if (!trie) return;
	trie_node_t *cursor = trie;

	stack_t *path = stack_constructor(NULL);

	stack_push(path, cursor);

	while (!stack_is_empty(path)) {
		cursor = stack_pop(path);

		if (cursor->ch != '\0')
			stack_push(path, cursor->next);
		if (cursor->list)
			stack_push(path, cursor->list);

		free(cursor);
	}

	stack_destructor(path);
}

char *trie_find(trie_node_t *trie, char *key) {
	trie_node_t *cursor  = trie;
	char	    *qcursor = key;

	while (*qcursor != '\0') {
		while (cursor->ch != *qcursor) {
			if (!cursor->list)
				return NULL;
			else
				cursor = cursor->list;
		}

		cursor = cursor->next;
		qcursor++;
	}

	while (cursor->ch != '\0') {
		if (!cursor->list)
			return NULL;
		else
			cursor = cursor->list;
	}

	return (char *) cursor->next;
}

void trie_insert(trie_node_t *trie, char *key, char *val) {
	trie_node_t *cursor  = trie;
	char	    *qcursor = key;

	unsigned char finished = 0;

	while (!finished) {
		while (cursor->ch != *qcursor && cursor->list)
			cursor = cursor->list;

		if (cursor->ch == *qcursor) {
			if (*qcursor != '\0') {
				cursor = cursor->next;
				qcursor++;
			} else {
				return;
			}
		} else {
			finished = 1;
		}
	}

	cursor->list = malloc(sizeof(*cursor->list));
	cursor = cursor->list;
	cursor->list = NULL;
	cursor->ch = *qcursor;

	while (*qcursor != '\0') {
		qcursor++;

		cursor->next = malloc(sizeof(*cursor->next));
		cursor = cursor->next;
		cursor->list = NULL;
		cursor->ch = *qcursor;
	}

	cursor->next = (trie_node_t *) val;
}

void trie_delete(trie_node_t *trie, char *key) {
	trie_node_t *cursor, *_cursor, *first, *last;

	char *qcursor;

	if (!trie->list || *key == '\0')
		return;

	unsigned char finished = 0;
	unsigned char branch = 1;

	last = _cursor = trie;

	first = cursor = trie->list;

	qcursor = key;

	while (!finished) {
		while (cursor->ch != *qcursor) {
			if (!cursor->list)
				return;
			else {
				_cursor = cursor;
				cursor = cursor->list;
				branch = 1;
			}
		}

		if (branch || (cursor->list != NULL)) {
			last = _cursor;
			first = cursor;
			branch = 0;
		}

		if (*qcursor == '\0')
			finished = 1;
		else {
			qcursor++;
			_cursor = cursor;
			cursor = cursor->next;
		}
	}

	cursor->next = NULL;

	if (first == last->next)
		last->next = first->list;
	else
		last->list = first->list;

	cursor = first;

	while (!cursor) {
		first = cursor->next;
		free(cursor);
		cursor = NULL;
		cursor = first;
	}
}