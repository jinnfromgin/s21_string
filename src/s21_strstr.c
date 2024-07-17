#include "s21_string.h"

// |+_| Ищет первое вхождение str2->str1, возвращает указатель на вхождение
// ("иголку в стоге сена")
char *s21_strstr(const char *str1, const char *str2) {
  char *result = *str2 == '\0' ? (char *)str1 : S21_NULL;
  while (*str1 != '\0') {  // пока не дошли до конца "сена"
    const char *ptr_str1 = (const char *)str1;
    const char *ptr_str2 = (const char *)str2;
    while (*ptr_str2 != '\0' &&
           *ptr_str1 == *ptr_str2) {  // Пока не дошли до конца "иголки" и
                                      // совпадает "иголка" и "сено"
      ptr_str1++;
      ptr_str2++;
    }
    if (*ptr_str2 == '\0') {
      result = (char *)str1;
      break;
    }
    str1++;
  }
  return result;
}
