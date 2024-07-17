#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (!src || !str) {
    return S21_NULL;
  }
  s21_size_t src_len = s21_strlen(src);
  if (start_index > src_len) {
    return S21_NULL;
  }

  char *new_str = S21_NULL;
  new_str = malloc(sizeof(char) * (src_len + s21_strlen(str) + 1));

  if (new_str) {
    s21_size_t i = 0;
    s21_size_t last_src_index = 0;
    for (; i != start_index; i++) {
      new_str[i] = src[i];
    }
    last_src_index = i;
    for (s21_size_t j = 0; j < s21_strlen(str); i++, j++) {
      new_str[i] = str[j];
    }
    for (; last_src_index < src_len; last_src_index++, i++) {
      new_str[i] = src[last_src_index];
    }

    new_str[i] = '\0';
  }

  return new_str;
}
