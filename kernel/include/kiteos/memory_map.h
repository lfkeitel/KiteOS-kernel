#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define MEMORY_MAP_ADDR 0x800;

typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi_attrs;
} __attribute((packed)) memory_map_entry_t;

typedef struct {
    uint16_t length;
    memory_map_entry_t entries[];
} __attribute((packed)) memory_map_t;

void print_memory_map();

#endif
