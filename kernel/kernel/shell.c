#include <kernel/shell.h>
#include <kernel/memory.h>
#include <sys/function.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

int shell_exit(char *input);
int shell_page(char *input);
int shell_echo(char *input);
int shell_false(char *input);
int shell_true(char *input);
int shell_abort(char *input);

typedef struct { char *key; shell_cmd_t val; } t_symstruct;

static t_symstruct lookuptable[] = {
    { "exit", shell_exit },
    { "end", shell_exit },
    { "page", shell_page },
    { "echo", shell_echo },
    { "true", shell_true },
    { "false", shell_false },
    { "abort", shell_abort }
};

#define NKEYS (int)(sizeof(lookuptable)/sizeof(t_symstruct))

shell_cmd_t keyfromstring(char *key)
{
    int i;
    for (i = 0; i < NKEYS; i++) {
        t_symstruct sym = lookuptable[i];
        if (strcmp(sym.key, key) == 0)
            return sym.val;
    }
    return 0;
}

void user_input(char *input) {
    str_to_lowercase(input);

    shell_cmd_t cmd = keyfromstring(input);
    if (cmd == 0) {
        printf("Command not found: %s", input);
        puts("\n> ");
        return;
    }

    int ret = cmd(input);
    if (ret > 0) {
        puts("Exit code ");
        char exit_code[4];
        int_to_ascii(ret, exit_code);
        puts(exit_code);
        puts("\n");
    }
    puts("> ");
}

int shell_echo(char *input) {
    puts(input);
    puts("\n");
    return 0;
}

int shell_exit(char *input) {
    puts("Stopping the CPU. Bye!\n");
    asm volatile("hlt");
    UNUSED(input);
    return 0;
}

int shell_page(char *input) {
    uint32_t phys_addr;
    uint32_t page = kmalloc(1000, 1, &phys_addr);
    char page_str[16] = "";
    hex_to_ascii(page, page_str);
    char phys_str[16] = "";
    hex_to_ascii(phys_addr, phys_str);
    puts("Page: ");
    puts(page_str);
    puts(", physical address: ");
    puts(phys_str);
    UNUSED(input);
    return 0;
}

int shell_false(char *input) {
    UNUSED(input);
    return 1;
}

int shell_true(char *input) {
    UNUSED(input);
    return 0;
}

int shell_abort(char *input) {
    abort();
    UNUSED(input);
    return 0;
}
