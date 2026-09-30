#include <check.h>
#include <stdlib.h>

Suite *s21_string_suite(void);
Suite *s21_sprintf_suite(void);
Suite *s21_sscanf_suite(void);

int main(void) {
  Suite *string_suite = s21_string_suite();
  Suite *sprintf_suite = s21_sprintf_suite();
  Suite *sscanf_suite = s21_sscanf_suite();

  SRunner *runner = srunner_create(string_suite);
  srunner_add_suite(runner, sprintf_suite);
  srunner_add_suite(runner, sscanf_suite);

  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed == 0 ? 0 : 1;
}