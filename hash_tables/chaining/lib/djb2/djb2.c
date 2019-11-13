#include "djb2.h"
#include <stdio.h>

uint32_t djb2(void *key, size_t key_len) {
	if (!key) 	 exit(EXIT_FAILURE);

	const unsigned char *k = (const unsigned char *) key;

	unsigned long hash = 5381;
	uint32_t i;

	for (i = 0; i < key_len; i++)
		hash = ((hash << 5) + hash) + k[i];

	return hash;
}