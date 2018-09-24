#ifndef MEM_H
#define MEM_H

#include <stdint.h>

/* At this stage there is no 'free' implemented. */
uint32_t kmalloc(uint32_t size, int align, uint32_t *phys_addr);

#endif
