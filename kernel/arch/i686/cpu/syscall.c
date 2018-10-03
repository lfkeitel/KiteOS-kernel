#include <kiteos/isr.h>
#include <kiteos/ports.h>
#include <kiteos/syscall.h>
#include <sys/function.h>
#include <string.h>
#include <stdio.h>

static syscall_t syscall_handlers[] = {
    syscall_null,
    syscall_shutdown
};

void handle_syscall(registers_t *r) {
    uint32_t syscall_num = r->eax;

    if (syscall_num <= MAX_SYSCALL && syscall_handlers[syscall_num] != 0) {
        isr_t handler = syscall_handlers[syscall_num];
        handler(r);
    } else {
        puts("No handler for syscall ");
        char s[10];
        hex_to_ascii(r->eax, s);
        puts(s);
        putchar('\n');
    }
}

void syscall_null(registers_t *r) {
    puts("null syscall\n");
    UNUSED(r);
}

void syscall_shutdown(registers_t *r) {
    // NOTE: This is emulator specific and will not work on real hardware
    for (const char *s = "Shutdown"; *s; ++s) {
        port_byte_write(0x8900, *s);
    }
    UNUSED(r);
}
