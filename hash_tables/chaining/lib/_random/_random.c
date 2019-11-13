#include "_random.h"

static int seed_reset = 0;

uint32_t _random(uint32_t min, uint32_t max) {
	if (!seed_reset) {
		srand(time(NULL));
		seed_reset = 1;
	}

	return (uint32_t) (min + rand() / (RAND_MAX / (max - min + 1) + 1));
}