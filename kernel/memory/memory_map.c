#include <kernel/memory_map.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

void print_memory_map() {
    uint16_t mem_table_len = *((uint16_t*) 0x7fe);
    memory_map_t *mem_map = (memory_map_t*) 0x800;

    char buff[19];
    int_to_ascii(mem_table_len, buff);
    puts("Number of entries: ");
    puts(buff);
    putchar('\n');
    buff[0] = 0;

    for (int i = 0; i < mem_table_len; i++) {
        hex_to_ascii(mem_map[i].base, buff);
        puts(buff);
        puts(" + ");
        buff[0] = 0;

        hex_to_ascii(mem_map[i].length, buff);
        puts(buff);
        buff[0] = 0;

        puts(" | Type: ");
        hex_to_ascii(mem_map[i].type, buff);
        puts(buff);
        buff[0] = 0;

        putchar('\n');
    }
}
