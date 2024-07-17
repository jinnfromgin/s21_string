#include "s21_string.h"

void *s21_to_upper(const char *str) {
  int str_len = s21_strlen(str);
  char *upper_string = S21_NULL;
  upper_string = malloc(sizeof(char) * str_len + 1);

  if (upper_string) {
    int i = 0;
    for (; i < str_len; i++) {
      if (str[i] >= 97 && str[i] <= 122) {
        upper_string[i] = str[i] - 32;
      } else {
        upper_string[i] = str[i];
      }
    }
    upper_string[i] = '\0';
  }

  return upper_string;
}
