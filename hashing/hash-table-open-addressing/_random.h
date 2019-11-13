#ifndef _RANDOM
#define _RANDOM 1

#ifndef LIMITS
#define LIMITS 1
#include <limits.h>
#endif

#ifndef TIME
#define TIME 1
#include <time.h>
#endif

#ifndef STDLIB
#define STDLIB 1
#include <stdlib.h>
#endif

#ifndef STDINT
#define STDINT 1
#include <stdint.h>
#endif

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifndef uint32_t
#define uint32_t unsigned int
#endif

extern uint32_t _random(uint32_t max);

#endif