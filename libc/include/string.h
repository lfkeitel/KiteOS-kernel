#ifndef STRINGS_H
#define STRINGS_H

#include <sys/cdefs.h>

#include <stddef.h>
#include <stdint.h>

// String examination
int strcmp(const char *s1, const char *s2);
size_t strlen(const char*);

// Character array manipulation
void* memchr(const void*, int, size_t);
int memcmp(const void*, const void*, size_t);
void* memset(void*, uint8_t, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);

// KiteLibc Extensions
void int_to_ascii(int n, char *str);
void hex_to_ascii(int n, char *str);
void reverse(char *s);
void backspace(char *s);
void append(char *s, char n);
void str_to_lowercase(char *s);
void str_to_uppercase(char *s);

#endif
