#include "s21_string.h"

int s21_sscanf(const char *str, const char *format, ...) {
  va_list arg_list;
  va_start(arg_list, format);
  int success = 0, readed = 0, read_err;
  if (!check_empty(str, format, &success)) {
    int shift;
    while (*format) {
      if (*format == '%') {
        format++;
        read_err = 0;
        s21_scanf_t specifier;
        s21_scanf_init(&specifier);
        format = s21_scanf_parse(format, &specifier);
        shift = s21_scanf_process(specifier, &arg_list, str, &success, &readed,
                                  &read_err);
        if (!read_err) {
          str += shift;
        } else if (*str == '%') {
          str++;
        }
      } else if (*str == *format && *format != ' ') {
        // символ в строке и формате совпадает
        format++;
        str++;
        readed++;
      } else if (*str == ' ' || *str == 10 || *str == '\t') {
        // убираем непечатные символы
        int r_temp = s21_space_reader(str);
        str += r_temp;
        readed += r_temp;
        format += s21_space_reader(format);
      } else {
        format++;
      }
    }
    va_end(arg_list);
  }
  return success;
}

void s21_scanf_init(s21_scanf_t *spec) {
  spec->width = 0;
  spec->length = 0;
  spec->type = 0;
}

char *s21_scanf_parse(const char *format, s21_scanf_t *specifier) {
  format += s21_scanf_set_width(format, specifier);
  format += s21_scanf_set_length(format, specifier);
  format += s21_scanf_set_type(format, specifier);
  return (char *)format;
}

int s21_scanf_set_width(const char *format, s21_scanf_t *specifier) {
  int result = 0;
  if (*format != '*') {
    // читаем только цифры
    while (*format >= 48 && *format < 58) {
      result++;
      format++;
    }
    // проходим по строке и умножаем на разрядность
    for (int i = result; i > 0; i--) {
      specifier->width += s21_get_exp(i) * (*(format - i) - 48);
    }
  } else {
    specifier->width = -1;
    result++;
  }
  return result;
}

int s21_scanf_set_length(const char *format, s21_scanf_t *specifier) {
  int result = 0;
  while (*format == 'h' || *format == 'l' || *format == 'L') {
    specifier->length = *format;
    result++;
    format++;
  }
  return result;
}
int s21_scanf_set_type(const char *format, s21_scanf_t *specifier) {
  int result = 0;
  if (*format == 'c' || *format == 'd' || *format == 'i' || *format == 'e' ||
      *format == 'E' || *format == 'f' || *format == 'g' || *format == 'G' ||
      *format == 'o' || *format == 's' || *format == 'u' || *format == 'x' ||
      *format == 'X' || *format == 'p' || *format == 'n' || *format == '%') {
    specifier->type = *format;
    result = 1;
  }
  return result;
}
int s21_scanf_process(s21_scanf_t specifier, va_list *arg_list, const char *str,
                      int *success, int *readed, int *read_err) {
  int result = 0;
  if (specifier.type == 'c') {
    result += s21_scanf_c(specifier, str, arg_list);
  } else if (specifier.type == 'd' || specifier.type == 'i') {
    result += s21_scanf_di(specifier, str, arg_list, read_err);
  } else if (specifier.type == 'e' || specifier.type == 'E' ||
             specifier.type == 'g' || specifier.type == 'G' ||
             specifier.type == 'f') {
    result += s21_scanf_e(specifier, str, arg_list, read_err);
  } else if (specifier.type == 'o' || specifier.type == 'u' ||
             specifier.type == 'X' || specifier.type == 'x') {
    result += s21_scanf_o(specifier, str, arg_list, read_err);
  } else if (specifier.type == 's') {
    result += s21_scanf_s(specifier, str, arg_list, read_err);
  } else if (specifier.type == 'p') {
    result += s21_scanf_p(specifier, str, arg_list, read_err);
  } else if (specifier.type == 'n') {
    s21_scanf_n(arg_list, read_err, *readed);
  } else if (specifier.type == '%') {
    *read_err = 1;
  }
  if (*read_err == 0 && specifier.type != 'n' && specifier.width != -1) {
    (*readed) += result;
    (*success)++;
  }
  return result;
}
int s21_scanf_s(s21_scanf_t specifier, const char *str, va_list *arg_list,
                int *read_err) {
  int result = 0;         // сдвиг в изначальной строке
  char *temp = S21_NULL;  // ссылка на аргумент
  if (specifier.width != -1) {
    temp = (char *)va_arg(*arg_list, char *);
  }
  if (str && *str != 0) {
    int temp_len = s21_space_reader(str);  // убираем все непечатные символы
    str += temp_len;
    result += temp_len;

    specifier.width = set_spec_w(specifier.width);
    s21_populate_str(&temp, str, &result,
                     specifier);  // заполняем целевую строку
  } else if (temp) {
    *temp = 0;
    *read_err = 1;
  }

  return result;
}
int s21_scanf_c(s21_scanf_t specifier, const char *str, va_list *arg_list) {
  int result = 0;         // сдвиг в изначальной строке
  char *temp = S21_NULL;  // ссылка на аргумент
  if (specifier.width != -1) {
    temp = (char *)va_arg(*arg_list, char *);
  }
  if (str) {
    if (specifier.width > 0) {
      s21_populate_str(&temp, str, &result,
                       specifier);  // заполняем целевую строку
    } else {
      if (temp) {
        *temp = *str;
      }
      result++;
    }
  } else if (temp) {
    *temp = 0;
  }
  return result;
}
void s21_populate_str(char **temp, const char *str, int *result,
                      s21_scanf_t specifier) {  // заполняем целевую строку
  int shift = *result;
  while (*str != 0 && *str != 10 && *str != ' ' && *str != '\t' &&
         *result < specifier.width + shift) {
    if (*temp) {  // адрес аргумента не NULL
      **temp = *str;
      (*temp)++;
    }
    (*result)++;  // длина считанной строки
    str++;
  }
  if (specifier.type != 'c' && *temp) {  // нулевой символ в конце строки
    **temp = 0;
  }
}
int s21_scanf_di(s21_scanf_t specifier, const char *str, va_list *arg_list,
                 int *err) {
  unsigned long long int n;
  int result = 0, znak = 1;  // сдвиг в изначальной строке и знак числа
  result += s21_space_reader(str);  // убираем все непечатные символы
  str += result;
  result += s21_parse_str(str, &n, specifier, &znak,
                          err);  // получаем число и сдвиг
  if (!(*err) && specifier.width != -1) {
    s21_add_long_int(specifier, arg_list, n,
                     znak);  // заносим в аргумент
  } else if (specifier.width != -1) {
    *(int *)va_arg(*arg_list, int *) = 0;  // заносим в аргумент 0
  }
  return result;
}

int s21_scanf_e(s21_scanf_t specifier, const char *str, va_list *arg_list,
                int *err) {
  int result = 0;
  long double n = 0;
  result += s21_space_reader(str);
  str += result;
  result += s21_calc_double(&n, str, specifier,
                            err);  // получаем дробное число и сдвиг
  if (!(*err) && specifier.width != -1) {
    s21_add_long_double(specifier, arg_list, n);
  } else if (specifier.width != -1) {
    *(float *)va_arg(*arg_list, float *) = 0;
  }
  return result;
}

int s21_scanf_o(s21_scanf_t specifier, const char *str, va_list *arg_list,
                int *err) {
  unsigned long long int n;
  int result = 0, znak = 1;
  result += s21_space_reader(str);
  str += result;
  result += s21_parse_str(str, &n, specifier, &znak, err);
  if (!(*err) && specifier.width != -1) {
    s21_add_long_int(specifier, arg_list, n, znak);
  } else if (specifier.width != -1) {
    *(unsigned int *)va_arg(*arg_list, unsigned int *) = 0;
  }
  return result;
}

int s21_scanf_p(s21_scanf_t specifier, const char *str, va_list *arg_list,
                int *err) {
  unsigned long long int n;
  int result = 0, znak = 1;
  void *p = S21_NULL;
  if (!(*err) && specifier.width != -1) {
    result += s21_space_reader(str);
    str += result;
    result += s21_parse_str(str, &n, specifier, &znak, err);
    p = (void *)n;
    *(void **)va_arg(*arg_list, void **) = p;
  } else if (specifier.width != -1) {
    *(void **)va_arg(*arg_list, void **) = p;
  }
  return result;
}

void s21_scanf_n(va_list *arg_list, int *err, const int readed) {
  short *temp = (short *)va_arg(*arg_list, short *);
  if (temp) {
    *temp = readed;
  } else {
    *err = 1;
  }
}
int s21_parse_str(const char *str, unsigned long long int *n,
                  s21_scanf_t specifier, int *znak, int *err) {
  int temp_len = 0, result = 0, sys, x_finded = 0;
  *n = 0;
  int znak_len = set_znak(str, znak);
  result += znak_len;
  str += znak_len;
  sys = set_ohd(str, specifier);
  specifier.width = set_spec_w(specifier.width);

  while (s21_check_char(*str, specifier.type, sys, &x_finded) == 1 &&
         temp_len + result < specifier.width) {
    temp_len++;
    str++;
  }
  if (temp_len > 0) {
    s21_calc_int(n, str, temp_len, sys);
    result += temp_len;
  } else if (sys != 10 && *str == 0) {
    *n = 0;
  } else if (err) {
    *err = 1;
  }
  return result;
}
void s21_add_long_int(s21_scanf_t specifier, va_list *arg_list,
                      unsigned long long int n, const int znak) {
  n = s21_lim_int(n, specifier, znak);
  if (specifier.length == 'h') {
    n = (signed short int)n * znak;
    *(signed short int *)va_arg(*arg_list, signed short int *) = n;
  } else if (specifier.length == 0) {
    n = (signed int)n * znak;
    *(signed int *)va_arg(*arg_list, signed int *) = n;
  } else if (specifier.length == 'l') {
    n = (signed long int)n * znak;
    *(signed long int *)va_arg(*arg_list, signed long int *) = n;
  } else if (specifier.length == 'L') {
    n = (signed long long int)n * znak;
    *(long long int *)va_arg(*arg_list, signed long long int *) = n;
  }
}
void s21_add_long_double(s21_scanf_t specifier, va_list *arg_list,
                         long double n) {
  if (specifier.length == 0 || specifier.length == 'h') {
    *(float *)va_arg(*arg_list, float *) = (float)n;
  } else if (specifier.length == 'l') {
    *(double *)va_arg(*arg_list, double *) = (double)n;
  } else if (specifier.length == 'L') {
    *(long double *)va_arg(*arg_list, long double *) = n;
  }
}
unsigned long long int s21_get_exp(int x) {
  unsigned long long int result = 1;
  for (int i = 1; i < x; i++) {
    result *= 10;
  }
  return result;
}
int s21_space_reader(const char *str) {
  int result = 0;
  for (; *str; str++) {
    if (*str != ' ' && *str != 10 && *str != '\t') {
      break;
    }
    result++;
  }
  return result;
}
void s21_calc_int(unsigned long long int *n, const char *str, int length,
                  const int sys) {
  if (sys == 10) {  // число десятичное
    for (int i = length; i > 0; i--) {
      if (*n < S21_LONG_LONG_INT_MAX) {
        *n += s21_get_exp(i) * (*(str - i) - 48);
      }
    }
  } else {  // число oct или hex
    unsigned long long int u_pow;
    for (int i = length; i > 0; i--) {
      if (to_upper_ch(*(str - i)) == 'X') {
        continue;
      }
      u_pow = s21_pow(sys, i - 1);
      // проверка на большие значения
      if (*n >= S21_LONG_LONG_INT_MAX || u_pow >= S21_LONG_LONG_INT_MAX) {
        *n = S21_LONG_LONG_INT_MAX;
        break;
      }
      *n += s21_char_to_int(*(str - i)) * u_pow;
    }
  }
}
int s21_calc_double(long double *n, const char *str, s21_scanf_t specifier,
                    int *err) {
  int result = 0, first_len = 0, sec_len = 0, exp_len = 0, znak = 1,
      salt_len = 0;
  unsigned long long int first, sec;
  specifier.width = set_spec_w(specifier.width);
  first_len =
      s21_parse_str(str, &first, specifier, &znak, err);  // длина целой части
  str += first_len;
  if (!check_inf(str, n, &first_len, err, znak)) {  // проверка на inf и nan
    if (*(str) == '.' && first_len + 1 <= specifier.width) {
      str++;
      sec_len = s21_parse_str(str, &sec, specifier, &znak, S21_NULL);
      salt_len++;
      str += sec_len;
    }
    // проверка на ошибку в дроби
    if ((first_len == 0 && sec_len == 0) ||
        (first_len == specifier.width && *err == 1)) {
      *err = 1;
    } else {
      *err = 0;
    }
    long double decim = 0;
    // проверка на длину считанных символов
    if (sec_len != 0 && sec_len + first_len + salt_len <= specifier.width) {
      decim = (double)sec / s21_get_exp(sec_len + 1);
    } else {
      decim = 0.0;
    }
    *n = (first + decim) * znak;
    // проверка на экспоненту
    exp_len =
        double_exp_check(str, specifier, n, &salt_len, first_len + sec_len);
  }
  result = first_len + sec_len + exp_len + salt_len;  // общий сдвиг по строке
  return result;
}
unsigned long long int s21_lim_int(unsigned long long int n,
                                   s21_scanf_t specifier, int znak) {
  // макс мин значения для интов
  if (specifier.length == 'l' || specifier.length == 'L') {
    if (n >= S21_LONG_LONG_INT_MAX) {
      if (znak > 0) {
        n = S21_LONG_LONG_INT_MAX;
      } else {
        n = (unsigned long long int)S21_LONG_LONG_INT_MAX +
            (unsigned long long int)1;
      }
    }
  } else if (specifier.length == 0 || specifier.length == 'h') {
    if (n >= S21_LONG_LONG_INT_MAX) {
      if (znak > 0) {
        n = -1;
      } else {
        n = 0;
      }
    }
  }
  return n;
}
int s21_check_char(char ch, const char spec, const int system, int *x_finded) {
  int result = 0;

  if (system == 8 && ch >= 48 && ch < 56) {  // проверка oct значений
    result = 1;
  } else if (ch >= 48 && ch < 58 &&
             system != 8) {  // проверка hex и dex значений
    result = 1;
  } else {
    ch = to_upper_ch(ch);  // проверка буквы на hex
    if ((spec == 'i' || spec == 'x' || spec == 'X' || spec == 'p') &&
        system == 16 &&
        ((ch >= 65 && ch < 71) ||
         (to_upper_ch(ch) == 'X' &&
          *x_finded == 0))) {  //  если i шестадцатеричное
      result = 1;
      *x_finded = 1;
    } else {
      result = 0;
    }
  }
  return result;
}
int s21_char_to_int(char ch) {
  int num = 0;
  ch = to_upper_ch(ch);
  if (ch >= 65 && ch < 71) {  // маленькие или большие буквы
    num = ch - 55;
  } else {
    num = ch - '0';
  }
  return num;
}
unsigned long long int s21_pow(int a, int b) {
  // простая степень целого положительного числа
  // а не как в math
  unsigned long long int res = 1;
  for (int i = 1; i <= b && res < S21_LONG_LONG_INT_MAX; i++) {
    res *= (unsigned long long int)a;
  }
  if (res > S21_LONG_LONG_INT_MAX || res == 0) {
    res = S21_LONG_LONG_INT_MAX;
  }

  return res;
}
char to_upper_ch(char ch) {
  if (ch >= 97) {
    ch -= 32;
  }
  return (char)ch;
}

int check_empty(const char *str, const char *format, int *success) {
  int result = 0, not_empty_len = 0;
  while (*str) {
    // проверяем, есть ли в строке печатные символы
    if (*str != ' ' && *str != 10 && *str != '\t') {
      not_empty_len++;
    }
    str++;
  }
  if (!not_empty_len && *format != 0) {
    *success = -1;
    result = 1;
  } else if (!not_empty_len && *format == 0) {
    *success = 0;
    result = 1;
  }
  return result;
}

int check_inf(const char *str, long double *n, int *first_len, int *err,
              const int znak) {
  int result = 0;
  if (to_upper_ch(str[0]) == 'I' && to_upper_ch(str[1]) == 'N' &&
      to_upper_ch(str[2]) == 'F') {  // проверка на inf
    (*first_len) += 3;
    *n = S21_MAX_INF * znak;
    *err = 0;
    result = 1;
  } else if (to_upper_ch(str[0]) == 'N' && to_upper_ch(str[1]) == 'A' &&
             to_upper_ch(str[2]) == 'N') {  // проверка на nan
    (*first_len) += 3;
    *n = S21_NAN;
    *err = 0;
    result = 1;
  }
  return result;
}
int double_exp_check(const char *str, s21_scanf_t specifier, long double *n,
                     int *salt_len, const int fs_len) {
  // проверка на экспоненту
  int exp_len = 0, exp_znak = 1;
  unsigned long long int expo = 0;
  if ((*str == 'e' || *str == 'E') && fs_len + *salt_len < specifier.width) {
    str++;
    exp_len = s21_parse_str(str, &expo, specifier, &exp_znak, S21_NULL);
  }
  if (exp_len != 0) {
    (*salt_len)++;
    if (exp_znak > 0) {
      *n = *n * s21_get_exp(expo + 1);
    } else {
      *n = *n / s21_get_exp(expo + 1);
    }
    if (S21_IS_INF(*n)) {
      *n = S21_DBL_MIN;
    }
  }
  return exp_len;
}
int set_spec_w(int width) {
  if (width == 0 || width == -1) {  // трюк для считывания
    width = SPECIFIER_DEFAULT_W;
  }
  return width;
}
int set_znak(const char *str, int *znak) {
  int znak_len = 0;
  if (*str == '-') {
    *znak = -1;
    znak_len++;
  } else if (*str == '+') {
    znak_len++;
  }
  return znak_len;
}
int set_ohd(const char *str, s21_scanf_t specifier) {
  int sys = 10;
  if ((*str == '0' && to_upper_ch(*(str + 1)) == 'X' &&
       specifier.type == 'i') ||
      specifier.type == 'p' || to_upper_ch(specifier.type) == 'X') {
    sys = 16;
  } else if ((*str == '0' && specifier.type == 'i') || specifier.type == 'o') {
    sys = 8;
  }
  return sys;
}
