#include <kiteos/keyboard.h>
#include <kiteos/ports.h>
#include <kiteos/isr.h>
#include <kiteos/screen.h>
#include <kiteos/shell.h>
#include <sys/function.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LSHIFT 0x2a
#define RSHIFT 0x36

static char key_buffer[256];
static keyboard_mod_t keyboard_mod;

#define SC_MAX 57
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6",
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
        'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};
const char sc_ascii_shifted[] = { '?', '?', '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
        'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G',
        'H', 'J', 'K', 'L', ':', '"', '~', '?', '|', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '};

static void keyboard_callback(registers_t *regs) {
    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = port_byte_read(0x60);
    bool keyup = false;

    if (scancode > SC_MAX && scancode <= 0x39 + 0x80) {
        keyup = true;
        scancode -= 0x80;
    }

    if (scancode > SC_MAX) return;
    switch (scancode) {
        case BACKSPACE:
            if (keyup) break;
            backspace(key_buffer);
            kprint_backspace();
            break;
        case ENTER:
            if (keyup) break;
            puts("\n");
            user_input(key_buffer); /* kernel-controlled function */
            key_buffer[0] = '\0';
            break;
        case LSHIFT:
        case RSHIFT:
            keyboard_mod.shift = !keyup;
            break;
        default: {
            if (keyup) break;
            char letter;
            if (keyboard_mod.shift) letter = sc_ascii_shifted[(int)scancode];
            else letter = sc_ascii[(int)scancode];

            /* Remember that kprint only accepts char[] */
            char str[2] = {letter, '\0'};
            append(key_buffer, letter);
            puts(str);
        }
    }

    UNUSED(regs);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}
