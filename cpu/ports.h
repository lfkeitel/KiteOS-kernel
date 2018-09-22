#ifndef PORTS_H
#define PORTS_H

#include "types.h"

unsigned char port_byte_read (u16 port);
void port_byte_write (u16 port, u8 data);
unsigned short port_word_read (u16 port);
void port_word_write (u16 port, u16 data);

#endif
