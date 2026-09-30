#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#include "s21_string.h"

typedef struct {
  const char* s;
  const char* f;
  const char* str_start;
  va_list* args;
  int matched;
} s21_sscanf_ctx;

static int s21_ss_is_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' ||
         c == '\v';
}

static int s21_ss_is_digit(char c) { return c >= '0' && c <= '9'; }

static int s21_ss_hex_val(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return -1;
}

static void s21_ss_skip_spaces(s21_sscanf_ctx* ctx) {
  while (s21_ss_is_space(*ctx->s)) ctx->s++;
}

static void s21_ss_parse_spec(s21_sscanf_ctx* ctx, int* suppress, int* width,
                              char* len_mod, char* spec) {
  *suppress = 0;
  *width = 0;
  *len_mod = 0;

  if (*ctx->f == '*') {
    *suppress = 1;
    ctx->f++;
  }
  while (s21_ss_is_digit(*ctx->f)) {
    *width = *width * 10 + (*ctx->f - '0');
    ctx->f++;
  }
  if (*ctx->f == 'h' || *ctx->f == 'l' || *ctx->f == 'L') {
    *len_mod = *ctx->f;
    ctx->f++;
  }
  *spec = *ctx->f;
  if (*spec != '\0') ctx->f++;
}

static int s21_ss_handle_char(s21_sscanf_ctx* ctx, int suppress, int width) {
  int n = (width > 0) ? width : 1;
  int k = 0;
  if (!suppress) {
    char* dest = va_arg(*ctx->args, char*);
    while (k < n && ctx->s[k]) {
      dest[k] = ctx->s[k];
      k++;
    }
    if (k == 0) return 0;
  } else {
    while (k < n && ctx->s[k]) k++;
    if (k == 0) return 0;
  }
  ctx->s += k;
  return 1;
}

static int s21_ss_handle_string(s21_sscanf_ctx* ctx, int suppress,
                                int max_len) {
  char buf[1024];
  int i = 0;
  while (*ctx->s && !s21_ss_is_space(*ctx->s) && i < max_len && i < 1023) {
    buf[i++] = *ctx->s++;
  }
  buf[i] = '\0';
  if (i == 0) return 0;
  if (!suppress) {
    char* dest = va_arg(*ctx->args, char*);
    for (int k = 0; k <= i; k++) dest[k] = buf[k];
  }
  return 1;
}

static int s21_ss_handle_scanset(s21_sscanf_ctx* ctx, int suppress,
                                 int max_len) {
  int negate = 0;
  int charset[256] = {0};

  if (*ctx->f == '^') {
    negate = 1;
    ctx->f++;
  }

  while (*ctx->f && *ctx->f != ']') {
    char start = *ctx->f++;
    if (*ctx->f == '-' && *(ctx->f + 1) != ']' && *(ctx->f + 1) != '\0') {
      ctx->f++;
      char end = *ctx->f++;
      for (int c = (unsigned char)start; c <= (unsigned char)end; c++) {
        charset[c] = 1;
      }
    } else {
      charset[(unsigned char)start] = 1;
    }
  }
  if (*ctx->f == ']') ctx->f++;

  char buf[1024];
  int i = 0;
  while (*ctx->s && i < max_len && i < 1023) {
    int in = charset[(unsigned char)*ctx->s];
    if (negate) in = !in;
    if (!in) break;
    buf[i++] = *ctx->s++;
  }
  buf[i] = '\0';

  if (i == 0) return 0;
  if (!suppress) {
    char* dest = va_arg(*ctx->args, char*);
    for (int k = 0; k <= i; k++) dest[k] = buf[k];
  }
  return 1;
}

static int s21_ss_read_digits(s21_sscanf_ctx* ctx, int base, int max_len,
                              char* buf, int* out_len) {
  int i = 0;
  while (*ctx->s && i < 127 && i < max_len) {
    int v = (base == 16) ? s21_ss_hex_val(*ctx->s)
                         : (s21_ss_is_digit(*ctx->s) ? (*ctx->s - '0') : -1);
    if (v < 0 || v >= base) break;
    buf[i++] = *ctx->s++;
  }
  buf[i] = '\0';
  *out_len = i;
  return i > 0;
}

static int s21_ss_handle_int(s21_sscanf_ctx* ctx, int suppress, int max_len,
                             char len_mod, char spec) {
  int is_signed = (spec == 'd' || spec == 'i');
  int base = 10;
  int neg = 0;
  char buf[128];
  int i = 0;

  if ((spec == 'd' || spec == 'i') && (*ctx->s == '-' || *ctx->s == '+')) {
    if (*ctx->s == '-') neg = 1;
    ctx->s++;
  }

  if (spec == 'p' || spec == 'x' || spec == 'X') {
    base = 16;
    if (*ctx->s == '0' && (ctx->s[1] == 'x' || ctx->s[1] == 'X')) ctx->s += 2;
  } else if (spec == 'o') {
    base = 8;
  } else if (spec == 'i') {
    if (*ctx->s == '0' && (ctx->s[1] == 'x' || ctx->s[1] == 'X')) {
      base = 16;
      ctx->s += 2;
    } else if (*ctx->s == '0') {
      base = 8;
    }
  }

  if (!s21_ss_read_digits(ctx, base, max_len, buf, &i)) return 0;

  unsigned long long val = 0;
  for (int k = 0; k < i; k++) {
    int v = (base == 16) ? s21_ss_hex_val(buf[k]) : (buf[k] - '0');
    val = val * base + v;
  }

  if (!suppress) {
    if (spec == 'p') {
      void** pp = va_arg(*ctx->args, void**);
      *pp = (void*)(uintptr_t)val;
    } else if (len_mod == 'h') {
      short* p = va_arg(*ctx->args, short*);
      *p = (short)(neg ? -(long long)val : (long long)val);
    } else if (len_mod == 'l') {
      long* p = va_arg(*ctx->args, long*);
      *p = neg ? -(long)val : (long)val;
    } else if (is_signed) {
      int* p = va_arg(*ctx->args, int*);
      *p = neg ? -(int)val : (int)val;
    } else {
      unsigned int* p = va_arg(*ctx->args, unsigned int*);
      *p = (unsigned int)val;
    }
  }
  return 1;
}

static int s21_ss_handle_float(s21_sscanf_ctx* ctx, int suppress, int max_len,
                               char len_mod) {
  char buf[128];
  int i = 0;
  int has_digit = 0;
  int has_dot = 0;
  int has_exp = 0;

  if ((*ctx->s == '-' || *ctx->s == '+') && i < 127) {
    buf[i++] = *ctx->s++;
  }
  while (*ctx->s && i < 127 && i < max_len) {
    char c = *ctx->s;
    if (s21_ss_is_digit(c)) {
      has_digit = 1;
      buf[i++] = c;
      ctx->s++;
    } else if (c == '.' && !has_dot && !has_exp) {
      has_dot = 1;
      buf[i++] = c;
      ctx->s++;
    } else if ((c == 'e' || c == 'E') && has_digit && !has_exp) {
      has_exp = 1;
      buf[i++] = c;
      ctx->s++;
      if ((*ctx->s == '-' || *ctx->s == '+') && i < 127) {
        buf[i++] = *ctx->s++;
      }
    } else {
      break;
    }
  }
  buf[i] = '\0';

  if (!has_digit) return 0;

  if (!suppress) {
    long double val = strtold(buf, S21_NULL);
    if (len_mod == 'L') {
      long double* p = va_arg(*ctx->args, long double*);
      *p = val;
    } else if (len_mod == 'l') {
      double* p = va_arg(*ctx->args, double*);
      *p = (double)val;
    } else {
      float* p = va_arg(*ctx->args, float*);
      *p = (float)val;
    }
  }
  return 1;
}

static int s21_ss_step(s21_sscanf_ctx* ctx) {
  if (s21_ss_is_space(*ctx->f)) {
    s21_ss_skip_spaces(ctx);
    ctx->f++;
    return 1;
  }

  if (*ctx->f != '%') {
    if (*ctx->s != *ctx->f) return 0;
    ctx->s++;
    ctx->f++;
    return 1;
  }

  ctx->f++;

  if (*ctx->f == '%') {
    if (*ctx->s != '%') return 0;
    ctx->s++;
    ctx->f++;
    return 1;
  }

  int suppress, width;
  char len_mod, spec;
  s21_ss_parse_spec(ctx, &suppress, &width, &len_mod, &spec);
  if (spec == '\0') return 0;

  if (spec != 'c' && spec != 'n') s21_ss_skip_spaces(ctx);

  if (spec == 'n') {
    if (!suppress) {
      int* p = va_arg(*ctx->args, int*);
      *p = (int)(ctx->s - ctx->str_start);
    }
    return 1;
  }

  if (*ctx->s == '\0') return 0;

  int max_len = (width > 0) ? width : INT_MAX;
  int ok = 0;

  if (spec == 'c')
    ok = s21_ss_handle_char(ctx, suppress, width);
  else if (spec == 's')
    ok = s21_ss_handle_string(ctx, suppress, max_len);
  else if (spec == '[')
    ok = s21_ss_handle_scanset(ctx, suppress, max_len);
  else if (spec == 'd' || spec == 'i' || spec == 'u' || spec == 'o' ||
           spec == 'x' || spec == 'X' || spec == 'p')
    ok = s21_ss_handle_int(ctx, suppress, max_len, len_mod, spec);
  else if (spec == 'e' || spec == 'E' || spec == 'f' || spec == 'g' ||
           spec == 'G')
    ok = s21_ss_handle_float(ctx, suppress, max_len, len_mod);
  else
    return 0;

  if (!ok) return 0;
  if (!suppress) ctx->matched++;
  return 1;
}

int s21_sscanf(const char* str, const char* format, ...) {
  if (str == S21_NULL || format == S21_NULL) return -1;

  va_list args;
  va_start(args, format);

  s21_sscanf_ctx ctx = {str, format, str, &args, 0};

  int running = 1;
  while (running && *ctx.f) {
    if (!s21_ss_step(&ctx)) running = 0;
  }

  va_end(args);
  return ctx.matched;
}