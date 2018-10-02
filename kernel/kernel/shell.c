#include <kernel/shell.h>
#include <kernel/ports.h>
#include <kernel/memory.h>
#include <kernel/memory_map.h>
#include <sys/function.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

int shell_exit(char*);
int shell_page(char*);
int shell_echo(char*);
int shell_false(char*);
int shell_true(char*);
int shell_abort(char*);
int shell_test_syscall(char*);
int shell_shutdown(char*);
int shell_whoami(char*);
int shell_memory_map(char*);
int shell_help(char*);
int shell_malloc_test(char*);

typedef struct { char *key; shell_cmd_t val; } t_symstruct;

static t_symstruct lookuptable[] = {
    { "abort", shell_abort },
    { "echo", shell_echo },
    { "end", shell_exit },
    { "exit", shell_exit },
    { "false", shell_false },
    { "help", shell_help },
    { "logout", shell_shutdown },
    { "memmap", shell_memory_map },
    { "page", shell_page },
    { "sc", shell_test_syscall },
    { "shutdown", shell_shutdown },
    { "true", shell_true },
    { "kmalloc", shell_malloc_test },
    { "whoami", shell_whoami }
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

    if (strlen(input) == 0) {
        puts("> ");
        return;
    }

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

void print_page_info(size_t addr) {
    char page_str[16] = "";
    hex_to_ascii((size_t)ADDR_TO_PAGE(addr), page_str);

    char phys_str[16] = "";
    hex_to_ascii(addr, phys_str);

    printf("Page: %s, physical address: %s\n", page_str, phys_str);
}

int shell_page(char *input) {
    UNUSED(input);

    size_t phys_addr = (size_t)mem_req_pages(1);
    if (phys_addr == 0) {
        puts("Failed to allocate memory");
        return 0;
    }

    print_page_info(phys_addr);

    puts("Freeing page\n");
    mem_free_page((void*)phys_addr);

    phys_addr = (size_t)mem_req_pages(1);
    if (phys_addr == 0) {
        puts("Failed to allocate memory");
        return 0;
    }

    print_page_info(phys_addr);

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

int shell_test_syscall(char *input) {
    asm volatile("mov $0, %%eax" : : : "%eax");
    asm volatile("int $66");
    UNUSED(input);
    return 0;
}

int shell_whoami(char *input) {
    puts("groot\n");
    UNUSED(input);
    return 0;
}

int shell_shutdown(char *input) {
    asm volatile("mov $1, %%eax" : : : "%eax");
    asm volatile("int $66");
    UNUSED(input);
    return 0;
}

int shell_memory_map(char *input) {
    print_memory_map();
    UNUSED(input);
    return 0;
}

int shell_help(char *input) {
    puts("Available Commands:\n");
    for (int i = 0; i < NKEYS; i++) {
        t_symstruct sym = lookuptable[i];
        printf("  %s\n", sym.key);
    }
    UNUSED(input);
    return 0;
}

int shell_malloc_test(char *input) {
    UNUSED(input);
    char *test1 = (char*) kmalloc(10);
    char *test2 = (char*) kmalloc(16);
    char *test3 = (char*) kmalloc(20);

    char phys_str[16] = "";
    hex_to_ascii((size_t) test1, phys_str);
    printf("test1: %s\n", phys_str);

    memset(phys_str, 0, sizeof(phys_str));
    hex_to_ascii((size_t) test2, phys_str);
    printf("test2: %s\n", phys_str);

    memset(phys_str, 0, sizeof(phys_str));
    hex_to_ascii((size_t) test3, phys_str);
    printf("test3: %s\n", phys_str);
    return 0;
}
