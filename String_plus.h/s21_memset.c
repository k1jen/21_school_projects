#include "s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
    unsigned char *buff = str;
    if (n!= 0) {
        do {
            *buff = (unsigned char)c;
            buff++;
            n--;
        } while (n != 0);
    }
    return str;
}
