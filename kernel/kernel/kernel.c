#include <kernel/isr.h>
#include <kernel/screen.h>
#include <stdio.h>

void kernel_main() {
    isr_install();
    irq_install();

    clear_screen();

    puts("Welcome to KiteOS!\n\n> ");
}
