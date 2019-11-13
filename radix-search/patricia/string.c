#include "string.h"

size_t stringkv_len(const void *key) {
	const char *k = (const char *) key;

	return strlen(k);
}

void stringkv_destroy(void *ptr) {
	free(ptr);
	ptr = NULL;
}