#include "s21_string.h"

void *s21_to_lower(const char *str) {
  int str_len = s21_strlen(str);
  char *lower_string = S21_NULL;
  lower_string = malloc(sizeof(char) * str_len + 1);

  if (lower_string) {
    int i = 0;
    for (; i < str_len; i++) {
      if (str[i] >= 65 && str[i] <= 90) {
        lower_string[i] = str[i] + 32;
      } else {
        lower_string[i] = str[i];
      }
    }
    lower_string[i] = '\0';
  }

  return lower_string;
}
