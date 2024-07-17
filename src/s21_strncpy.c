#include "s21_string.h"

// Копирование n символов из src -> dest
char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *result = S21_NULL;
  if (src != S21_NULL && dest != S21_NULL) {
    result = dest;
    while (n && *src) {
      *dest++ = *src++;
      n--;
    }
    while (n) {
      *dest++ = '\0';
      n--;
    }
  }
  return result;
}