#ifndef DJB2_H
#define DJB2_H

#include <stdint.h>
#include <stdlib.h>

uint32_t djb2(void *key, size_t key_len);

#endif