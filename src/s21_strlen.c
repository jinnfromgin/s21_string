#include "s21_string.h"

// |+_| Длина строки
s21_size_t s21_strlen(const char *str) {  // i = len
  s21_size_t i = 0;
  if (str != S21_NULL) {
    while (str[i] != '\0') i++;  // Счетчик по символу до конца строки
  }
  return i;
}