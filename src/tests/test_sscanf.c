#include <check.h>
#include <stdlib.h>

#include "s21_string.h"

/* ==================== sscanf ==================== */

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
  suite_add_tcase(suite, tc_sscanf);

  return suite;
}