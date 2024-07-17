#include "s21_string.h"

// |+_| Первое вхождение символа str1->str2 возврат указателя на найденый
// символ (похож на strcspn)
char *s21_strpbrk(const char *str1, const char *str2) {
  const char *ptr_str1 = (const char *)str1;

  int flag_trigger = 0;
  while (*ptr_str1 != '\0') {
    const char *ptr_str2 = (const char *)str2;
    while (*ptr_str2 != '\0') {
      if (*ptr_str1 == *ptr_str2) {
        flag_trigger = 1;
        break;
      }
      ptr_str2++;
    }
    if (flag_trigger) break;
    ptr_str1++;
  }
  return flag_trigger ? (char *)ptr_str1 : S21_NULL;
}