/*
 * ZeonOS - Standard Library
 * lib/string.c
 * 
 * String manipulation functions
 */

#include "../kernel.h"

/**
 * Calculate string length
 */
int string_length(const char* str) {
    int len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

/**
 * Compare two strings
 */
int string_compare(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

/**
 * Copy string
 */
char* string_copy(char* dest, const char* src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
    return dest;
}

/**
 * Concatenate strings
 */
char* string_concat(char* dest, const char* src) {
    while (*dest) {
        dest++;
    }
    return string_copy(dest, src);
}

/**
 * Find character in string
 */
char* string_find_char(const char* str, char c) {
    while (*str) {
        if (*str == c) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

/**
 * Reverse string
 */
void string_reverse(char* str) {
    int len = string_length(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}
