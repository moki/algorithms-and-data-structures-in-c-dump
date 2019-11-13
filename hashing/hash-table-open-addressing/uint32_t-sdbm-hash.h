/**
 * uint32 sdbm hash function implementation
 *
 * Jun 2018 Kirill Morozov <morozov.kirill.moki@gmail.com>
 */

#ifndef STDINT
#define STDINT 1
#include <stdint.h>
#endif

#ifndef uint32_t
#define uint32_t unsigned int
#endif

extern uint32_t uint32_t_sdbm_hash(void *key);