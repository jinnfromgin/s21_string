#include "s21_string.h"

typedef struct {
  int minus;
  int plus;
  int space;
  int zero;
  int hash;
  int precision;
  int is_precision_set;
  int width;
  char length;
  char specifier;
} Flags;

extern Flags flag;
Flags flag = {'\0'};

int s21_sprintf(char *str, const char *format, ...) {
  va_list va;
  va_start(va, format);
  char *str_beginning = str;
  while (*format) {
    if (*format != '%') {
      *str++ = *format++;
      continue;
    } else {
      format++;
      s21_memset(&flag, 0, sizeof(Flags));
    }

    format = flags(format);
    format = width(format, va);
    format = precision(format, va);
    format = length(format);

    flag.specifier = *format;
    format++;

    char buff[BUFF_SIZE] = {'\0'};
    set_spec(buff, va);
    for (int i = 0; buff[i]; i++, str++) *str = buff[i];

    if (flag.specifier == 'n') {
      int *ret = va_arg(va, int *);
      *ret = str - str_beginning;
    }
  }
  *str = '\0';
  va_end(va);
  return str - str_beginning;
}

const char *flags(const char *format) {
  while (*format == '-' || *format == '+' || *format == ' ' || *format == '0' ||
         *format == '#') {
    switch (*format) {
      case '0':
        flag.zero = 1;
        break;
      case '-':
        flag.minus = 1;
        break;
      case '+':
        flag.plus = 1;
        break;
      case ' ':
        flag.space = 1;
        break;
      case '#':
        flag.hash = 1;
        break;
    }
    format++;
  }
  return format;
}

const char *width(const char *format, va_list va) {
  if (*format == '*') {
    format++;
    flag.width = va_arg(va, int);
  } else if (s21_isdigit(*format)) {
    char tmp[BUFF_SIZE] = {'\0'};
    for (int i = 0; s21_isdigit(*format); i++, format++) tmp[i] = *format;
    flag.width = s21_atoi(tmp);
  }
  return format;
}

const char *precision(const char *format, va_list va) {
  if (*format == '.') {
    flag.is_precision_set = 1;
    format++;
  }

  if (*format == '*') {
    format++;
    flag.precision = va_arg(va, int);
  }
  if (s21_isdigit(*format)) {
    char tmp[BUFF_SIZE] = {'\0'};
    for (int i = 0; s21_isdigit(*format); i++, format++) tmp[i] = *format;
    flag.precision = s21_atoi(tmp);
  }
  return format;
}
const char *length(const char *format) {
  switch (*format) {
    case 'h':
      flag.length = 'h';
      format++;
      break;
    case 'l':
      flag.length = 'l';
      format++;
      break;
    case 'L':
      flag.length = 'L';
      format++;
  }
  return format;
}
void set_spec(char *buff, va_list va) {
  if (flag.specifier == 'd' || flag.specifier == 'i')
    parse_int(buff, va);
  else if (flag.specifier == 'u')
    parse_unsigned(buff, va);
  else if (flag.specifier == 'o')
    parse_octal(buff, va);
  else if (flag.specifier == 'x' || flag.specifier == 'X')
    parse_hex(buff, va);
  else if (flag.specifier == '%')
    buff[0] = '%';
  else if (flag.specifier == 'c')
    parse_char(buff, va);
  else if (flag.specifier == 's')
    parse_string(buff, va);
  else if (flag.specifier == 'p')
    parse_pointer(buff, va);
  else if (flag.specifier == 'f')
    parse_float(buff, va);
  else if (flag.specifier == 'e' || flag.specifier == 'E')
    parse_mantiss(buff, va);
  else if (flag.specifier == 'g' || flag.specifier == 'G')
    parse_float_g_G(buff, va);
  if (flag.specifier == 'G' || flag.specifier == 'E' || flag.specifier == 'X')
    to_upper(buff);
}

void parse_int(char *buff, va_list va) {
  int64_t val = va_arg(va, int64_t);

  switch (flag.length) {
    case 0:
      val = (int32_t)val;
      break;
    case 'h':
      val = (int16_t)val;
  }
  whole_num_to_string(val, buff, 10);
  format_precision(buff);
  format_flags(buff);
}

void whole_num_to_string(int64_t val, char *ret, int base) {
  char tmp[BUFF_SIZE] = {'\0'};
  int idx = BUFF_SIZE - 2;

  int neg = val < 0 ? 1 : 0;
  val = neg ? -val : val;

  if (val == 0) tmp[idx] = '0';

  while (val > 0) {
    idx--;
    tmp[idx] = "0123456789abcdef"[val % base];
    val /= base;
  }
  for (int j = 0; tmp[idx]; idx++, j++) {
    if (neg && j == 0) {
      ret[j++] = '-';
    }

    ret[j] = tmp[idx];
  }
}

void format_precision(char *buff) {
  char tmp[BUFF_SIZE] = {'\0'};

  int sign = 0;
  int len = s21_strlen(buff);

  if (buff[0] == '-') {
    tmp[0] = '-';
    len--;
    sign = 1;
  }

  if (flag.precision > len) {
    int idx;
    for (idx = sign; idx < flag.precision - len + sign; idx++) tmp[idx] = '0';

    for (int i = sign; buff[i]; i++, idx++) tmp[idx] = buff[i];

    s21_strncpy(buff, tmp, s21_strlen(tmp));
  }

  if (flag.is_precision_set && flag.precision == 0 &&
      check_integer_specifier(flag.specifier) && buff[0] == '0')
    buff[0] = '\0';
}

int check_integer_specifier(char c) {
  char specs[] = {'d', 'i', 'o', 'u', 'x', 'X'};
  int res = 0;
  for (s21_size_t i = 0; i < sizeof(specs); i++) {
    if (specs[i] == c) {
      res = 1;
      break;
    }
  }
  return res;
}

void format_flags(char *buff) {
  char tmp[BUFF_SIZE + 1] = {'\0'};
  if (flag.plus && flag.specifier != 'u') {
    tmp[0] = buff[0] == '-' ? buff[0] : '+';
    int isneg = buff[0] == '-' ? 1 : 0;
    s21_strncpy(tmp + 1, buff + isneg, s21_strlen(buff + isneg));
    s21_strncpy(buff, tmp, s21_strlen(tmp));
  } else if (flag.space && buff[0] != '-' && flag.specifier != 'u') {
    tmp[0] = ' ';
    s21_strncpy(tmp + 1, buff, s21_strlen(buff));
    s21_strncpy(buff, tmp, s21_strlen(tmp));
  }
  if (flag.width > (int)s21_strlen(buff)) {
    int idx = flag.width - s21_strlen(buff);
    if (!flag.minus) {
      s21_memset(tmp, flag.zero ? '0' : ' ', idx);
      s21_strncpy(tmp + idx, buff, s21_strlen(buff));
    } else {
      s21_strncpy(tmp, buff, s21_strlen(buff));
      s21_memset(tmp + s21_strlen(tmp), ' ', idx);
    }
    s21_strncpy(buff, tmp, s21_strlen(tmp));
  }
}

void unsigned_num_to_string(uint64_t val, char *ret, int base) {
  char buf[BUFF_SIZE + 1] = {'\0'};
  int idx = BUFF_SIZE - 1;
  if (val == 0) {
    buf[idx] = '0';
    idx--;
  }

  for (; val && idx; --idx, val /= base)
    buf[idx] = "0123456789abcdef"[val % base];
  for (int j = 0; buf[idx + 1]; idx++, j++) ret[j] = buf[idx + 1];
}

void parse_unsigned(char *buff, va_list va) {
  uint64_t val = va_arg(va, uint64_t);
  switch (flag.length) {
    case 'h':
      val = (uint16_t)val;
      break;
    case 'l':
      val = (uint64_t)val;
      break;
    case 0:
      val = (uint32_t)val;
  }
  unsigned_num_to_string(val, buff, 10);
  format_precision(buff);
  format_flags(buff);
}

void parse_octal(char *buff, va_list va) {
  buff[0] = '0';
  whole_num_to_string(va_arg(va, int64_t), buff + flag.hash, 8);
  format_precision(buff);
  format_flags(buff);
}

void to_upper(char *str) {
  while (*str) {
    if (*str >= 'a' && *str <= 'z') *str = *str - 'a' + 'A';
    str++;
  }
}

int is_all_zeroes(char *buff) {
  for (int i = 0; buff[i]; i++)
    if (buff[i] != '0') return 0;
  return 1;
}

void parse_hex(char *buff, va_list va) {
  uint64_t val = va_arg(va, uint64_t);
  switch (flag.length) {
    case 0:
      val = (uint32_t)val;
      break;
    case 'h':
      val = (uint16_t)val;
      break;
    case 'l':
      val = (uint64_t)val;
      break;
  }
  unsigned_num_to_string(val, buff, 16);
  format_precision(buff);
  if (flag.hash) {
    prepend_ox(buff);
  }
  format_flags(buff);
}

void prepend_ox(char *buff) {
  if (!is_all_zeroes(buff) || flag.specifier == 'p') {
    int len = s21_strlen(buff);
    for (int i = len; i >= 0; i--) {
      buff[i + 2] = buff[i];
    }
    buff[0] = '0';
    buff[1] = 'x';
  }
}

void parse_char(char *buff, va_list va) {
  if (flag.length == 'l') {
    wchar_t w_c;
    w_c = va_arg(va, wchar_t);
    format_wchar(buff, w_c);
  } else {
    char c;
    c = va_arg(va, int);
    format_char(buff, c);
  }
}

void format_wchar(char *buff, wchar_t w_c) {
  if (!flag.minus && flag.width) {
    char tmp[BUFF_SIZE] = {'\0'};
    wcstombs(tmp, &w_c, BUFF_SIZE);
    for (s21_size_t i = 0; i < flag.width - s21_strlen(tmp); i++) buff[i] = ' ';
    s21_strncat(buff, tmp, s21_strlen(tmp));
  } else if (flag.width) {
    wcstombs(buff, &w_c, BUFF_SIZE);
    for (int i = s21_strlen(buff); i < flag.width; i++) buff[i] = ' ';
  } else {
    wcstombs(buff, &w_c, BUFF_SIZE);
  }
}
void format_char(char *buff, char c) {
  if (!flag.minus && flag.width) {
    for (int i = 0; i < flag.width; i++) {
      buff[i] = ' ';
      if (i == flag.width - 1) buff[i] = c;
    }
  } else if (flag.width) {
    buff[0] = c;
    for (int i = 1; i < flag.width; i++) buff[i] = ' ';
  } else {
    buff[0] = c;
  }
}

void parse_string(char *buff, va_list va) {
  if (flag.length == 'l') {
    wchar_t *wstr = va_arg(va, wchar_t *);
    format_wide_string(buff, wstr);
  } else {
    char *str = va_arg(va, char *);
    format_string(buff, str);
  }
}
void format_string(char *buff, char *str) {
  char tmp[BUFF_SIZE] = {'\0'};
  s21_strncpy(tmp, str, s21_strlen(str));
  if (flag.is_precision_set) tmp[flag.precision] = '\0';

  int shift = flag.width - s21_strlen(tmp);
  int len = s21_strlen(tmp);

  if (flag.minus && shift > 0) {
    s21_strncpy(buff, tmp, s21_strlen(tmp));
    s21_memset(buff + len, ' ', shift);
  } else if (shift > 0) {
    s21_memset(buff, ' ', shift);
    s21_strncpy(buff + shift, tmp, s21_strlen(tmp));
  } else {
    s21_strncpy(buff, tmp, s21_strlen(tmp));
  }
}
void format_wide_string(char *buff, wchar_t *wstr) {
  char tmp[BUFF_SIZE] = {'\0'};
  char str[BUFF_SIZE] = {'\0'};

  wcstombs(str, wstr, BUFF_SIZE);
  s21_strncpy(tmp, str, s21_strlen(str));
  if (flag.is_precision_set) tmp[flag.precision] = '\0';

  int shift = flag.width - s21_strlen(tmp);
  int len = s21_strlen(tmp);

  if (flag.minus && shift > 0) {
    s21_strncpy(buff, tmp, s21_strlen(tmp));
    s21_memset(buff + len, ' ', shift);
  } else if (shift > 0) {
    s21_memset(buff, ' ', shift);
    s21_strncpy(buff + shift, tmp, s21_strlen(tmp));
  } else {
    s21_strncpy(buff, tmp, s21_strlen(tmp));
  }
}

void parse_pointer(char *buff, va_list va) {
  unsigned_num_to_string(va_arg(va, uint64_t), buff, 16);
  format_precision(buff);
  prepend_ox(buff);
  format_flags(buff);
}

void parse_float(char *buff, va_list va) {
  long double val = 0;
  if (flag.length == 'L') {
    val = va_arg(va, long double);
  } else {
    val = va_arg(va, double);
  }

  if (!flag.is_precision_set) {
    flag.precision = 6;
  }

  double_to_string(val, buff);
  format_flags(buff);
}
void double_to_string(long double val, char *ret) {
  char buff[BUFF_SIZE] = {'\0'};
  int idx = BUFF_SIZE - 2;
  int neg = val < 0 ? 1 : 0;
  val = neg ? val * -1 : val;
  long double l = 0, r = modfl(val, &l);
  if (flag.precision == 0) {
    l = roundl(val);
    r = 0;
  }
  char fractions[BUFF_SIZE] = {'\0'};
  for (int i = 0; i < flag.precision; i++) {
    r = r * 10;
    fractions[i] = (int)r + '0';
  }
  long long right = roundl(r), left = l;
  if (!right) {
    for (int i = 0; i < flag.precision; idx--, i++) buff[idx] = '0';
  } else {
    for (int i = s21_strlen(fractions); right || i > 0; right /= 10, idx--, i--)
      buff[idx] = (int)(right % 10 + 0.05) + '0';
  }
  if ((flag.is_precision_set && flag.precision != 0) || (int)r ||
      (!flag.is_precision_set && val == 0) || s21_strlen(fractions))
    buff[idx--] = '.';
  if (!left) {
    buff[idx] = '0';
    idx--;
  } else {
    for (; left; left /= 10, idx--) buff[idx] = (int)(left % 10) + '0';
  }
  for (int i = 0; buff[idx + 1]; idx++, i++) {
    if (neg && i == 0) {
      ret[i] = '-';
      i++;
    }
    ret[i] = buff[idx + 1];
  }
}

void parse_mantiss(char *buff, va_list va) {
  long double val = 0;
  if (flag.length == 'L') {
    val = va_arg(va, long double);
  } else {
    val = va_arg(va, double);
  }
  int pow = 0;
  char sign = (int)val == 0 ? '-' : '+';

  if ((int)val - val) {
    while ((int)val == 0) {
      pow++;
      val *= 10;
    }
  } else {
    sign = '+';
  }
  while ((int)val / 10 != 0) {
    pow++;
    val /= 10;
  }

  if (!flag.is_precision_set) flag.precision = 6;
  double_to_string(val, buff);
  prepend_mantiss(buff, pow, sign);
  format_flags(buff);
}

void prepend_mantiss(char *str, int pow, char sign) {
  int len = s21_strlen(str);
  str[len] = 'e';
  str[len + 1] = sign;
  str[len + 3] = pow % 10 + '0';
  pow /= 10;
  str[len + 2] = pow % 10 + '0';
  str[len + 4] = '\0';
}

void parse_float_g_G(char *buff, va_list va) {
  long double val = 0;
  if (flag.length == 'L') {
    val = va_arg(va, long double);
  } else {
    val = va_arg(va, double);
  }
  if (!flag.is_precision_set) {
    flag.precision = 6;
  }
  if (flag.precision == 0) flag.precision = 1;
  int val_precision = flag.precision;
  long double m_val = val;
  int pow = 0;
  if ((int)val - val) {
    while ((int)m_val == 0) {
      pow++;
      m_val *= 10;
    }
  }
  if (pow > 4) {
    flag.precision = 0;
    double_to_string(m_val, buff);
  } else {
    flag.precision = 10;
    double_to_string(val, buff);
  }
  format_gG_precision(buff, val_precision);
  if (pow > 4) prepend_mantiss(buff, pow, '-');
  remove_trailing_zeroes(buff);
  format_flags(buff);
}
void remove_trailing_zeroes(char *buff) {
  int len = s21_strlen(buff);
  char *dot = s21_strchr(buff, '.');
  if (dot) {
    for (int i = len - 1; buff[i] != '.'; i--) {
      if (buff[i] == '0')
        buff[i] = '\0';
      else
        break;
    }
    if (dot[1] == '\0') dot[0] = '\0';
  }
}
void format_gG_precision(char *buff, int val_precision) {
  int sig_digs = 0;
  s21_size_t len = s21_strlen(buff);
  int not_zero_found = 0;
  for (s21_size_t i = 0; i < s21_strlen(buff); i++) {
    if ((buff[i] == '0' && !not_zero_found) || buff[i] == '.')
      continue;
    else
      not_zero_found = 1;

    if (s21_isdigit(buff[i]) && not_zero_found) {
      sig_digs++;
    }
    if (sig_digs == val_precision && i + 1 < len) {
      int next = buff[i + 1] == '.' ? 2 : 1;
      buff[i] = buff[i + next] - '0' > 5 ? (char)(buff[i] + 1) : buff[i];
      buff[i + 1] = '\0';
      break;
    }
  }
}

int s21_atoi(const char *str) {
  int res = 0;
  int sign = 1;
  int overflow = 0;

  while (*str == ' ') str++;
  if (*str == '-') {
    str++;
    sign = -1;
  }
  if (*str == '+') {
    str++;
  }

  while (*str && *str >= '0' && *str <= '9') {
    res = res * 10 + (*str - '0');
    if (res < 0) {
      overflow = 1;
      break;
    }
    str++;
  }
  if (overflow)
    res = sign > 0 ? INT32_MAX : INT32_MIN;
  else
    res *= sign;

  return res;
}
int s21_isdigit(char c) { return (c >= '0' && c <= '9'); }