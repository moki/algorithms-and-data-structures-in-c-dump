#include "_random.h"

static int seed_reset = 0;

// max value of max is UINT32_MAX - 1
uint32_t _random(uint32_t max) {
	if (!seed_reset) {
		srand(time(NULL));
		seed_reset = 1;
	}

	return (uint32_t)(0 + rand() / (RAND_MAX / (max - 0 + 1) + 1));
}