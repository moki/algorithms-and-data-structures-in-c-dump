/**
 * uint32 djb2 hash function implementation, author: Daniel J. Bernstein
 *
 * Jun 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include "uint32-djb2-hash.h"

uint32_t uint32_t_djb2_hash(void *key) {
	const unsigned char *k = (const unsigned char *) key;
	unsigned long hash = 5381;
	uint32_t i;
	uint32_t j;

	for (i = 0, j = 0; i < sizeof(uint32_t); i++, j+= 8) {
		hash = ((hash << 5) + hash) + (k[i] << j);
	}

	return (uint32_t) hash;
}