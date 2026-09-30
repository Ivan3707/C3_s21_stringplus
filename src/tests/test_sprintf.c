#include "s21_string.h"

#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

/* ==================== целые: базовые ==================== */

START_TEST(test_sprintf_int_basic) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%d", 42);
  sprintf(std_buf, "%d", 42);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_int_negative) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%d", -42);
  sprintf(std_buf, "%d", -42);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_int_zero) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%d", 0);
  sprintf(std_buf, "%d", 0);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_int_int_min) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%d", INT_MIN);
  sprintf(std_buf, "%d", INT_MIN);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_int_int_max) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%d", INT_MAX);
  sprintf(std_buf, "%d", INT_MAX);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_i_specifier) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%i", -123);
  sprintf(std_buf, "%i", -123);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== целые: флаги ==================== */

START_TEST(test_sprintf_flag_plus) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%+d %+d", 42, -42);
  sprintf(std_buf, "%+d %+d", 42, -42);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_flag_space) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "% d % d", 42, -42);
  sprintf(std_buf, "% d % d", 42, -42);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_flag_minus) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "[%-5d]", 42);
  sprintf(std_buf, "[%-5d]", 42);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_flag_zero) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%05d", 42);
  sprintf(std_buf, "%05d", 42);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_flag_zero_negative) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%05d", -42);
  sprintf(std_buf, "%05d", -42);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== целые: ширина ==================== */

START_TEST(test_sprintf_width_basic) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "[%10d]", 42);
  sprintf(std_buf, "[%10d]", 42);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_width_star) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "[%*d]", 8, 42);
  sprintf(std_buf, "[%*d]", 8, 42);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_width_star_negative) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "[%*d]", -8, 42);
  sprintf(std_buf, "[%*d]", -8, 42);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== целые: точность ==================== */

START_TEST(test_sprintf_prec_basic) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%.5d", 42);
  sprintf(std_buf, "%.5d", 42);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_prec_zero_value) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%.0d", 0);
  sprintf(std_buf, "%.0d", 0);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_prec_star) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%.*d", 6, 42);
  sprintf(std_buf, "%.*d", 6, 42);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== беззнаковые / hex / octal ==================== */

START_TEST(test_sprintf_unsigned) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%u", 4000000000u);
  sprintf(std_buf, "%u", 4000000000u);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_hex) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%x %X", 0xdeadbeefu, 0xdeadbeefu);
  sprintf(std_buf, "%x %X", 0xdeadbeefu, 0xdeadbeefu);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_hex_hash) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%#x %#X", 255u, 255u);
  sprintf(std_buf, "%#x %#X", 255u, 255u);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_octal) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%o %#o", 64u, 64u);
  sprintf(std_buf, "%o %#o", 64u, 64u);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== длина ==================== */

START_TEST(test_sprintf_long) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%ld", 1234567890L);
  sprintf(std_buf, "%ld", 1234567890L);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_long_negative) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%ld", -1234567890L);
  sprintf(std_buf, "%ld", -1234567890L);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_short) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%hd", (short)30000);
  sprintf(std_buf, "%hd", (short)30000);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_unsigned_long) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%lu", 1234567890UL);
  sprintf(std_buf, "%lu", 1234567890UL);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== строки ==================== */

START_TEST(test_sprintf_string) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%s", "hello");
  sprintf(std_buf, "%s", "hello");
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_string_width) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "[%10s]", "abc");
  sprintf(std_buf, "[%10s]", "abc");
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_string_left) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "[%-10s]", "abc");
  sprintf(std_buf, "[%-10s]", "abc");
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_string_prec) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%.3s", "abcdef");
  sprintf(std_buf, "%.3s", "abcdef");
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_string_empty) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "[%s]", "");
  sprintf(std_buf, "[%s]", "");
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== символ ==================== */

START_TEST(test_sprintf_char) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%c", 'X');
  sprintf(std_buf, "%c", 'X');
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_char_width) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "[%5c]", 'X');
  sprintf(std_buf, "[%5c]", 'X');
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== процент ==================== */

START_TEST(test_sprintf_percent) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "100%% done");
  sprintf(std_buf, "100%% done");
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== смешанные ==================== */

START_TEST(test_sprintf_mixed) {
  char s21_buf[256];
  char std_buf[256];

  s21_sprintf(s21_buf, "%d + %d = %d, %s!", 2, 3, 5, "ok");
  sprintf(std_buf, "%d + %d = %d, %s!", 2, 3, 5, "ok");
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_complex_format) {
  char s21_buf[256];
  char std_buf[256];

  s21_sprintf(s21_buf, "[%-6d] [%+05d] [%#x] [%.3s]", 42, -7, 255u, "abcdef");
  sprintf(std_buf, "[%-6d] [%+05d] [%#x] [%.3s]", 42, -7, 255u, "abcdef");
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== возвращаемое значение ==================== */

START_TEST(test_sprintf_return_value) {
  char s21_buf[128];
  char std_buf[128];

  int s21_ret = s21_sprintf(s21_buf, "%d %s", 42, "abc");
  int std_ret = sprintf(std_buf, "%d %s", 42, "abc");

  ck_assert_int_eq(s21_ret, std_ret);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== float ==================== */

START_TEST(test_sprintf_float_basic) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%f", 3.14159);
  sprintf(std_buf, "%f", 3.14159);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_float_prec) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%.2f", 3.14159);
  sprintf(std_buf, "%.2f", 3.14159);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_float_negative) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%.3f", -2.5);
  sprintf(std_buf, "%.3f", -2.5);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_float_zero) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%.4f", 0.0);
  sprintf(std_buf, "%.4f", 0.0);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_sci) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%.2e", 12345.678);
  sprintf(std_buf, "%.2e", 12345.678);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_sci_upper) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%.3E", 0.000123);
  sprintf(std_buf, "%.3E", 0.000123);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_float_width) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "[%10.2f]", 3.14);
  sprintf(std_buf, "[%10.2f]", 3.14);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_float_left) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "[%-10.2f]", 3.14);
  sprintf(std_buf, "[%-10.2f]", 3.14);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_sprintf_float_plus) {
  char s21_buf[128];
  char std_buf[128];

  s21_sprintf(s21_buf, "%+.2f", 3.14);
  sprintf(std_buf, "%+.2f", 3.14);
  ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== Suite ==================== */

Suite *s21_sprintf_suite(void) {
  Suite *suite = suite_create("s21_sprintf");

  TCase *tc_int = tcase_create("int");
  tcase_add_test(tc_int, test_sprintf_int_basic);
  tcase_add_test(tc_int, test_sprintf_int_negative);
  tcase_add_test(tc_int, test_sprintf_int_zero);
  tcase_add_test(tc_int, test_sprintf_int_int_min);
  tcase_add_test(tc_int, test_sprintf_int_int_max);
  tcase_add_test(tc_int, test_sprintf_i_specifier);
  suite_add_tcase(suite, tc_int);

  TCase *tc_flags = tcase_create("flags");
  tcase_add_test(tc_flags, test_sprintf_flag_plus);
  tcase_add_test(tc_flags, test_sprintf_flag_space);
  tcase_add_test(tc_flags, test_sprintf_flag_minus);
  tcase_add_test(tc_flags, test_sprintf_flag_zero);
  tcase_add_test(tc_flags, test_sprintf_flag_zero_negative);
  suite_add_tcase(suite, tc_flags);

  TCase *tc_width = tcase_create("width");
  tcase_add_test(tc_width, test_sprintf_width_basic);
  tcase_add_test(tc_width, test_sprintf_width_star);
  tcase_add_test(tc_width, test_sprintf_width_star_negative);
  suite_add_tcase(suite, tc_width);

  TCase *tc_prec = tcase_create("precision");
  tcase_add_test(tc_prec, test_sprintf_prec_basic);
  tcase_add_test(tc_prec, test_sprintf_prec_zero_value);
  tcase_add_test(tc_prec, test_sprintf_prec_star);
  suite_add_tcase(suite, tc_prec);

  TCase *tc_radix = tcase_create("radix");
  tcase_add_test(tc_radix, test_sprintf_unsigned);
  tcase_add_test(tc_radix, test_sprintf_hex);
  tcase_add_test(tc_radix, test_sprintf_hex_hash);
  tcase_add_test(tc_radix, test_sprintf_octal);
  suite_add_tcase(suite, tc_radix);

  TCase *tc_length = tcase_create("length");
  tcase_add_test(tc_length, test_sprintf_long);
  tcase_add_test(tc_length, test_sprintf_long_negative);
  tcase_add_test(tc_length, test_sprintf_short);
  tcase_add_test(tc_length, test_sprintf_unsigned_long);
  suite_add_tcase(suite, tc_length);

  TCase *tc_str = tcase_create("string");
  tcase_add_test(tc_str, test_sprintf_string);
  tcase_add_test(tc_str, test_sprintf_string_width);
  tcase_add_test(tc_str, test_sprintf_string_left);
  tcase_add_test(tc_str, test_sprintf_string_prec);
  tcase_add_test(tc_str, test_sprintf_string_empty);
  suite_add_tcase(suite, tc_str);

  TCase *tc_char = tcase_create("char");
  tcase_add_test(tc_char, test_sprintf_char);
  tcase_add_test(tc_char, test_sprintf_char_width);
  suite_add_tcase(suite, tc_char);

  TCase *tc_percent = tcase_create("percent");
  tcase_add_test(tc_percent, test_sprintf_percent);
  suite_add_tcase(suite, tc_percent);

  TCase *tc_mixed = tcase_create("mixed");
  tcase_add_test(tc_mixed, test_sprintf_mixed);
  tcase_add_test(tc_mixed, test_sprintf_complex_format);
  tcase_add_test(tc_mixed, test_sprintf_return_value);
  suite_add_tcase(suite, tc_mixed);

  TCase *tc_float = tcase_create("float");
  tcase_add_test(tc_float, test_sprintf_float_basic);
  tcase_add_test(tc_float, test_sprintf_float_prec);
  tcase_add_test(tc_float, test_sprintf_float_negative);
  tcase_add_test(tc_float, test_sprintf_float_zero);
  tcase_add_test(tc_float, test_sprintf_sci);
  tcase_add_test(tc_float, test_sprintf_sci_upper);
  tcase_add_test(tc_float, test_sprintf_float_width);
  tcase_add_test(tc_float, test_sprintf_float_left);
  tcase_add_test(tc_float, test_sprintf_float_plus);
  suite_add_tcase(suite, tc_float);

  return suite;
}