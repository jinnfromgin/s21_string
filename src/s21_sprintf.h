#ifndef _S21_SPRINTF_H_
#define _S21_SPRINTF_H_

#include "s21_string.h"

#define BUFF_SIZE 1024 * 8

int s21_sprintf(char *str, const char *format, ...);
// helper functions
void whole_num_to_string(int64_t val, char *ret, int base);
void unsigned_num_to_string(uint64_t val, char *ret, int base);
int check_integer_specifier(char c);
int is_all_zeroes(char *buff);
void to_upper(char *str);
void prepend_ox(char *buff);
void double_to_string(long double val, char *ret);
void prepend_mantiss(char *str, int pow, char sign);
void remove_trailing_zeroes(char *buff);

// parse format
const char *flags(const char *format);
const char *width(const char *format, va_list va);
const char *precision(const char *format, va_list va);
const char *length(const char *format);

// parse values
void set_spec(char *buff, va_list va);
void parse_int(char *buff, va_list va);
void parse_unsigned(char *buff, va_list va);
void parse_octal(char *buff, va_list va);
void parse_hex(char *buff, va_list va);
void parse_char(char *buff, va_list va);
void parse_string(char *buff, va_list va);
void parse_pointer(char *buff, va_list va);
void parse_float(char *buff, va_list va);
void parse_mantiss(char *buff, va_list va);
void parse_float_g_G(char *buff, va_list va);

// format values
void format_precision(char *buff);
void format_flags(char *buff);
void format_gG_precision(char *buff, int precision);
void format_wchar(char *buff, wchar_t w_c);
void format_char(char *buff, char c);
void format_string(char *buff, char *str);
void format_wide_string(char *buff, wchar_t *wstr);

int s21_isdigit(char c);
int s21_atoi(const char *str);
#endif  //  _S21_SPRINTF_H_
