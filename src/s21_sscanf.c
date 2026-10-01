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
  int result = 0;
  if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' ||
      c == '\v') {
    result = 1;
  }
  return result;
}

static int s21_ss_is_digit(char c) {
  int result = 0;
  if (c >= '0' && c <= '9') {
    result = 1;
  }
  return result;
}

static int s21_ss_hex_val(char c) {
  int result = -1;
  if (c >= '0' && c <= '9') {
    result = c - '0';
  } else if (c >= 'a' && c <= 'f') {
    result = c - 'a' + 10;
  } else if (c >= 'A' && c <= 'F') {
    result = c - 'A' + 10;
  }
  return result;
}

static int s21_ss_is_valid_digit(char c, int base) {
  int v = (base == 16) ? s21_ss_hex_val(c)
                       : (s21_ss_is_digit(c) ? (c - '0') : -1);
  int result = 0;
  if (v >= 0 && v < base) result = 1;
  return result;
}

static int s21_ss_in_charset(const int* charset, int negate, char c) {
  int in = charset[(unsigned char)c];
  if (negate) in = !in;
  return in;
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
  int result = 1;
  int n = (width > 0) ? width : 1;
  int k = 0;
  if (!suppress) {
    char* dest = va_arg(*ctx->args, char*);
    while (k < n && ctx->s[k]) {
      dest[k] = ctx->s[k];
      k++;
    }
  } else {
    while (k < n && ctx->s[k]) k++;
  }
  if (k == 0) {
    result = 0;
  } else {
    ctx->s += k;
  }
  return result;
}

static int s21_ss_handle_string(s21_sscanf_ctx* ctx, int suppress,
                                int max_len) {
  int result = 1;
  char buf[1024];
  int i = 0;
  while (*ctx->s && !s21_ss_is_space(*ctx->s) && i < max_len && i < 1023) {
    buf[i++] = *ctx->s++;
  }
  buf[i] = '\0';
  if (i == 0) {
    result = 0;
  } else if (!suppress) {
    char* dest = va_arg(*ctx->args, char*);
    for (int k = 0; k <= i; k++) dest[k] = buf[k];
  }
  return result;
}

static void s21_ss_parse_charset(s21_sscanf_ctx* ctx, int* charset,
                                 int* negate) {
  if (*ctx->f == '^') {
    *negate = 1;
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
}

static int s21_ss_handle_scanset(s21_sscanf_ctx* ctx, int suppress,
                                 int max_len) {
  int result = 1;
  int negate = 0;
  int charset[256] = {0};
  char buf[1024];
  int i = 0;

  s21_ss_parse_charset(ctx, charset, &negate);

  while (*ctx->s && i < max_len && i < 1023 &&
         s21_ss_in_charset(charset, negate, *ctx->s)) {
    buf[i++] = *ctx->s++;
  }
  buf[i] = '\0';

  if (i == 0) {
    result = 0;
  } else if (!suppress) {
    char* dest = va_arg(*ctx->args, char*);
    for (int k = 0; k <= i; k++) dest[k] = buf[k];
  }
  return result;
}

static int s21_ss_read_digits(s21_sscanf_ctx* ctx, int base, int max_len,
                              char* buf, int* out_len) {
  int i = 0;
  while (*ctx->s && i < 127 && i < max_len &&
         s21_ss_is_valid_digit(*ctx->s, base)) {
    buf[i++] = *ctx->s++;
  }
  buf[i] = '\0';
  *out_len = i;
  return i > 0;
}

static int s21_ss_detect_base(s21_sscanf_ctx* ctx, char spec) {
  int base = 10;
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
  return base;
}

static unsigned long long s21_ss_buf_to_ull(const char* buf, int i, int base) {
  unsigned long long val = 0;
  for (int k = 0; k < i; k++) {
    int v = (base == 16) ? s21_ss_hex_val(buf[k]) : (buf[k] - '0');
    val = val * base + v;
  }
  return val;
}

static void s21_ss_store_int(s21_sscanf_ctx* ctx, unsigned long long val,
                             int neg, int is_signed, char len_mod,
                             char spec) {
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

static int s21_ss_handle_int(s21_sscanf_ctx* ctx, int suppress, int max_len,
                             char len_mod, char spec) {
  int result = 1;
  int is_signed = (spec == 'd' || spec == 'i');
  int neg = 0;
  char buf[128];
  int i = 0;

  if ((spec == 'd' || spec == 'i') && (*ctx->s == '-' || *ctx->s == '+')) {
    if (*ctx->s == '-') neg = 1;
    ctx->s++;
  }

  int base = s21_ss_detect_base(ctx, spec);

  if (!s21_ss_read_digits(ctx, base, max_len, buf, &i)) {
    result = 0;
  } else {
    unsigned long long val = s21_ss_buf_to_ull(buf, i, base);
    if (!suppress) {
      s21_ss_store_int(ctx, val, neg, is_signed, len_mod, spec);
    }
  }
  return result;
}

static int s21_ss_try_float_char(s21_sscanf_ctx* ctx, char* buf, int* i,
                                 int* has_digit, int* has_dot, int* has_exp) {
  char c = *ctx->s;
  int ok = 0;

  if (s21_ss_is_digit(c)) {
    *has_digit = 1;
    buf[(*i)++] = c;
    ctx->s++;
    ok = 1;
  } else if (c == '.' && !*has_dot && !*has_exp) {
    *has_dot = 1;
    buf[(*i)++] = c;
    ctx->s++;
    ok = 1;
  } else if ((c == 'e' || c == 'E') && *has_digit && !*has_exp) {
    *has_exp = 1;
    buf[(*i)++] = c;
    ctx->s++;
    if ((*ctx->s == '-' || *ctx->s == '+') && *i < 127) {
      buf[(*i)++] = *ctx->s++;
    }
    ok = 1;
  }
  return ok;
}

static void s21_ss_store_float(s21_sscanf_ctx* ctx, const char* buf,
                               char len_mod) {
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

static int s21_ss_handle_float(s21_sscanf_ctx* ctx, int suppress, int max_len,
                               char len_mod) {
  int result = 1;
  char buf[128];
  int i = 0;
  int has_digit = 0;
  int has_dot = 0;
  int has_exp = 0;

  if ((*ctx->s == '-' || *ctx->s == '+') && i < 127) {
    buf[i++] = *ctx->s++;
  }
  while (*ctx->s && i < 127 && i < max_len &&
         s21_ss_try_float_char(ctx, buf, &i, &has_digit, &has_dot, &has_exp)) {
  }
  buf[i] = '\0';

  if (!has_digit) {
    result = 0;
  } else if (!suppress) {
    s21_ss_store_float(ctx, buf, len_mod);
  }
  return result;
}

static int s21_ss_dispatch_spec(s21_sscanf_ctx* ctx, int suppress, int width,
                                char len_mod, char spec) {
  int ok = 0;
  int max_len = (width > 0) ? width : INT_MAX;

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
    ok = -1;

  return ok;
}

static void s21_ss_handle_n(s21_sscanf_ctx* ctx, int suppress) {
  if (!suppress) {
    int* p = va_arg(*ctx->args, int*);
    *p = (int)(ctx->s - ctx->str_start);
  }
}

static int s21_ss_step_spec(s21_sscanf_ctx* ctx) {
  int result = 1;
  int suppress, width;
  char len_mod, spec;
  s21_ss_parse_spec(ctx, &suppress, &width, &len_mod, &spec);

  if (spec == '\0') {
    result = 0;
  } else {
    if (spec != 'c' && spec != 'n') s21_ss_skip_spaces(ctx);

    if (spec == 'n') {
      s21_ss_handle_n(ctx, suppress);
    } else if (*ctx->s == '\0') {
      result = 0;
    } else {
      int ok = s21_ss_dispatch_spec(ctx, suppress, width, len_mod, spec);
      if (ok == 0 || ok == -1) {
        result = 0;
      } else if (!suppress) {
        ctx->matched++;
      }
    }
  }
  return result;
}

static int s21_ss_step_literal(s21_sscanf_ctx* ctx) {
  int result = 1;
  if (*ctx->s == *ctx->f) {
    ctx->s++;
    ctx->f++;
  } else {
    result = 0;
  }
  return result;
}

static int s21_ss_step_percent(s21_sscanf_ctx* ctx) {
  int result = 1;
  if (*ctx->s == '%') {
    ctx->s++;
    ctx->f++;
  } else {
    result = 0;
  }
  return result;
}

static int s21_ss_step_space(s21_sscanf_ctx* ctx) {
  s21_ss_skip_spaces(ctx);
  ctx->f++;
  return 1;
}

static int s21_ss_step(s21_sscanf_ctx* ctx) {
  int result = 1;
  if (s21_ss_is_space(*ctx->f)) {
    result = s21_ss_step_space(ctx);
  } else if (*ctx->f != '%') {
    result = s21_ss_step_literal(ctx);
  } else {
    ctx->f++;
    if (*ctx->f == '%') {
      result = s21_ss_step_percent(ctx);
    } else {
      result = s21_ss_step_spec(ctx);
    }
  }
  return result;
}

int s21_sscanf(const char* str, const char* format, ...) {
  int result = -1;

  if (str != S21_NULL && format != S21_NULL) {
    va_list args;
    va_start(args, format);

    s21_sscanf_ctx ctx = {str, format, str, &args, 0};

    int running = 1;
    while (running && *ctx.f) {
      if (!s21_ss_step(&ctx)) running = 0;
    }

    va_end(args);
    result = ctx.matched;
  }

  return result;
}