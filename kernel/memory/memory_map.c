#include <kernel/memory_map.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

memory_map_t *mem_map = (memory_map_t*) MEMORY_MAP_ADDR;

void print_memory_map() {
    char buff[19];
    int_to_ascii(mem_map->length, buff);
    printf("Number of entries: %s\n", buff);
    buff[0] = 0;

    for (int i = 0; i < mem_map->length; i++) {
        hex_to_ascii(mem_map->entries[i].base, buff);
        printf("%s + ", buff);
        buff[0] = 0;

        hex_to_ascii(mem_map->entries[i].length, buff);
        printf(buff);
        buff[0] = 0;

        hex_to_ascii(mem_map->entries[i].type, buff);
        printf(" | Type: %s\n", buff);
        buff[0] = 0;
    }
}
