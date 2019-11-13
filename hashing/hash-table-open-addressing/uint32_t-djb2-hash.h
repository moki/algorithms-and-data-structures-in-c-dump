/**
 * uint32 djb2 hash function implementation, author: Daniel J. Bernstein
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

extern uint32_t uint32_t_djb2_hash(void *key);