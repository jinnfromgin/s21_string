#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *start_token = S21_NULL;

  char *end_token;  // Указатель на конец текущего токена
  char *result;  // Указатель на текущий токен

  if (str) start_token = str;
  while (start_token && *start_token) {
    while (*start_token && s21_strpbrk(start_token, delim) == start_token)
      start_token++;  // если в start есть что то из разделителей, то смещаем
    if (*start_token == '\0') break;
    end_token = s21_strpbrk(start_token, delim);

    if (end_token) {
      *end_token = '\0';
      result = start_token;
      start_token = end_token + 1;
    } else {  // Если не найден символ разделителя
      result = start_token;
      start_token = S21_NULL;
    }
    if (*result != '\0') return result;
  }

  return S21_NULL;
}
