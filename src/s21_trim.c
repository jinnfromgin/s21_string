#include "s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  char *new_str = S21_NULL;
  if (!src || !trim_chars) {
    return new_str;
  }
  s21_size_t src_len = s21_strlen(src);
  int str_start_index = -1;
  int str_end_index = 0;
  int new_str_len = 0;

  if (!trim_chars || *trim_chars == '\0') {
    str_start_index = s21_get_first_trim_index(src, src_len, " \n\t");
  } else {
    str_start_index = s21_get_first_trim_index(src, src_len, trim_chars);
  }

  if (str_start_index < 0) {
    new_str = malloc(1);
    new_str[0] = '\0';
  } else if (str_start_index >= 0) {
    new_str = malloc(sizeof(char) * src_len + 1);
    if (new_str) {
      if (!trim_chars || *trim_chars == '\0') {
        str_end_index = s21_get_end_trim_index(src, src_len, " \n\t");
      } else {
        str_end_index = s21_get_end_trim_index(src, src_len, trim_chars);
      }

      new_str_len = str_end_index - str_start_index + 1;
      int i = 0;
      for (; i < new_str_len; i++) {
        new_str[i] = src[str_start_index++];
      }
      new_str[i] = '\0';
    }
  }

  return new_str;
}

int s21_get_first_trim_index(const char *src, s21_size_t src_len,
                             const char *trim_chars) {
  int str_start_index = -1;
  for (s21_size_t i = 0; i < src_len; i++) {
    if (s21_strchr(trim_chars, src[i]) == S21_NULL) {
      str_start_index = i;
      break;
    }
  }
  return str_start_index;
}

int s21_get_end_trim_index(const char *src, s21_size_t src_len,
                           const char *trim_chars) {
  int str_end_index = 0;
  for (int i = src_len - 1; src[i]; i--) {
    if (s21_strchr(trim_chars, src[i]) == S21_NULL) {
      str_end_index = i;
      break;
    }
  }
  return str_end_index;
}
