#ifndef PORTS_H
#define PORTS_H

#include "type.h"

unsigned char port_byte_read(uint16_t port);
void port_byte_write(uint16_t port, uint8_t data);
unsigned short port_word_read(uint16_t port);
void port_word_write(uint16_t port, uint16_t data);

#endif
