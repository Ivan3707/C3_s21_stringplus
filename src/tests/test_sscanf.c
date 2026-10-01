#include <check.h>
#include <stdlib.h>

#include "s21_string.h"

/* ==================== sscanf base ==================== */

START_TEST(test_sscanf_int) {
  int a, b;
  int n = s21_sscanf("42 -17", "%d %d", &a, &b);
  ck_assert_int_eq(n, 2);
  ck_assert_int_eq(a, 42);
  ck_assert_int_eq(b, -17);
}
END_TEST

START_TEST(test_sscanf_string) {
  char buf[64];
  int n = s21_sscanf("hello world", "%s", buf);
  ck_assert_int_eq(n, 1);
  ck_assert_str_eq(buf, "hello");
}
END_TEST

START_TEST(test_sscanf_mixed) {
  char name[32];
  int age;
  float score;
  int n = s21_sscanf("Ivan 25 4.75", "%s %d %f", name, &age, &score);
  ck_assert_int_eq(n, 3);
  ck_assert_str_eq(name, "Ivan");
  ck_assert_int_eq(age, 25);
  ck_assert_float_eq_tol(score, 4.75, 0.001);
}
END_TEST

START_TEST(test_sscanf_char) {
  char c;
  int n = s21_sscanf("X", "%c", &c);
  ck_assert_int_eq(n, 1);
  ck_assert_int_eq(c, 'X');
}
END_TEST

START_TEST(test_sscanf_hex) {
  unsigned int x;
  int n = s21_sscanf("0xFF", "%x", &x);
  ck_assert_int_eq(n, 1);
  ck_assert_uint_eq(x, 255);
}
END_TEST

START_TEST(test_sscanf_octal) {
  unsigned int o;
  int n = s21_sscanf("0755", "%o", &o);
  ck_assert_int_eq(n, 1);
  ck_assert_uint_eq(o, 0755);
}
END_TEST

START_TEST(test_sscanf_suppress) {
  int a, b;
  int n = s21_sscanf("10 20 30", "%d %*d %d", &a, &b);
  ck_assert_int_eq(n, 2);
  ck_assert_int_eq(a, 10);
  ck_assert_int_eq(b, 30);
}
END_TEST

START_TEST(test_sscanf_width) {
  char buf[64];
  int n = s21_sscanf("abcdef", "%3s", buf);
  ck_assert_int_eq(n, 1);
  ck_assert_str_eq(buf, "abc");
}
END_TEST

START_TEST(test_sscanf_n) {
  int pos;
  int val;
  int n = s21_sscanf("abc123", "%*[a-z]%d%n", &val, &pos);
  ck_assert_int_eq(n, 1);
  ck_assert_int_eq(val, 123);
  ck_assert_int_eq(pos, 6);
}
END_TEST

START_TEST(test_sscanf_no_match) {
  int a;
  int n = s21_sscanf("hello", "%d", &a);
  ck_assert_int_eq(n, 0);
}
END_TEST

START_TEST(test_sscanf_long) {
  long l;
  int n = s21_sscanf("123456789", "%ld", &l);
  ck_assert_int_eq(n, 1);
  ck_assert_int_eq(l, 123456789L);
}
END_TEST

START_TEST(test_sscanf_short) {
  short s;
  int n = s21_sscanf("300", "%hd", &s);
  ck_assert_int_eq(n, 1);
  ck_assert_int_eq(s, 300);
}
END_TEST

/* ==================== Additional for coverage ==================== */

START_TEST(test_sscanf_float) {
  float f;
  int n = s21_sscanf("3.14", "%f", &f);
  ck_assert_int_eq(n, 1);
  ck_assert_float_eq_tol(f, 3.14f, 0.001f);
}
END_TEST

START_TEST(test_sscanf_double) {
  double d;
  int n = s21_sscanf("2.71828", "%lf", &d);
  ck_assert_int_eq(n, 1);
  ck_assert_double_eq_tol(d, 2.71828, 0.00001);
}
END_TEST

START_TEST(test_sscanf_long_double) {
  long double ld;
  int n = s21_sscanf("1.5", "%Lf", &ld);
  ck_assert_int_eq(n, 1);
  ck_assert((double)ld > 1.49 && (double)ld < 1.51);
}
END_TEST

START_TEST(test_sscanf_sci_e) {
  float f;
  int n = s21_sscanf("1.5e3", "%e", &f);
  ck_assert_int_eq(n, 1);
  ck_assert_float_eq_tol(f, 1500.0f, 0.1f);
}
END_TEST

START_TEST(test_sscanf_sci_E) {
  float f;
  int n = s21_sscanf("2.5E-2", "%E", &f);
  ck_assert_int_eq(n, 1);
  ck_assert_float_eq_tol(f, 0.025f, 0.001f);
}
END_TEST

START_TEST(test_sscanf_g) {
  float f;
  int n = s21_sscanf("1.23", "%g", &f);
  ck_assert_int_eq(n, 1);
  ck_assert_float_eq_tol(f, 1.23f, 0.001f);
}
END_TEST

START_TEST(test_sscanf_G) {
  float f;
  int n = s21_sscanf("4.56", "%G", &f);
  ck_assert_int_eq(n, 1);
  ck_assert_float_eq_tol(f, 4.56f, 0.001f);
}
END_TEST

START_TEST(test_sscanf_unsigned) {
  unsigned int u;
  int n = s21_sscanf("42", "%u", &u);
  ck_assert_int_eq(n, 1);
  ck_assert_uint_eq(u, 42);
}
END_TEST

START_TEST(test_sscanf_i_decimal) {
  int i;
  int n = s21_sscanf("42", "%i", &i);
  ck_assert_int_eq(n, 1);
  ck_assert_int_eq(i, 42);
}
END_TEST

START_TEST(test_sscanf_i_hex) {
  int i;
  int n = s21_sscanf("0x1F", "%i", &i);
  ck_assert_int_eq(n, 1);
  ck_assert_int_eq(i, 31);
}
END_TEST

START_TEST(test_sscanf_i_octal) {
  int i;
  int n = s21_sscanf("0755", "%i", &i);
  ck_assert_int_eq(n, 1);
  ck_assert_int_eq(i, 493);
}
END_TEST

START_TEST(test_sscanf_X_upper) {
  unsigned int x;
  int n = s21_sscanf("0xFF", "%X", &x);
  ck_assert_int_eq(n, 1);
  ck_assert_uint_eq(x, 255);
}
END_TEST

START_TEST(test_sscanf_scanset_negate) {
  char buf[64];
  int n = s21_sscanf("abc123", "%[^0-9]", buf);
  ck_assert_int_eq(n, 1);
  ck_assert_str_eq(buf, "abc");
}
END_TEST

START_TEST(test_sscanf_scanset_simple) {
  char buf[64];
  int n = s21_sscanf("hello world", "%[a-z]", buf);
  ck_assert_int_eq(n, 1);
  ck_assert_str_eq(buf, "hello");
}
END_TEST

START_TEST(test_sscanf_multiple_values) {
  int a, b, c;
  int n = s21_sscanf("1,2,3", "%d,%d,%d", &a, &b, &c);
  ck_assert_int_eq(n, 3);
  ck_assert_int_eq(a, 1);
  ck_assert_int_eq(b, 2);
  ck_assert_int_eq(c, 3);
}
END_TEST

START_TEST(test_sscanf_negative) {
  int a;
  int n = s21_sscanf("-42", "%d", &a);
  ck_assert_int_eq(n, 1);
  ck_assert_int_eq(a, -42);
}
END_TEST

START_TEST(test_sscanf_plus_sign) {
  int a;
  int n = s21_sscanf("+42", "%d", &a);
  ck_assert_int_eq(n, 1);
  ck_assert_int_eq(a, 42);
}
END_TEST

START_TEST(test_sscanf_empty_str) {
  int a;
  int n = s21_sscanf("", "%d", &a);
  ck_assert_int_eq(n, 0);
}
END_TEST

START_TEST(test_sscanf_null_str) {
  int n = s21_sscanf(S21_NULL, "%d");
  ck_assert_int_eq(n, -1);
}
END_TEST

START_TEST(test_sscanf_null_format) {
  int n = s21_sscanf("42", S21_NULL);
  ck_assert_int_eq(n, -1);
}
END_TEST

START_TEST(test_sscanf_char_multiple) {
  char buf[5];
  int n = s21_sscanf("abc", "%3c", buf);
  ck_assert_int_eq(n, 1);
  ck_assert(buf[0] == 'a' && buf[1] == 'b' && buf[2] == 'c');
}
END_TEST

/* ==================== Suite ==================== */

Suite* s21_sscanf_suite(void) {
  Suite* suite = suite_create("s21_sscanf");
  TCase* tc_sscanf = tcase_create("sscanf");

  tcase_add_test(tc_sscanf, test_sscanf_int);
  tcase_add_test(tc_sscanf, test_sscanf_string);
  tcase_add_test(tc_sscanf, test_sscanf_mixed);
  tcase_add_test(tc_sscanf, test_sscanf_char);
  tcase_add_test(tc_sscanf, test_sscanf_hex);
  tcase_add_test(tc_sscanf, test_sscanf_octal);
  tcase_add_test(tc_sscanf, test_sscanf_suppress);
  tcase_add_test(tc_sscanf, test_sscanf_width);
  tcase_add_test(tc_sscanf, test_sscanf_n);
  tcase_add_test(tc_sscanf, test_sscanf_no_match);
  tcase_add_test(tc_sscanf, test_sscanf_long);
  tcase_add_test(tc_sscanf, test_sscanf_short);

  tcase_add_test(tc_sscanf, test_sscanf_float);
  tcase_add_test(tc_sscanf, test_sscanf_double);
  tcase_add_test(tc_sscanf, test_sscanf_long_double);
  tcase_add_test(tc_sscanf, test_sscanf_sci_e);
  tcase_add_test(tc_sscanf, test_sscanf_sci_E);
  tcase_add_test(tc_sscanf, test_sscanf_g);
  tcase_add_test(tc_sscanf, test_sscanf_G);
  tcase_add_test(tc_sscanf, test_sscanf_unsigned);
  tcase_add_test(tc_sscanf, test_sscanf_i_decimal);
  tcase_add_test(tc_sscanf, test_sscanf_i_hex);
  tcase_add_test(tc_sscanf, test_sscanf_i_octal);
  tcase_add_test(tc_sscanf, test_sscanf_X_upper);
  tcase_add_test(tc_sscanf, test_sscanf_scanset_negate);
  tcase_add_test(tc_sscanf, test_sscanf_scanset_simple);
  tcase_add_test(tc_sscanf, test_sscanf_multiple_values);
  tcase_add_test(tc_sscanf, test_sscanf_negative);
  tcase_add_test(tc_sscanf, test_sscanf_plus_sign);
  tcase_add_test(tc_sscanf, test_sscanf_empty_str);
  tcase_add_test(tc_sscanf, test_sscanf_null_str);
  tcase_add_test(tc_sscanf, test_sscanf_null_format);
  tcase_add_test(tc_sscanf, test_sscanf_char_multiple);

  suite_add_tcase(suite, tc_sscanf);
  return suite;
}