#include "s21_string.h"

// // Сравнение 2 строк в первых n символах - работает только с со строками
int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  const unsigned char *ptr_str1 = (const unsigned char *)str1;
  const unsigned char *ptr_str2 = (const unsigned char *)str2;
  int result = 0;

  for (s21_size_t i = 0; i < n; i++) {
    if (ptr_str1[i] != ptr_str2[i] || ptr_str1[i] == '\0' ||
        ptr_str2[i] == '\0') {
      result = ptr_str1[i] - ptr_str2[i];
      break;
    }
  }

  return result;
}