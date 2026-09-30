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
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  return -1;
}

int s21_sscanf(const char *str, const char *format, ...) {
  if (str == S21_NULL || format == S21_NULL) {
    return -1;
  }

  va_list args;
  va_start(args, format);

  const char *s = str;
  const char *f = format;
  int matched = 0;

  while (*f) {
    if (s21_ss_is_space(*f)) {
      while (s21_ss_is_space(*s))
        s++;
      f++;
      continue;
    }

    if (*f != '%') {
      if (*s != *f)
        break;
      s++;
      f++;
      continue;
    }

    f++;

    if (*f == '%') {
      if (*s != '%')
        break;
      s++;
      f++;
      continue;
    }

    int suppress = 0;
    if (*f == '*') {
      suppress = 1;
      f++;
    }

    int width = 0;
    while (s21_ss_is_digit(*f)) {
      width = width * 10 + (*f - '0');
      f++;
    }

    char len_mod = 0;
    if (*f == 'h' || *f == 'l' || *f == 'L') {
      len_mod = *f;
      f++;
    }

    char spec = *f;
    if (spec == '\0')
      break;
    f++;

    if (spec != 'c' && spec != 'n') {
      while (s21_ss_is_space(*s))
        s++;
    }

    if (spec == 'n') {
      if (!suppress) {
        int *p = va_arg(args, int *);
        *p = (int)(s - str);
      }
      continue;
    }

    if (*s == '\0')
      break;

    int this_match = 0;
    int max_len = (width > 0) ? width : INT_MAX;

    if (spec == 'c') {
      int n = (width > 0) ? width : 1;
      if (!suppress) {
        char *dest = va_arg(args, char *);
        int k = 0;
        while (k < n && s[k]) {
          dest[k] = s[k];
          k++;
        }
        if (k == 0)
          break;
        s += k;
      } else {
        int k = 0;
        while (k < n && s[k])
          k++;
        if (k == 0)
          break;
        s += k;
      }
      this_match = 1;
    } else if (spec == 's') {
      char buf[1024];
      int i = 0;
      while (*s && !s21_ss_is_space(*s) && i < max_len && i < 1023) {
        buf[i++] = *s++;
      }
      buf[i] = '\0';
      if (i == 0)
        break;
      if (!suppress) {
        char *dest = va_arg(args, char *);
        for (int k = 0; k <= i; k++)
          dest[k] = buf[k];
      }
      this_match = 1;
    } else if (spec == '[') {
      int negate = 0;
      int charset[256] = {0};

      if (*f == '^') {
        negate = 1;
        f++;
      }

      while (*f && *f != ']') {
        char start = *f++;
        if (*f == '-' && *(f + 1) != ']' && *(f + 1) != '\0') {
          f++;
          char end = *f++;
          for (int c = (unsigned char)start; c <= (unsigned char)end; c++) {
            charset[c] = 1;
          }
        } else {
          charset[(unsigned char)start] = 1;
        }
      }
      if (*f == ']')
        f++;

      char buf[1024];
      int i = 0;
      while (*s && i < max_len && i < 1023) {
        int in = charset[(unsigned char)*s];
        if (negate)
          in = !in;
        if (!in)
          break;
        buf[i++] = *s++;
      }
      buf[i] = '\0';

      if (i == 0)
        break;

      if (!suppress) {
        char *dest = va_arg(args, char *);
        for (int k = 0; k <= i; k++)
          dest[k] = buf[k];
      }
      this_match = 1;
    } else if (spec == 'd' || spec == 'i' || spec == 'u' || spec == 'o' ||
               spec == 'x' || spec == 'X' || spec == 'p') {
      int is_signed = (spec == 'd' || spec == 'i');
      int base = 10;
      char buf[128];
      int i = 0;
      int neg = 0;

      if ((spec == 'd' || spec == 'i') && (*s == '-' || *s == '+')) {
        if (*s == '-')
          neg = 1;
        s++;
      }

      if (spec == 'p') {
        base = 16;
        if (*s == '0' && (s[1] == 'x' || s[1] == 'X'))
          s += 2;
      } else if (spec == 'x' || spec == 'X') {
        base = 16;
        if (*s == '0' && (s[1] == 'x' || s[1] == 'X'))
          s += 2;
      } else if (spec == 'o') {
        base = 8;
      } else if (spec == 'i') {
        if (*s == '0' && (s[1] == 'x' || s[1] == 'X')) {
          base = 16;
          s += 2;
        } else if (*s == '0') {
          base = 8;
        }
      }

      while (*s && i < (int)sizeof(buf) - 1 && i < max_len) {
        int v;
        if (base == 16)
          v = s21_ss_hex_val(*s);
        else
          v = s21_ss_is_digit(*s) ? (*s - '0') : -1;
        if (v < 0 || v >= base)
          break;
        buf[i++] = *s++;
      }
      buf[i] = '\0';

      if (i == 0)
        break;

      unsigned long long val = 0;
      for (int k = 0; k < i; k++) {
        int v;
        if (base == 16)
          v = s21_ss_hex_val(buf[k]);
        else
          v = buf[k] - '0';
        val = val * base + v;
      }

      if (!suppress) {
        if (spec == 'p') {
          void **pp = va_arg(args, void **);
          *pp = (void *)(uintptr_t)val;
        } else if (len_mod == 'h') {
          short *p = va_arg(args, short *);
          *p = (short)(neg ? -(long long)val : (long long)val);
        } else if (len_mod == 'l') {
          long *p = va_arg(args, long *);
          *p = neg ? -(long)val : (long)val;
        } else if (is_signed) {
          int *p = va_arg(args, int *);
          *p = neg ? -(int)val : (int)val;
        } else {
          unsigned int *p = va_arg(args, unsigned int *);
          *p = (unsigned int)val;
        }
      }
      this_match = 1;
    } else if (spec == 'e' || spec == 'E' || spec == 'f' || spec == 'g' ||
               spec == 'G') {
      char buf[128];
      int i = 0;
      int has_digit = 0;
      int has_dot = 0;
      int has_exp = 0;

      if ((*s == '-' || *s == '+') && i < 127) {
        buf[i++] = *s++;
      }
      while (*s && i < 127 && i < max_len) {
        char c = *s;
        if (s21_ss_is_digit(c)) {
          has_digit = 1;
          buf[i++] = c;
          s++;
        } else if (c == '.' && !has_dot && !has_exp) {
          has_dot = 1;
          buf[i++] = c;
          s++;
        } else if ((c == 'e' || c == 'E') && has_digit && !has_exp) {
          has_exp = 1;
          buf[i++] = c;
          s++;
          if ((*s == '-' || *s == '+') && i < 127) {
            buf[i++] = *s++;
          }
        } else {
          break;
        }
      }
      buf[i] = '\0';

      if (!has_digit)
        break;

      if (!suppress) {
        long double val = strtold(buf, S21_NULL);
        if (len_mod == 'L') {
          long double *p = va_arg(args, long double *);
          *p = val;
        } else if (len_mod == 'l') {
          double *p = va_arg(args, double *);
          *p = (double)val;
        } else {
          float *p = va_arg(args, float *);
          *p = (float)val;
        }
      }
      this_match = 1;
    } else {
      break;
    }

    if (this_match && !suppress) {
      matched++;
    }
  }

  va_end(args);
  return matched;
}