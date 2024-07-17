#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  const char *ptr_str1 = str1;
  int flag_trigger = 0;
  s21_size_t count = 0;
  while (*ptr_str1 != '\0') {
    const char *ptr_str2 = str2;
    while (*ptr_str2 != '\0') {
      if (*ptr_str1 == *ptr_str2) {
        flag_trigger = 1;
        break;
      }
      ptr_str2++;
    }
    if (flag_trigger) break;
    ptr_str1++;
    count++;
  }
  return count;
}