/**
 * uint32 sdbm hash function implementation
 *
 * Jun 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#include "uint32_t-sdbm-hash.h"

uint32_t uint32_t_sdbm_hash(void *key) {
	const unsigned char *k = (const unsigned char *) key;
	unsigned long hash = 5381;
	uint32_t i;
	uint32_t j;

	for (i = 0, j = 0; i < sizeof(uint32_t); i++, j+= 8) {
		hash = (k[i] << j) + (hash << 6) + (hash << 16) - hash;
	}

	return (uint32_t) hash;
}