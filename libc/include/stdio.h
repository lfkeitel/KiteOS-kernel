#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int puts(const char*);
int putchar(char c);
int printf(const char* __restrict, ...);

#ifdef __cplusplus
}
#endif

#endif
