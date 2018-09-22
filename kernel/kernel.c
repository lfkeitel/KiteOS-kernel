#include "../drivers/screen.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"

void main() {
    isr_install();
    clear_screen();
    /* Test the interrupts */
    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");
    __asm__ __volatile__("int $4");

    // /* Fill up the screen */
    // int i = 0;
    // for (i = 0; i < 24; i++) {
    //     char str[255];
    //     int_to_ascii(i, str);
    //     kprint_at(str, 0, i);
    // }

    // kprint_at("This text forces the kernel to scroll. Row 0 will disappear. ", 60, 24);
    // kprint("And with this text, the kernel will scroll again, and row 1 will disappear too!");
}
