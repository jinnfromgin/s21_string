#ifndef S21_STRING_H
#define S21_STRING_H

#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

#include "s21_sprintf.h"
#include "s21_sscanf.h"

typedef long unsigned s21_size_t;
#define S21_NULL (void *)0

char *s21_strerror(int errnum);

void *s21_memchr(const void *str, int symbol, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int symbol, s21_size_t n);

char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int symbol);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);

s21_size_t s21_strcspn(const char *str1, const char *str2);
s21_size_t s21_strlen(const char *str);

char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int symbol);
char *s21_strstr(const char *str1, const char *str2);
char *s21_strtok(char *str, const char *delim);

void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);

void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);

int s21_get_first_trim_index(const char *src, s21_size_t src_len,
                             const char *trim_chars);

int s21_get_end_trim_index(const char *src, s21_size_t src_len,
                           const char *trim_chars);

#endif  //  _S21_STRING_H_