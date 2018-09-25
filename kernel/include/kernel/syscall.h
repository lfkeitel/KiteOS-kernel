#ifndef SYSCALL_H
#define SYSCALL_H

#include <kernel/isr.h>

#define MAX_SYSCALL 1

void handle_syscall(registers_t *r);

// 0x0
void syscall_null(registers_t *r);

// 0x1
void syscall_shutdown(registers_t *r);

#endif
