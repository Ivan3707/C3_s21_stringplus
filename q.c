#include "s21_string.h"

#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

static int s21_ss_is_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' ||
         c == '\v';
}

static int s21_ss_is_digit(char c) { return c >= '0' && c <= '9'; }

static int s21_ss_hex_val(char c) {
  int result = -1;

  if (c >= '0' && c <= '9')
    result = c - '0';
  else if (c >= 'a' && c <= 'f')
    result = c - 'a' + 10;
  else if (c >= 'A' && c <= 'F')
    result = c - 'A' + 10;

  return result;
}

static void s21_ss_skip_spaces(const char **s) {
  while (s21_ss_is_space(**s))
    (*s)++;
}

static int s21_ss_scan_char(const char **s, int width, int suppress,
                            va_list *args) {
  int n = width > 0 ? width : 1;
  int available = 0;
  int result = 0;

  while (available < n && (*s)[available])
    available++;

  if (available == n) {
    if (!suppress) {
      char *dest = va_arg(*args, char *);
      int k = 0;

      while (k < n) {
        dest[k] = (*s)[k];
        k++;
      }
    }

    *s += n;
    result = 1;
  }

  return result;
}

static int s21_ss_scan_string(const char **s, int max_len, int suppress,
                              va_list *args) {
  int i = 0;
  int result = 0;

  if (!suppress) {
    char *dest = va_arg(*args, char *);

    while (**s && !s21_ss_is_space(**s) && i < max_len) {
      dest[i++] = **s;
      (*s)++;
    }

    if (i > 0) {
      dest[i] = '\0';
      result = 1;
    }
  } else {
    while (**s && !s21_ss_is_space(**s) && i < max_len) {
      (*s)++;
      i++;
    }

    if (i > 0)
      result = 1;
  }

  return result;
}

static void s21_ss_build_charset(const char **f, int charset[256],
                                 int *negate) {
  *negate = 0;

  if (**f == '^') {
    *negate = 1;
    (*f)++;
  }

  if (**f == ']') {
    charset[(unsigned char)**f] = 1;
    (*f)++;
  }

  while (**f && **f != ']') {
    unsigned char start = (unsigned char)**f;
    (*f)++;

    if (**f == '-' && (*f)[1] != ']' && (*f)[1] != '\0') {
      unsigned char end;

      (*f)++;
      end = (unsigned char)**f;
      (*f)++;

      if (start <= end) {
        int c = start;

        while (c <= end) {
          charset[c] = 1;
          c++;
        }
      } else {
        charset[start] = 1;
        charset[end] = 1;
      }
    } else {
      charset[start] = 1;
    }
  }

  if (**f == ']')
    (*f)++;
}

static int s21_ss_scan_set(const char **s, const char **f, int max_len,
                           int suppress, va_list *args) {
  int charset[256] = {0};
  int negate = 0;
  int i = 0;
  int result = 0;

  s21_ss_build_charset(f, charset, &negate);

  if (!suppress) {
    char *dest = va_arg(*args, char *);

    while (**s && i < max_len) {
      int in = charset[(unsigned char)**s];

      if (negate)
        in = !in;

      if (in) {
        dest[i++] = **s;
        (*s)++;
      } else {
        break;
      }
    }

    if (i > 0) {
      dest[i] = '\0';
      result = 1;
    }
  } else {
    while (**s && i < max_len) {
      int in = charset[(unsigned char)**s];

      if (negate)
        in = !in;

      if (in) {
        (*s)++;
        i++;
      } else {
        break;
      }
    }

    if (i > 0)
      result = 1;
  }

  return result;
}

static int s21_ss_get_base(char spec, const char **s, int *remaining) {
  int base = 10;

  if (spec == 'p' || spec == 'x' || spec == 'X') {
    base = 16;

    if (*remaining >= 2 && (*s)[0] == '0' &&
        ((*s)[1] == 'x' || (*s)[1] == 'X')) {
      *s += 2;
      *remaining -= 2;
    }
  } else if (spec == 'o') {
    base = 8;
  } else if (spec == 'i') {
    if (*remaining >= 2 && (*s)[0] == '0' &&
        ((*s)[1] == 'x' || (*s)[1] == 'X')) {
      base = 16;
      *s += 2;
      *remaining -= 2;
    } else if (**s == '0') {
      base = 8;
    }
  }

  return base;
}

static unsigned long long s21_ss_make_value(const char *buf, int size,
                                            int base) {
  unsigned long long val = 0;
  int k = 0;

  while (k < size) {
    int v = base == 16 ? s21_ss_hex_val(buf[k]) : buf[k] - '0';

    val = val * (unsigned long long)base + (unsigned)v;
    k++;
  }

  return val;
}

static void s21_ss_store_integer(char spec, char len_mod, int neg,
                                 va_list *args, unsigned long long val) {
  if (spec == 'p') {
    void **pp = va_arg(*args, void **);
    *pp = (void *)(uintptr_t)val;
  } else if (spec == 'd' || spec == 'i') {
    if (len_mod == 'h') {
      short *p = va_arg(*args, short *);
      *p = neg ? (short)(-(long long)val) : (short)val;
    } else if (len_mod == 'l') {
      long *p = va_arg(*args, long *);
      *p = neg ? -(long)val : (long)val;
    } else {
      int *p = va_arg(*args, int *);
      *p = neg ? -(int)val : (int)val;
    }
  } else if (len_mod == 'h') {
    unsigned short *p = va_arg(*args, unsigned short *);
    *p = (unsigned short)(neg ? -val : val);
  } else if (len_mod == 'l') {
    unsigned long *p = va_arg(*args, unsigned long *);
    *p = (unsigned long)(neg ? -val : val);
  } else {
    unsigned int *p = va_arg(*args, unsigned int *);
    *p = (unsigned int)(neg ? -val : val);
  }
}

static int s21_ss_scan_integer(const char **s, char spec, char len_mod,
                               int width, int suppress, va_list *args) {
  int max_len = width > 0 ? width : INT_MAX;
  int remaining = max_len;
  int base = 10;
  int neg = 0;
  int i = 0;
  char buf[128];

  if ((*s)[0] == '-' || (*s)[0] == '+') {
    neg = (*s)[0] == '-';
    (*s)++;
    remaining--;
  }

  if (remaining > 0) {
    base = s21_ss_get_base(spec, s, &remaining);

    while (**s && i < remaining && i < (int)sizeof(buf) - 1) {
      int v = base == 16 ? s21_ss_hex_val(**s)
                         : (s21_ss_is_digit(**s) ? **s - '0' : -1);

      if (v >= 0 && v < base) {
        buf[i++] = **s;
        (*s)++;
      } else {
        break;
      }
    }
  }

  if (i > 0 && !suppress) {
    unsigned long long val = s21_ss_make_value(buf, i, base);
    s21_ss_store_integer(spec, len_mod, neg, args, val);
  }

  return i > 0;
}

static int s21_ss_scan_float(const char **s, char len_mod, int max_len,
                             int suppress, va_list *args) {
  char buf[128];
  int i = 0;
  int has_digit = 0;
  int has_dot = 0;
  int has_exp = 0;
  int result = 0;
  if ((**s == '-' || **s == '+') && i < max_len && i < 127)
    buf[i++] = *(*s)++;
  while (**s && i < max_len && i < 127) {
    char c = **s;
    if (s21_ss_is_digit(c)) {
      has_digit = 1;
      buf[i++] = c;
      (*s)++;
    } else if (c == '.' && !has_dot && !has_exp) {
      has_dot = 1;
      buf[i++] = c;
      (*s)++;
    } else {
      break;
    }
  }
  if (has_digit && !has_exp && **s && (**s == 'e' || **s == 'E') &&
      i < max_len && i < 127) {
    const char *exp_start = *s;
    int exp_pos = i;
    int exp_digits = 0;
    buf[i++] = *(*s)++;
    if ((**s == '-' || **s == '+') && i < max_len && i < 127)
      buf[i++] = *(*s)++;
    while (s21_ss_is_digit(**s) && i < max_len && i < 127) {
      buf[i++] = *(*s)++;
      exp_digits++;
    }
    if (exp_digits == 0) {
      *s = exp_start;
      i = exp_pos;
    } else {
      has_exp = 1;
    }
  }
  buf[i] = '\0';
  if (has_digit) {
    if (!suppress) {
      long double val = strtold(buf, S21_NULL);
      if (len_mod == 'L') {
        long double *p = va_arg(*args, long double *);
        *p = val;
      } else if (len_mod == 'l') {
        double *p = va_arg(*args, double *);
        *p = (double)val;
      } else {
        float *p = va_arg(*args, float *);
        *p = (float)val;
      }
    }
    result = 1;
  }
  return result;
}

static void s21_ss_store_n(const char *str, const char *s, char len_mod,
                           int suppress, va_list *args) {
  if (!suppress) {
    if (len_mod == 'h') {
      short *p = va_arg(*args, short *);
      *p = (short)(s - str);
    } else if (len_mod == 'l') {
      long *p = va_arg(*args, long *);
      *p = (long)(s - str);
    } else {
      int *p = va_arg(*args, int *);
      *p = (int)(s - str);
    }
  }
}

static void s21_ss_parse_spec(const char **f, int *suppress, int *width,
                              char *len_mod, char *spec) {
  *suppress = 0;
  *width = 0;
  *len_mod = 0;

  if (**f == '*') {
    *suppress = 1;
    (*f)++;
  }

  while (s21_ss_is_digit(**f)) {
    *width = *width * 10 + (**f - '0');
    (*f)++;
  }

  if (**f == 'h' || **f == 'l' || **f == 'L') {
    *len_mod = **f;
    (*f)++;
  }

  *spec = **f;

  if (**f)
    (*f)++;
}

static int s21_ss_process_spec(const char *str, const char **s, const char **f,
                               va_list *args, int *matched,
                               int *input_failure) {
  int suppress;
  int width;
  char len_mod;
  char spec;
  int this_match = 0;
  int stop = 0;

  s21_ss_parse_spec(f, &suppress, &width, &len_mod, &spec);

  if (spec != 'c' && spec != 'n')
    s21_ss_skip_spaces(s);

  if (spec == 'n') {
    s21_ss_store_n(str, *s, len_mod, suppress, args);
  } else if (**s == '\0') {
    *input_failure = 1;
    stop = 1;
  } else if (spec == 'c') {
    this_match = s21_ss_scan_char(s, width, suppress, args);
  } else if (spec == 's') {
    int max_len = width > 0 ? width : INT_MAX;
    this_match = s21_ss_scan_string(s, max_len, suppress, args);
  } else if (spec == '[') {
    int max_len = width > 0 ? width : INT_MAX;
    this_match = s21_ss_scan_set(s, f, max_len, suppress, args);
  } else if (spec == 'd' || spec == 'i' || spec == 'u' || spec == 'o' ||
             spec == 'x' || spec == 'X' || spec == 'p') {
    this_match = s21_ss_scan_integer(s, spec, len_mod, width, suppress, args);
  } else if (spec == 'e' || spec == 'E' || spec == 'f' || spec == 'g' ||
             spec == 'G') {
    int max_len = width > 0 ? width : INT_MAX;
    this_match = s21_ss_scan_float(s, len_mod, max_len, suppress, args);
  } else {
    stop = 1;
  }

  if (this_match && !suppress)
    (*matched)++;

  if (!this_match && spec != 'n')
    stop = 1;

  return stop;
}

int s21_sscanf(const char *str, const char *format, ...) {
  if (str == S21_NULL || format == S21_NULL)
    return -1;

  va_list args;
  va_start(args, format);

  const char *s = str;
  const char *f = format;
  int matched = 0;
  int input_failure = 0;
  int stop = 0;

  while (*f && !stop) {
    if (s21_ss_is_space(*f)) {
      s21_ss_skip_spaces(&s);
      f++;
    } else if (*f != '%') {
      if (*s == '\0') {
        input_failure = 1;
        stop = 1;
      } else if (*s == *f) {
        s++;
        f++;
      } else {
        stop = 1;
      }
    } else {
      f++;

      if (*f == '%') {
        if (*s == '%') {
          s++;
          f++;
        } else {
          input_failure = *s == '\0';
          stop = 1;
        }
      } else {
        stop =
            s21_ss_process_spec(str, &s, &f, &args, &matched, &input_failure);
      }
    }
  }

  va_end(args);

  if (matched == 0 && input_failure)
    return -1;

  return matched;
}