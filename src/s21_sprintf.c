#include "s21_string.h"

typedef struct {
  char* buf;
  s21_size pos;
  s21_size cap;
} out_t;

typedef struct {
  int minus, plus, space, hash, zero;
  int width, prec;
  int width_star, prec_star;
  int len;
  char conv;
} spec_t;

#define FP_MAX_DIGITS 16

typedef struct {
  char digits[FP_MAX_DIGITS];
  int ndigits;
  int exp10;
  int negative;
  int is_nan;
  int is_inf;
} fp_t;

enum { LEN_NONE, LEN_h, LEN_l, LEN_L };

static void out_char(out_t* o, char c) {
  if (o->cap == 0 || o->pos + 1 < o->cap) {
    o->buf[o->pos] = c;
  }
  o->pos++;
}

static void out_repeat(out_t* o, char c, int n) {
  int k = 0;
  while (k < n) {
    out_char(o, c);
    k++;
  }
}

static void out_str_n(out_t* o, const char* s, int n) {
  int i = 0;
  while (i < n) {
    out_char(o, s[i]);
    i++;
  }
}

static void spec_init(spec_t* s) {
  s->minus = s->plus = s->space = s->hash = s->zero = 0;
  s->width = s->prec = -1;
  s->width_star = s->prec_star = 0;
  s->len = LEN_NONE;
  s->conv = 0;
}

static const char* parse_flags(const char* p, spec_t* s) {
  int go = 1;
  while (go) {
    if (*p == '-') {
      s->minus = 1;
      p++;
    } else if (*p == '+') {
      s->plus = 1;
      p++;
    } else if (*p == ' ') {
      s->space = 1;
      p++;
    } else if (*p == '#') {
      s->hash = 1;
      p++;
    } else if (*p == '0') {
      s->zero = 1;
      p++;
    } else {
      go = 0;
    }
  }
  if (s->minus) s->zero = 0;
  if (s->plus) s->space = 0;
  return p;
}

static const char* parse_width(const char* p, spec_t* s, va_list* ap) {
  if (*p == '*') {
    s->width_star = 1;
    p++;
  } else {
    int any = 0;
    while (*p >= '0' && *p <= '9') {
      if (!any) {
        s->width = 0;
        any = 1;
      }
      s->width = s->width * 10 + (*p - '0');
      p++;
    }
  }
  if (s->width_star) {
    s->width = va_arg(*ap, int);
    if (s->width < 0) {
      s->minus = 1;
      s->width = -s->width;
    }
  }
  return p;
}

static const char* parse_prec(const char* p, spec_t* s, va_list* ap) {
  if (*p == '.') {
    p++;
    if (*p == '*') {
      s->prec_star = 1;
      p++;
    } else {
      s->prec = 0;
      while (*p >= '0' && *p <= '9') {
        s->prec = s->prec * 10 + (*p - '0');
        p++;
      }
    }
    if (s->prec_star) s->prec = va_arg(*ap, int);
    if (s->prec < 0) s->prec = -1;
  }
  return p;
}

static const char* parse_len(const char* p, spec_t* s) {
  if (*p == 'h') {
    s->len = LEN_h;
    p++;
  } else if (*p == 'l') {
    s->len = LEN_l;
    p++;
  } else if (*p == 'L') {
    s->len = LEN_L;
    p++;
  }
  return p;
}

static const char* parse_spec(const char* p, spec_t* s, va_list* ap) {
  spec_init(s);
  p = parse_flags(p, s);
  p = parse_width(p, s, ap);
  p = parse_prec(p, s, ap);
  p = parse_len(p, s);
  s->conv = *p;
  return p;
}

static int make_digits(char* digits, unsigned long v, unsigned base, int upper,
                       const spec_t* s) {
  const char* dig = upper ? "0123456789ABCDEF" : "0123456789abcdef";
  int n = 0;
  if (v == 0) {
    if (s->prec != 0) {
      digits[n] = '0';
      n++;
    }
  } else {
    while (v != 0) {
      digits[n] = dig[v % base];
      n++;
      v /= base;
    }
  }
  return n;
}

static int make_prefix(char* prefix, unsigned base, int upper, int n, int zeros,
                       const char* digits, const spec_t* s) {
  int is_zero = (n == 1 && digits[0] == '0' && zeros == 0);
  int plen = 0;
  if (s->hash && base == 16 && !is_zero) {
    prefix[plen++] = '0';
    prefix[plen++] = upper ? 'X' : 'x';
  } else if (s->hash && base == 8 && !is_zero) {
    prefix[plen++] = '0';
  }
  return plen;
}

static char make_sign(int negative, unsigned base, const spec_t* s) {
  char sign = 0;
  if (base == 10) {
    if (negative)
      sign = '-';
    else if (s->plus)
      sign = '+';
    else if (s->space)
      sign = ' ';
  }
  return sign;
}

static void emit_left(out_t* o, char sign, const char* prefix, int plen) {
  if (sign) out_char(o, sign);
  int k = 0;
  while (k < plen) {
    out_char(o, prefix[k]);
    k++;
  }
}

static void emit_right_space(out_t* o, int pad, char sign, const char* prefix,
                             int plen) {
  out_repeat(o, ' ', pad);
  emit_left(o, sign, prefix, plen);
}

static void emit_right_zero(out_t* o, int pad, char sign, const char* prefix,
                            int plen) {
  emit_left(o, sign, prefix, plen);
  out_repeat(o, '0', pad);
}

static void emit_digits(out_t* o, const char* digits, int n, int zeros) {
  out_repeat(o, '0', zeros);
  while (n > 0) {
    n--;
    out_char(o, digits[n]);
  }
}

static int is_empty_zero(const spec_t* s, unsigned long v, unsigned base) {
  return (v == 0 && s->prec == 0 && base == 10 && !s->hash && !s->plus &&
          !s->space);
}

static void emit_empty_zero(out_t* o, const spec_t* s) {
  if (s->width > 0) out_repeat(o, ' ', s->width);
}

static void emit_number_body(out_t* o, const char* digits, int n, int zeros,
                             char sign, const char* prefix, int plen, int pad,
                             const spec_t* s) {
  if (s->minus) {
    emit_left(o, sign, prefix, plen);
  } else if (s->zero && s->prec < 0) {
    emit_right_zero(o, pad, sign, prefix, plen);
  } else {
    emit_right_space(o, pad, sign, prefix, plen);
  }
  emit_digits(o, digits, n, zeros);
  if (s->minus) out_repeat(o, ' ', pad);
}

static void out_number_full(out_t* o, unsigned long v, int negative,
                            unsigned base, int upper, const spec_t* s) {
  char digits[64];
  int n = make_digits(digits, v, base, upper, s);

  int zeros = 0;
  if (s->prec > n) zeros = s->prec - n;

  char prefix[2];
  int plen = make_prefix(prefix, base, upper, n, zeros, digits, s);
  char sign = make_sign(negative, base, s);

  int body = (sign ? 1 : 0) + plen + zeros + n;
  int pad = 0;
  if (s->width > body) pad = s->width - body;

  emit_number_body(o, digits, n, zeros, sign, prefix, plen, pad, s);
}

static void out_number(out_t* o, unsigned long v, int negative, unsigned base,
                       int upper, const spec_t* s) {
  if (is_empty_zero(s, v, base)) {
    emit_empty_zero(o, s);
  } else {
    out_number_full(o, v, negative, base, upper, s);
  }
}

static int str_len_capped(const char* s, int prec) {
  int n = 0;
  while (s[n] != '\0' && (prec < 0 || n < prec)) {
    n++;
  }
  return n;
}

static int compute_pad(int width, int n) {
  int pad = 0;
  if (width > 0 && width > n) pad = width - n;
  return pad;
}

static void out_string(out_t* o, const char* s, const spec_t* sp) {
  if (s == 0) s = "(null)";
  int n = str_len_capped(s, sp->prec);
  int pad = compute_pad(sp->width, n);
  if (!sp->minus) out_repeat(o, ' ', pad);
  out_str_n(o, s, n);
  if (sp->minus) out_repeat(o, ' ', pad);
}

static void out_char_field(out_t* o, char c, const spec_t* s) {
  int pad = 0;
  if (s->width > 1) pad = s->width - 1;
  if (!s->minus) out_repeat(o, ' ', pad);
  out_char(o, c);
  if (s->minus) out_repeat(o, ' ', pad);
}

static void fp_break_special(double x, fp_t* f) {
  f->is_nan = (x != x);
  if (!f->is_nan) {
    double ax = x < 0.0 ? -x : x;
    f->is_inf = (ax > 1.7976931348623157e308);
  }
}

static double fp_normalize(double ax, int* e_out) {
  int e = 0;
  if (ax != 0.0) {
    while (ax >= 10.0) {
      ax /= 10.0;
      e++;
    }
    while (ax < 1.0) {
      ax *= 10.0;
      e--;
    }
  }
  *e_out = e;
  return ax;
}

static long long fp_scaled(double ax) {
  long long scaled = 0;
  if (ax > 0.0) scaled = (long long)(ax * 1e15 + 0.5);
  return scaled;
}

static void fp_fill_digits(fp_t* f, long long scaled) {
  int i = FP_MAX_DIGITS - 1;
  while (i >= 0) {
    f->digits[i] = (char)(scaled % 10);
    scaled /= 10;
    i--;
  }
  f->ndigits = FP_MAX_DIGITS;
}

static void fp_fill_normal(double x, fp_t* f) {
  double ax = x < 0.0 ? -x : x;
  int e = 0;
  ax = fp_normalize(ax, &e);
  fp_fill_digits(f, fp_scaled(ax));
  f->exp10 = e + 1;
}

static void fp_break(double x, fp_t* f) {
  f->ndigits = 0;
  f->exp10 = 0;
  f->negative = (x < 0.0);
  f->is_nan = 0;
  f->is_inf = 0;

  fp_break_special(x, f);

  if (!f->is_nan && !f->is_inf) {
    fp_fill_normal(x, f);
  }
}

static int fp_carry(fp_t* f, int pos) {
  int i = pos;
  int carried = 0;
  while (i >= 0 && !carried) {
    if (f->digits[i] < 9) {
      f->digits[i]++;
      carried = 1;
    } else {
      f->digits[i] = 0;
      i--;
    }
  }
  return carried;
}

static void fp_shift_for_carry(fp_t* f) {
  int j = f->ndigits - 1;
  while (j > 0) {
    f->digits[j] = f->digits[j - 1];
    j--;
  }
  f->digits[0] = 1;
  f->exp10++;
}

static void fp_apply_round(fp_t* f, int next) {
  if (next >= 5) {
    int carried = fp_carry(f, f->ndigits - 1);
    if (!carried) fp_shift_for_carry(f);
  }
}

static void fp_round_digits(fp_t* f, int prec) {
  if (prec < 0) prec = 6;
  int keep = prec + 1;
  if (f->ndigits > keep) {
    int next = f->digits[keep];
    f->ndigits = keep;
    fp_apply_round(f, next);
  }
}

static void fp_emit_sign(out_t* o, const fp_t* f, const spec_t* s) {
  if (f->negative)
    out_char(o, '-');
  else if (s->plus)
    out_char(o, '+');
  else if (s->space)
    out_char(o, ' ');
}

static void fp_emit_int_part(out_t* o, const fp_t* f, int int_digits) {
  int i = 0;
  while (i < f->ndigits && i < int_digits) {
    out_char(o, (char)('0' + f->digits[i]));
    i++;
  }
  while (i < int_digits) {
    out_char(o, '0');
    i++;
  }
}

static void fp_emit_frac(out_t* o, const fp_t* f, int int_digits, int prec) {
  int j = 0;
  while (j < prec) {
    int idx = int_digits + j;
    int d = (idx < f->ndigits) ? f->digits[idx] : 0;
    out_char(o, (char)('0' + d));
    j++;
  }
}

static void fp_emit_plain(out_t* o, const fp_t* f, int prec, const spec_t* s) {
  int int_digits = f->exp10;
  if (int_digits <= 0) int_digits = 1;

  int body = int_digits + (prec > 0 ? 1 + prec : 0);
  if (s->hash && prec == 0) body += 1;
  if (f->negative || s->plus || s->space) body += 1;

  int pad = 0;
  if (s->width > body) pad = s->width - body;

  if (!s->minus && !s->zero) out_repeat(o, ' ', pad);
  fp_emit_sign(o, f, s);
  if (!s->minus && s->zero) out_repeat(o, '0', pad);

  fp_emit_int_part(o, f, int_digits);
  if (prec > 0 || s->hash) out_char(o, '.');
  fp_emit_frac(o, f, int_digits, prec);

  if (s->minus) out_repeat(o, ' ', pad);
}

static void fp_emit_exp(out_t* o, const fp_t* f, char e_char) {
  out_char(o, e_char);
  int e = f->exp10 - 1;
  if (e < 0) {
    out_char(o, '-');
    e = -e;
  } else {
    out_char(o, '+');
  }
  if (e >= 100) {
    out_char(o, (char)('0' + e / 100));
    e %= 100;
  }
  out_char(o, (char)('0' + (e / 10) % 10));
  out_char(o, (char)('0' + e % 10));
}

static void fp_emit_sci_frac(out_t* o, const fp_t* f, int prec) {
  int j = 0;
  while (j < prec) {
    int idx = 1 + j;
    int d = (idx < f->ndigits) ? f->digits[idx] : 0;
    out_char(o, (char)('0' + d));
    j++;
  }
}

static void fp_emit_sci(out_t* o, const fp_t* f, int prec, const spec_t* s,
                        int upper) {
  char e_char = upper ? 'E' : 'e';
  int body = 1 + (prec > 0 ? 1 + prec : 0) + 4;
  if (f->negative || s->plus || s->space) body += 1;
  if (s->hash && prec == 0) body += 1;

  int pad = 0;
  if (s->width > body) pad = s->width - body;

  if (!s->minus && !s->zero) out_repeat(o, ' ', pad);
  fp_emit_sign(o, f, s);
  if (!s->minus && s->zero) out_repeat(o, '0', pad);

  out_char(o, (char)('0' + f->digits[0]));
  if (prec > 0 || s->hash) out_char(o, '.');
  fp_emit_sci_frac(o, f, prec);
  fp_emit_exp(o, f, e_char);

  if (s->minus) out_repeat(o, ' ', pad);
}

static int choose_g_style(const fp_t* f, int prec) {
  int exp = f->exp10 - 1;
  if (prec < 0) prec = 6;
  if (prec == 0) prec = 1;
  int use_sci = 0;
  if (exp < -4 || exp >= prec) use_sci = 1;
  return use_sci;
}

static void fp_emit_special(out_t* o, const fp_t* f, const spec_t* s,
                            int upper) {
  const char* word =
      f->is_nan ? (upper ? "NAN" : "nan") : (upper ? "INF" : "inf");
  int n = 0;
  while (word[n] != '\0') n++;

  int body = n;
  if (f->negative || s->plus || s->space) body += 1;
  int pad = 0;
  if (s->width > body) pad = s->width - body;

  if (!s->minus) out_repeat(o, ' ', pad);
  fp_emit_sign(o, f, s);
  out_str_n(o, word, n);
  if (s->minus) out_repeat(o, ' ', pad);
}

static void out_float_f(out_t* o, fp_t* f, const spec_t* s) {
  int prec = s->prec;
  if (prec < 0) prec = 6;
  fp_round_digits(f, prec);
  fp_emit_plain(o, f, prec, s);
}

static void out_float_e(out_t* o, fp_t* f, const spec_t* s, int upper) {
  int prec = s->prec;
  if (prec < 0) prec = 6;
  fp_round_digits(f, prec);
  fp_emit_sci(o, f, prec, s, upper);
}

static void out_float_g(out_t* o, fp_t* f, const spec_t* s, int upper) {
  int prec = s->prec;
  int use_sci = choose_g_style(f, prec);
  if (prec < 0) prec = 6;
  if (prec == 0) prec = 1;

  if (use_sci) {
    int p = prec - 1;
    if (p < 0) p = 0;
    fp_round_digits(f, p);
    fp_emit_sci(o, f, p, s, upper);
  } else {
    int p = prec - f->exp10;
    if (p < 0) p = 0;
    fp_round_digits(f, p);
    fp_emit_plain(o, f, p, s);
  }
}

static void out_float(out_t* o, double x, const spec_t* s, char conv) {
  fp_t f;
  fp_break(x, &f);
  int upper = (conv == 'E' || conv == 'G');

  if (f.is_nan || f.is_inf) {
    fp_emit_special(o, &f, s, upper);
  } else if (conv == 'f' || conv == 'F') {
    out_float_f(o, &f, s);
  } else if (conv == 'e' || conv == 'E') {
    out_float_e(o, &f, s, upper);
  } else {
    out_float_g(o, &f, s, upper);
  }
}

static long read_signed(spec_t* s, va_list* ap) {
  long v = 0;
  if (s->len == LEN_l)
    v = va_arg(*ap, long);
  else if (s->len == LEN_h)
    v = (short)va_arg(*ap, int);
  else
    v = va_arg(*ap, int);
  return v;
}

static unsigned long read_unsigned(spec_t* s, va_list* ap) {
  unsigned long v = 0;
  if (s->len == LEN_l)
    v = va_arg(*ap, unsigned long);
  else if (s->len == LEN_h)
    v = (unsigned short)va_arg(*ap, unsigned);
  else
    v = va_arg(*ap, unsigned);
  return v;
}

static void conv_signed(out_t* o, spec_t* s, va_list* ap) {
  long v = read_signed(s, ap);
  unsigned long u =
      (v < 0) ? (unsigned long)(-(v + 1)) + 1UL : (unsigned long)v;
  out_number(o, u, v < 0, 10, 0, s);
}

static void conv_unsigned(out_t* o, spec_t* s, va_list* ap) {
  out_number(o, read_unsigned(s, ap), 0, 10, 0, s);
}

static void conv_octal(out_t* o, spec_t* s, va_list* ap) {
  out_number(o, read_unsigned(s, ap), 0, 8, 0, s);
}

static void conv_hex(out_t* o, spec_t* s, va_list* ap) {
  out_number(o, read_unsigned(s, ap), 0, 16, s->conv == 'X', s);
}

static void conv_char(out_t* o, spec_t* s, va_list* ap) {
  char c = (char)va_arg(*ap, int);
  out_char_field(o, c, s);
}

static void conv_string(out_t* o, spec_t* s, va_list* ap) {
  const char* str = va_arg(*ap, const char*);
  out_string(o, str, s);
}

static void conv_pointer(out_t* o, spec_t* s, va_list* ap) {
  void* ptr = va_arg(*ap, void*);
  spec_t ps = *s;
  ps.hash = 1;
  out_number(o, (unsigned long)(size_t)ptr, 0, 16, 0, &ps);
}

static void conv_count(out_t* o, va_list* ap) {
  int* dst = va_arg(*ap, int*);
  if (dst != 0) *dst = (int)o->pos;
}

static void conv_percent(out_t* o) { out_char(o, '%'); }

static void conv_float(out_t* o, spec_t* s, va_list* ap) {
  double v;
  if (s->len == LEN_L)
    v = (double)va_arg(*ap, long double);
  else
    v = va_arg(*ap, double);
  out_float(o, v, s, s->conv);
}

static int is_float_conv(char c) {
  return (c == 'f' || c == 'F' || c == 'e' || c == 'E' || c == 'g' || c == 'G');
}

static int dispatch(out_t* o, spec_t* s, va_list* ap) {
  int done = 1;
  char c = s->conv;

  if (c == 'd' || c == 'i') {
    conv_signed(o, s, ap);
  } else if (c == 'u') {
    conv_unsigned(o, s, ap);
  } else if (c == 'o') {
    conv_octal(o, s, ap);
  } else if (c == 'x' || c == 'X') {
    conv_hex(o, s, ap);
  } else if (c == 'c') {
    conv_char(o, s, ap);
  } else if (c == 's') {
    conv_string(o, s, ap);
  } else if (c == 'p') {
    conv_pointer(o, s, ap);
  } else if (c == 'n') {
    conv_count(o, ap);
  } else if (c == '%') {
    conv_percent(o);
  } else if (is_float_conv(c)) {
    conv_float(o, s, ap);
  } else {
    done = 0;
  }

  return done;
}

static void emit_unknown(out_t* o, char conv) {
  out_char(o, '%');
  out_char(o, conv);
}

static void step_plain(out_t* o, const char** pp) {
  out_char(o, **pp);
  *pp = *pp + 1;
}

static void step_spec(out_t* o, const char** pp, va_list* ap) {
  const char* p = *pp + 1;
  spec_t s;
  p = parse_spec(p, &s, ap);
  if (!dispatch(o, &s, ap)) emit_unknown(o, s.conv);
  if (s.conv != '\0') p++;
  *pp = p;
}

static void format_step(out_t* o, const char** pp, va_list* ap) {
  if (**pp != '%') {
    step_plain(o, pp);
  } else {
    step_spec(o, pp, ap);
  }
}

static void finish_buf(out_t* o) {
  if (o->cap > 0) {
    s21_size end = (o->pos < o->cap) ? o->pos : o->cap - 1;
    o->buf[end] = '\0';
  } else {
    o->buf[o->pos] = '\0';
  }
}

int s21_vsnprintf(char* buf, s21_size cap, const char* fmt, va_list ap) {
  va_list local_ap;
  va_copy(local_ap, ap);

  out_t o;
  o.buf = buf;
  o.pos = 0;
  o.cap = cap;

  const char* p = fmt;
  while (*p != '\0') {
    format_step(&o, &p, &local_ap);
  }

  va_end(local_ap);
  finish_buf(&o);
  return (int)o.pos;
}

int s21_sprintf(char* buf, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int n = s21_vsnprintf(buf, 0, fmt, ap);
  va_end(ap);
  return n;
}

int s21_snprintf(char* buf, s21_size cap, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int n = s21_vsnprintf(buf, cap, fmt, ap);
  va_end(ap);
  return n;
}
