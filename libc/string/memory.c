#include <string.h>
#include <stddef.h>
#include <stdint.h>

void* memset(void *dest, uint8_t val, size_t len) {
    uint8_t *temp = (uint8_t*) dest;

    for ( ; len != 0; len--) *temp++ = val;

    return dest;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;

    for (size_t i = 0; i < size; i++) dst[i] = src[i];

    return dstptr;
}

void* memmove(void* dstptr, const void* srcptr, size_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;

    if (dst < src) {
        for (size_t i = 0; i < size; i++) dst[i] = src[i];
    } else {
        for (size_t i = size; i != 0; i--) dst[i-1] = src[i-1];
    }

    return dstptr;
}

int memcmp(const void* aptr, const void* bptr, size_t size) {
    const unsigned char* a = (const unsigned char*) aptr;
    const unsigned char* b = (const unsigned char*) bptr;

    for (size_t i = 0; i < size; i++) {
        if (a[i] < b[i]) return -1;
        else if (b[i] < a[i]) return 1;
    }

    return 0;
}

void* memchr(const void* ptr, int ch, size_t count) {
    if (ptr == NULL) return NULL;

    unsigned char c = (unsigned char) ch;
    unsigned char *p = (unsigned char*) ptr;

    while (count) {
        if (*p == c) return p;
        p++;
        count--;
    }

    return NULL;
}
