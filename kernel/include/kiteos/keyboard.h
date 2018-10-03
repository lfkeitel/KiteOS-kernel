#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool shift, alt, ctrl;
    bool caps_lock, num_lock;
} keyboard_mod_t;

void init_keyboard();

#endif
