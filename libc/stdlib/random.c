#include <stdlib.h>
#include <stdint.h>

#define RAND_M 0x7fffffff
#define RAND_A 1103515245
#define RAND_C 12345

static unsigned long int next = 1;

int rand() {
    int32_t val = ((next * RAND_A) + RAND_C) & RAND_M;
    next = val;
    return val;
}

void srand(unsigned int seed) {
    next = seed;
}
