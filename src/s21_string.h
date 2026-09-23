#ifndef S21_STRING_H
#define S21_STRING_H

typedef unsigned long s21_size;

#define S21_NULL ((void *)0)

s21_size s21_strlen(const char *str);

void *s21_memset(void *destination, int value, s21_size n);
int s21_memcmp(const void *str1, const void *str2, s21_size n);
void *s21_memchr(const void *str, int c, s21_size n);
void *s21_memcpy(void *destination, const void *source, s21_size n);

s21_size s21_strcspn(const char *str1, const char *str2);
char *s21_strchr(const char *str, int c);
char *s21_strrchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size n);
char *s21_strncpy(char *destination, const char *source, s21_size n);
char *s21_strncat(char *destination, const char *source, s21_size n);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);
char *s21_strerror(int errnum);

#endif