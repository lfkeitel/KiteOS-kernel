#include "../cpu/isr.h"
#include "../drivers/screen.h"

void kernel_main() {
    isr_install();
    irq_install();

    clear_screen();

    kprint("Welcome to KiteOS!\n\n> ");
}
