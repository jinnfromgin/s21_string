#include "s21_string.h"

// // Поиск 1-го вхождение символа в строке
// // по сути int работает как char
// // Возращает указатель на 1-ое вхождение
char *s21_strchr(const char *str, int symbol) {
  char *result = S21_NULL;
  if (str != S21_NULL) {
    while (*str && *str != symbol) {
      str++;
    }
    if (*str == symbol) {
      result = (char *)str;
    }
  }
  return result;
}