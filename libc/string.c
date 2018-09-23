#include "string.h"
#include "bool_t.h"
#include <stdint.h>

char lowercase_char(char s);
char uppercase_char(char s);

/**
 * K&R implementation
 */
void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

void hex_to_ascii(int n, char str[]) {
    append(str, '0');
    append(str, 'x');
    char zeros = 0;

    int32_t tmp;
    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp > 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
    else append(str, tmp + '0');
}

/* K&R */
void reverse(char s[]) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* K&R */
int strlen(char s[]) {
    int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

void backspace(char s[]) {
    int len = strlen(s);
    s[len-1] = '\0';
}

/* K&R
 * Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2 */
int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

void str_to_lowercase(char s[]) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        s[i] = lowercase_char(s[i]);
    }
}

void str_to_uppercase(char s[]) {
    int i;
    for (i = 0; s[i] != '\0'; i++) {
        s[i] = uppercase_char(s[i]);
    }
}

void str_to_titlecase(char s[]) {
    bool uppercase = true;

    int i;
    for (i = 0; s[i] != '\0'; i++) {
        if (uppercase) {
            s[i] = uppercase_char(s[i]);
            uppercase = false;
        }

        if (s[i] == ' ') uppercase = true;
    }
}

char lowercase_char(char s) {
    if (s >= 0x41 && s <= 0x5A) {
        return s + 0x20;
    }
    return s;
}

char uppercase_char(char s) {
    if (s >= 0x61 && s <= 0x7A) {
        return s - 0x20;
    }
    return s;
}
