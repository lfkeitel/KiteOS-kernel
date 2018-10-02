#include <kernel/isr.h>
#include <kernel/screen.h>
#include <kernel/memory.h>
#include <stdio.h>

void kernel_main() {
    isr_install();
    irq_install();

    clear_screen();

    init_page_manager();
    init_kern_mem();

    puts("Welcome to KiteOS!\n\n> ");
}
