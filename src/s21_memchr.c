#include "s21_string.h"

void *s21_memchr(const void *str, int symbol, s21_size_t n) {
  s21_size_t result = n;
  const unsigned char *ptr_str = (const unsigned char *)str;
  if (str != S21_NULL) {
    s21_size_t i = 0;
    while (i < n && result == n) {
      if (ptr_str[i] == (unsigned char)symbol) {
        result = i;
      }
      i++;
    }
  }
  return result == n ? S21_NULL : (void *)(ptr_str + result);
}