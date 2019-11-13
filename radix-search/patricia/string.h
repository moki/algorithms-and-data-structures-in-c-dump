#ifndef STRING_KV_H
#define STRING_KV_H

#include <string.h>
#include <stdlib.h>

size_t stringkv_len(const void *key);

void stringkv_destroy(void *ptr);

#endif