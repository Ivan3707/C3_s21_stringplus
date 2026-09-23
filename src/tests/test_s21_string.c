#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "s21_string.h"

/* ==================== strlen ==================== */

START_TEST(test_strlen_normal) {
    const char *str = "Hello, world!";
    ck_assert_uint_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(test_strlen_empty) {
    const char *str = "";
    ck_assert_uint_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(test_strlen_spaces) {
    const char *str = "   hello   world   ";
    ck_assert_uint_eq(s21_strlen(str), strlen(str));
}
END_TEST

/* ==================== memset ==================== */

START_TEST(test_memset_normal) {
    char s21_buf[20] = "hello";
    char std_buf[20] = "hello";

    ck_assert_ptr_eq(
        s21_memset(s21_buf, 'X', 3),
        s21_buf
    );
    ck_assert_ptr_eq(
        memset(std_buf, 'X', 3),
        std_buf
    );

    ck_assert_mem_eq(s21_buf, std_buf, sizeof(s21_buf));
}
END_TEST

START_TEST(test_memset_zero_length) {
    char s21_buf[] = "hello";
    char std_buf[] = "hello";

    s21_memset(s21_buf, 'X', 0);
    memset(std_buf, 'X', 0);

    ck_assert_mem_eq(s21_buf, std_buf, sizeof(s21_buf));
}
END_TEST

START_TEST(test_memset_large_value) {
    unsigned char s21_buf[5] = {0};
    unsigned char std_buf[5] = {0};

    s21_memset(s21_buf, 300, sizeof(s21_buf));
    memset(std_buf, 300, sizeof(std_buf));

    ck_assert_mem_eq(s21_buf, std_buf, sizeof(s21_buf));
}
END_TEST

/* ==================== memcmp ==================== */

START_TEST(test_memcmp_equal) {
    const char *a = "hello";
    const char *b = "hello";

    ck_assert_int_eq(s21_memcmp(a, b, 5), 0);
}
END_TEST

START_TEST(test_memcmp_less) {
    const char *a = "abc";
    const char *b = "abd";

    int s21_result = s21_memcmp(a, b, 3);
    int std_result = memcmp(a, b, 3);

    ck_assert_int_lt(s21_result, 0);
    ck_assert_int_lt(std_result, 0);
}
END_TEST

START_TEST(test_memcmp_greater) {
    const char *a = "abd";
    const char *b = "abc";

    int s21_result = s21_memcmp(a, b, 3);
    int std_result = memcmp(a, b, 3);

    ck_assert_int_gt(s21_result, 0);
    ck_assert_int_gt(std_result, 0);
}
END_TEST

START_TEST(test_memcmp_zero_length) {
    const char *a = "abc";
    const char *b = "xyz";

    ck_assert_int_eq(s21_memcmp(a, b, 0), memcmp(a, b, 0));
}
END_TEST

START_TEST(test_memcmp_unsigned_char) {
    unsigned char a[] = {0xFF};
    unsigned char b[] = {0x01};

    int s21_result = s21_memcmp(a, b, 1);
    int std_result = memcmp(a, b, 1);

    ck_assert_int_gt(s21_result, 0);
    ck_assert_int_gt(std_result, 0);
}
END_TEST

/* ==================== memchr ==================== */

START_TEST(test_memchr_found) {
    char str[] = "hello";

    void *s21_result = s21_memchr(str, 'l', 5);
    void *std_result = memchr(str, 'l', 5);

    ck_assert_ptr_eq(s21_result, std_result);
}
END_TEST

START_TEST(test_memchr_not_found) {
    char str[] = "hello";

    ck_assert_ptr_eq(
        s21_memchr(str, 'x', 5),
        memchr(str, 'x', 5)
    );
}
END_TEST

START_TEST(test_memchr_null_character) {
    char str[] = "hello";

    void *s21_result = s21_memchr(str, '\0', 6);
    void *std_result = memchr(str, '\0', 6);

    ck_assert_ptr_eq(s21_result, std_result);
}
END_TEST

START_TEST(test_memchr_zero_length) {
    char str[] = "hello";

    ck_assert_ptr_eq(
        s21_memchr(str, 'h', 0),
        memchr(str, 'h', 0)
    );
}
END_TEST

/* ==================== memcpy ==================== */

START_TEST(test_memcpy_normal) {
    char s21_buf[20] = {0};
    char std_buf[20] = {0};
    const char *source = "hello";

    ck_assert_ptr_eq(
        s21_memcpy(s21_buf, source, 6),
        s21_buf
    );
    ck_assert_ptr_eq(
        memcpy(std_buf, source, 6),
        std_buf
    );

    ck_assert_mem_eq(s21_buf, std_buf, sizeof(s21_buf));
}
END_TEST

START_TEST(test_memcpy_partial) {
    char s21_buf[20] = "abcdefgh";
    char std_buf[20] = "abcdefgh";
    const char *source = "XYZ";

    s21_memcpy(s21_buf + 2, source, 3);
    memcpy(std_buf + 2, source, 3);

    ck_assert_mem_eq(s21_buf, std_buf, sizeof(s21_buf));
}
END_TEST

START_TEST(test_memcpy_zero_length) {
    char s21_buf[] = "hello";
    char std_buf[] = "hello";

    s21_memcpy(s21_buf, "XYZ", 0);
    memcpy(std_buf, "XYZ", 0);

    ck_assert_mem_eq(s21_buf, std_buf, sizeof(s21_buf));
}
END_TEST

/* ==================== strcspn ==================== */

START_TEST(test_strcspn_normal) {
    const char *a = "abc123";
    const char *b = "0123456789";

    ck_assert_uint_eq(s21_strcspn(a, b), strcspn(a, b));
}
END_TEST

START_TEST(test_strcspn_no_match) {
    const char *a = "abcdef";
    const char *b = "123";

    ck_assert_uint_eq(s21_strcspn(a, b), strcspn(a, b));
}
END_TEST

START_TEST(test_strcspn_first_character) {
    const char *a = "hello";
    const char *b = "h";

    ck_assert_uint_eq(s21_strcspn(a, b), strcspn(a, b));
}
END_TEST

START_TEST(test_strcspn_empty_reject) {
    const char *a = "hello";
    const char *b = "";

    ck_assert_uint_eq(s21_strcspn(a, b), strcspn(a, b));
}
END_TEST

/* ==================== strchr ==================== */

START_TEST(test_strchr_found) {
    char str[] = "hello";

    char *s21_result = s21_strchr(str, 'l');
    char *std_result = strchr(str, 'l');

    ck_assert_int_eq(
        s21_result - str,
        std_result - str
    );
}
END_TEST

START_TEST(test_strchr_not_found) {
    char str[] = "hello";

    ck_assert_ptr_eq(
        s21_strchr(str, 'x'),
        strchr(str, 'x')
    );
}
END_TEST

START_TEST(test_strchr_null_character) {
    char str[] = "hello";

    ck_assert_ptr_eq(
        s21_strchr(str, '\0'),
        strchr(str, '\0')
    );
}
END_TEST

START_TEST(test_strchr_first_character) {
    char str[] = "hello";

    ck_assert_int_eq(
        s21_strchr(str, 'h') - str,
        strchr(str, 'h') - str
    );
}
END_TEST

/* ==================== strrchr ==================== */

START_TEST(test_strrchr_found) {
    char str[] = "hello";

    char *s21_result = s21_strrchr(str, 'l');
    char *std_result = strrchr(str, 'l');

    ck_assert_int_eq(
        s21_result - str,
        std_result - str
    );
}
END_TEST

START_TEST(test_strrchr_not_found) {
    char str[] = "hello";

    ck_assert_ptr_eq(
        s21_strrchr(str, 'x'),
        strrchr(str, 'x')
    );
}
END_TEST

START_TEST(test_strrchr_null_character) {
    char str[] = "hello";

    ck_assert_ptr_eq(
        s21_strrchr(str, '\0'),
        strrchr(str, '\0')
    );
}
END_TEST

START_TEST(test_strrchr_first_character) {
    char str[] = "hello";

    ck_assert_int_eq(
        s21_strrchr(str, 'h') - str,
        strrchr(str, 'h') - str
    );
}
END_TEST

/* ==================== strncmp ==================== */

START_TEST(test_strncmp_equal) {
    ck_assert_int_eq(
        s21_strncmp("hello", "hello", 5),
        strncmp("hello", "hello", 5)
    );
}
END_TEST

START_TEST(test_strncmp_less) {
    int s21_result = s21_strncmp("abc", "abd", 3);
    int std_result = strncmp("abc", "abd", 3);

    ck_assert_int_lt(s21_result, 0);
    ck_assert_int_lt(std_result, 0);
}
END_TEST

START_TEST(test_strncmp_greater) {
    int s21_result = s21_strncmp("abd", "abc", 3);
    int std_result = strncmp("abd", "abc", 3);

    ck_assert_int_gt(s21_result, 0);
    ck_assert_int_gt(std_result, 0);
}
END_TEST

START_TEST(test_strncmp_zero_length) {
    ck_assert_int_eq(
        s21_strncmp("abc", "xyz", 0),
        strncmp("abc", "xyz", 0)
    );
}
END_TEST

START_TEST(test_strncmp_shorter_string) {
    int s21_result = s21_strncmp("ab", "abc", 3);
    int std_result = strncmp("ab", "abc", 3);

    ck_assert(s21_result < 0);
    ck_assert(std_result < 0);
}
END_TEST

/* ==================== strncpy ==================== */

START_TEST(test_strncpy_source_shorter) {
    char s21_buf[10] = {0};
    char std_buf[10] = {0};

    s21_strncpy(s21_buf, "abc", 6);
    strncpy(std_buf, "abc", 6);

    ck_assert_mem_eq(s21_buf, std_buf, sizeof(s21_buf));
}
END_TEST

START_TEST(test_strncpy_exact_length) {
    char s21_buf[10] = {0};
    char std_buf[10] = {0};

    s21_strncpy(s21_buf, "hello", 5);
    strncpy(std_buf, "hello", 5);

    ck_assert_mem_eq(s21_buf, std_buf, 5);
}
END_TEST

START_TEST(test_strncpy_source_longer) {
    char s21_buf[10] = {0};
    char std_buf[10] = {0};

    s21_strncpy(s21_buf, "abcdefgh", 4);
    strncpy(std_buf, "abcdefgh", 4);

    ck_assert_mem_eq(s21_buf, std_buf, sizeof(s21_buf));
}
END_TEST

START_TEST(test_strncpy_zero_length) {
    char s21_buf[] = "hello";
    char std_buf[] = "hello";

    s21_strncpy(s21_buf, "XYZ", 0);
    strncpy(std_buf, "XYZ", 0);

    ck_assert_mem_eq(s21_buf, std_buf, sizeof(s21_buf));
}
END_TEST

/* ==================== strncat ==================== */

START_TEST(test_strncat_normal) {
    char s21_buf[20] = "hello";
    char std_buf[20] = "hello";

    s21_strncat(s21_buf, " world", 6);
    strncat(std_buf, " world", 6);

    ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_strncat_partial) {
    char s21_buf[20] = "hello";
    char std_buf[20] = "hello";

    s21_strncat(s21_buf, "abcdef", 3);
    strncat(std_buf, "abcdef", 3);

    ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_strncat_zero_length) {
    char s21_buf[20] = "hello";
    char std_buf[20] = "hello";

    s21_strncat(s21_buf, "world", 0);
    strncat(std_buf, "world", 0);

    ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

START_TEST(test_strncat_empty_source) {
    char s21_buf[20] = "hello";
    char std_buf[20] = "hello";

    s21_strncat(s21_buf, "", 5);
    strncat(std_buf, "", 5);

    ck_assert_str_eq(s21_buf, std_buf);
}
END_TEST

/* ==================== strpbrk ==================== */

START_TEST(test_strpbrk_found) {
    char str[] = "hello";

    char *s21_result = s21_strpbrk(str, "xyzl");
    char *std_result = strpbrk(str, "xyzl");

    ck_assert_int_eq(
        s21_result - str,
        std_result - str
    );
}
END_TEST

START_TEST(test_strpbrk_not_found) {
    char str[] = "hello";

    ck_assert_ptr_eq(
        s21_strpbrk(str, "xyz"),
        strpbrk(str, "xyz")
    );
}
END_TEST

START_TEST(test_strpbrk_first_character) {
    char str[] = "hello";

    ck_assert_int_eq(
        s21_strpbrk(str, "h") - str,
        strpbrk(str, "h") - str
    );
}
END_TEST

START_TEST(test_strpbrk_empty_set) {
    char str[] = "hello";

    ck_assert_ptr_eq(
        s21_strpbrk(str, ""),
        strpbrk(str, "")
    );
}
END_TEST

/* ==================== strstr ==================== */

START_TEST(test_strstr_found) {
    char s21_buf[] = "hello world";
    char std_buf[] = "hello world";

    char *s21_result = s21_strstr(s21_buf, "world");
    char *std_result = strstr(std_buf, "world");

    ck_assert_int_eq(
        s21_result - s21_buf,
        std_result - std_buf
    );
}
END_TEST

START_TEST(test_strstr_not_found) {
    char s21_buf[] = "hello world";
    char std_buf[] = "hello world";

    ck_assert_ptr_eq(
        s21_strstr(s21_buf, "xyz"),
        strstr(std_buf, "xyz")
    );
}
END_TEST

START_TEST(test_strstr_beginning) {
    char s21_buf[] = "hello world";
    char std_buf[] = "hello world";

    ck_assert_int_eq(
        s21_strstr(s21_buf, "hello") - s21_buf,
        strstr(std_buf, "hello") - std_buf
    );
}
END_TEST

START_TEST(test_strstr_empty_needle) {
    char s21_buf[] = "hello";
    char std_buf[] = "hello";

    ck_assert_ptr_eq(
        s21_strstr(s21_buf, ""),
        s21_buf
    );
    ck_assert_ptr_eq(
        strstr(std_buf, ""),
        std_buf
    );
}
END_TEST

/* ==================== strtok ==================== */

START_TEST(test_strtok_normal) {
    char s21_buf[] = "one,two,three";
    char std_buf[] = "one,two,three";

    char *s21_token = s21_strtok(s21_buf, ",");
    char *std_token = strtok(std_buf, ",");

    while (s21_token != NULL || std_token != NULL) {
        ck_assert_ptr_ne(s21_token, NULL);
        ck_assert_ptr_ne(std_token, NULL);
        ck_assert_str_eq(s21_token, std_token);

        s21_token = s21_strtok(S21_NULL, ",");
        std_token = strtok(NULL, ",");
    }
}
END_TEST

START_TEST(test_strtok_repeated_delimiters) {
    char s21_buf[] = ",,one,,,two,,";
    char std_buf[] = ",,one,,,two,,";

    char *s21_token = s21_strtok(s21_buf, ",");
    char *std_token = strtok(std_buf, ",");

    while (s21_token != NULL || std_token != NULL) {
        ck_assert_ptr_ne(s21_token, NULL);
        ck_assert_ptr_ne(std_token, NULL);
        ck_assert_str_eq(s21_token, std_token);

        s21_token = s21_strtok(S21_NULL, ",");
        std_token = strtok(NULL, ",");
    }
}
END_TEST

START_TEST(test_strtok_empty_string) {
    char s21_buf[] = "";
    char std_buf[] = "";

    ck_assert_ptr_eq(
        s21_strtok(s21_buf, ","),
        strtok(std_buf, ",")
    );
}
END_TEST

START_TEST(test_strtok_multiple_delimiters) {
    char s21_buf[] = "one:two;three,four";
    char std_buf[] = "one:two;three,four";

    const char *delim = ":;,";

    char *s21_token = s21_strtok(s21_buf, delim);
    char *std_token = strtok(std_buf, delim);

    while (s21_token != NULL || std_token != NULL) {
        ck_assert_ptr_ne(s21_token, NULL);
        ck_assert_ptr_ne(std_token, NULL);
        ck_assert_str_eq(s21_token, std_token);

        s21_token = s21_strtok(S21_NULL, delim);
        std_token = strtok(NULL, delim);
    }
}
END_TEST

/* ==================== strerror ==================== */

START_TEST(test_strerror_zero) {
#if defined(__linux__) || defined(__APPLE__)
    ck_assert_str_eq(s21_strerror(0), strerror(0));
#else
    ck_assert_str_eq(s21_strerror(0), "Unknown error: 0");
#endif
}
END_TEST

START_TEST(test_strerror_known_error) {
#if defined(__linux__) || defined(__APPLE__)
    ck_assert_str_eq(s21_strerror(2), strerror(2));
#else
    ck_assert_str_eq(s21_strerror(2), "Unknown error: 2");
#endif
}
END_TEST

START_TEST(test_strerror_additional_errors) {
#if defined(__linux__)
    ck_assert_str_eq(s21_strerror(13), strerror(13));
    ck_assert_str_eq(s21_strerror(22), strerror(22));
    ck_assert_str_eq(s21_strerror(32), strerror(32));
    ck_assert_str_eq(s21_strerror(40), strerror(40));
    ck_assert_str_eq(s21_strerror(42), strerror(42));
    ck_assert_str_eq(s21_strerror(98), strerror(98));
    ck_assert_str_eq(s21_strerror(104), strerror(104));
    ck_assert_str_eq(s21_strerror(110), strerror(110));
    ck_assert_str_eq(s21_strerror(111), strerror(111));
    ck_assert_str_eq(s21_strerror(125), strerror(125));
    ck_assert_str_eq(s21_strerror(133), strerror(133));
#elif defined(__APPLE__)
    ck_assert_str_eq(s21_strerror(1), strerror(1));
    ck_assert_str_eq(s21_strerror(13), strerror(13));
    ck_assert_str_eq(s21_strerror(22), strerror(22));
    ck_assert_str_eq(s21_strerror(35), strerror(35));
    ck_assert_str_eq(s21_strerror(48), strerror(48));
    ck_assert_str_eq(s21_strerror(61), strerror(61));
    ck_assert_str_eq(s21_strerror(73), strerror(73));
    ck_assert_str_eq(s21_strerror(89), strerror(89));
    ck_assert_str_eq(s21_strerror(106), strerror(106));
#else
    ck_assert_str_eq(s21_strerror(13), "Unknown error: 13");
    ck_assert_str_eq(s21_strerror(22), "Unknown error: 22");
    ck_assert_str_eq(s21_strerror(133), "Unknown error: 133");
#endif
}
END_TEST

START_TEST(test_strerror_unknown_positive) {
#if defined(__linux__) || defined(__APPLE__)
    ck_assert_str_eq(
        s21_strerror(999),
        "Unknown error: 999"
    );
#else
    ck_assert_str_eq(
        s21_strerror(999),
        "Unknown error: 999"
    );
#endif
}
END_TEST

START_TEST(test_strerror_negative) {
    ck_assert_str_eq(
        s21_strerror(-1),
        "Unknown error: -1"
    );
}
END_TEST

/* ==================== Suites ==================== */

Suite *s21_string_suite(void) {
    Suite *suite = suite_create("s21_string");

    TCase *tc_strlen = tcase_create("strlen");
    tcase_add_test(tc_strlen, test_strlen_normal);
    tcase_add_test(tc_strlen, test_strlen_empty);
    tcase_add_test(tc_strlen, test_strlen_spaces);
    suite_add_tcase(suite, tc_strlen);

    TCase *tc_memset = tcase_create("memset");
    tcase_add_test(tc_memset, test_memset_normal);
    tcase_add_test(tc_memset, test_memset_zero_length);
    tcase_add_test(tc_memset, test_memset_large_value);
    suite_add_tcase(suite, tc_memset);

    TCase *tc_memcmp = tcase_create("memcmp");
    tcase_add_test(tc_memcmp, test_memcmp_equal);
    tcase_add_test(tc_memcmp, test_memcmp_less);
    tcase_add_test(tc_memcmp, test_memcmp_greater);
    tcase_add_test(tc_memcmp, test_memcmp_zero_length);
    tcase_add_test(tc_memcmp, test_memcmp_unsigned_char);
    suite_add_tcase(suite, tc_memcmp);

    TCase *tc_memchr = tcase_create("memchr");
    tcase_add_test(tc_memchr, test_memchr_found);
    tcase_add_test(tc_memchr, test_memchr_not_found);
    tcase_add_test(tc_memchr, test_memchr_null_character);
    tcase_add_test(tc_memchr, test_memchr_zero_length);
    suite_add_tcase(suite, tc_memchr);

    TCase *tc_memcpy = tcase_create("memcpy");
    tcase_add_test(tc_memcpy, test_memcpy_normal);
    tcase_add_test(tc_memcpy, test_memcpy_partial);
    tcase_add_test(tc_memcpy, test_memcpy_zero_length);
    suite_add_tcase(suite, tc_memcpy);

    TCase *tc_strcspn = tcase_create("strcspn");
    tcase_add_test(tc_strcspn, test_strcspn_normal);
    tcase_add_test(tc_strcspn, test_strcspn_no_match);
    tcase_add_test(tc_strcspn, test_strcspn_first_character);
    tcase_add_test(tc_strcspn, test_strcspn_empty_reject);
    suite_add_tcase(suite, tc_strcspn);

    TCase *tc_strchr = tcase_create("strchr");
    tcase_add_test(tc_strchr, test_strchr_found);
    tcase_add_test(tc_strchr, test_strchr_not_found);
    tcase_add_test(tc_strchr, test_strchr_null_character);
    tcase_add_test(tc_strchr, test_strchr_first_character);
    suite_add_tcase(suite, tc_strchr);

    TCase *tc_strrchr = tcase_create("strrchr");
    tcase_add_test(tc_strrchr, test_strrchr_found);
    tcase_add_test(tc_strrchr, test_strrchr_not_found);
    tcase_add_test(tc_strrchr, test_strrchr_null_character);
    tcase_add_test(tc_strrchr, test_strrchr_first_character);
    suite_add_tcase(suite, tc_strrchr);

    TCase *tc_strncmp = tcase_create("strncmp");
    tcase_add_test(tc_strncmp, test_strncmp_equal);
    tcase_add_test(tc_strncmp, test_strncmp_less);
    tcase_add_test(tc_strncmp, test_strncmp_greater);
    tcase_add_test(tc_strncmp, test_strncmp_zero_length);
    tcase_add_test(tc_strncmp, test_strncmp_shorter_string);
    suite_add_tcase(suite, tc_strncmp);

    TCase *tc_strncpy = tcase_create("strncpy");
    tcase_add_test(tc_strncpy, test_strncpy_source_shorter);
    tcase_add_test(tc_strncpy, test_strncpy_exact_length);
    tcase_add_test(tc_strncpy, test_strncpy_source_longer);
    tcase_add_test(tc_strncpy, test_strncpy_zero_length);
    suite_add_tcase(suite, tc_strncpy);

    TCase *tc_strncat = tcase_create("strncat");
    tcase_add_test(tc_strncat, test_strncat_normal);
    tcase_add_test(tc_strncat, test_strncat_partial);
    tcase_add_test(tc_strncat, test_strncat_zero_length);
    tcase_add_test(tc_strncat, test_strncat_empty_source);
    suite_add_tcase(suite, tc_strncat);

    TCase *tc_strpbrk = tcase_create("strpbrk");
    tcase_add_test(tc_strpbrk, test_strpbrk_found);
    tcase_add_test(tc_strpbrk, test_strpbrk_not_found);
    tcase_add_test(tc_strpbrk, test_strpbrk_first_character);
    tcase_add_test(tc_strpbrk, test_strpbrk_empty_set);
    suite_add_tcase(suite, tc_strpbrk);

    TCase *tc_strstr = tcase_create("strstr");
    tcase_add_test(tc_strstr, test_strstr_found);
    tcase_add_test(tc_strstr, test_strstr_not_found);
    tcase_add_test(tc_strstr, test_strstr_beginning);
    tcase_add_test(tc_strstr, test_strstr_empty_needle);
    suite_add_tcase(suite, tc_strstr);

    TCase *tc_strtok = tcase_create("strtok");
    tcase_add_test(tc_strtok, test_strtok_normal);
    tcase_add_test(tc_strtok, test_strtok_repeated_delimiters);
    tcase_add_test(tc_strtok, test_strtok_empty_string);
    tcase_add_test(tc_strtok, test_strtok_multiple_delimiters);
    suite_add_tcase(suite, tc_strtok);

    TCase *tc_strerror = tcase_create("strerror");
    tcase_add_test(tc_strerror, test_strerror_zero);
    tcase_add_test(tc_strerror, test_strerror_known_error);
    tcase_add_test(tc_strerror, test_strerror_additional_errors);
    tcase_add_test(tc_strerror, test_strerror_unknown_positive);
    tcase_add_test(tc_strerror, test_strerror_negative);
    suite_add_tcase(suite, tc_strerror);

    return suite;
}

int main(void) {
    Suite *suite = s21_string_suite();
    SRunner *runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);

    int failed = srunner_ntests_failed(runner);

    srunner_free(runner);
    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}