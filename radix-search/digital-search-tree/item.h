#ifndef ITEM_H
#define ITEM_H

#include <stddef.h>

typedef struct item {
	const void *key;
	size_t      key_len;
	const void *value;
} item_t;

#endif