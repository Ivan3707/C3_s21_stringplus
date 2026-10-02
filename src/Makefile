.RECIPEPREFIX = >

CC = gcc
AR = ar
ARFLAGS = rcs

CFLAGS = -std=c11 -Wall -Wextra -Werror
TEST_CFLAGS = -std=c11 -Wall -Wextra -Wno-memset-transposed-args -Wno-stringop-truncation -Wno-stringop-overflow -I.

COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage

ifeq ($(OS),Windows_NT)
CHECK_INC = -I"C:/msys64/ucrt64/include"
CHECK_LIB = -L"C:/msys64/ucrt64/lib" -lcheck
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
CHECK_INC =
CHECK_LIB = -lcheck -lsubunit -lm -lpthread -lrt
endif
ifeq ($(UNAME_S),Darwin)
CHECK_INC =
CHECK_LIB = -lcheck -lm -lpthread
endif
endif

TARGET = s21_string.a
SRC = s21_string.c s21_sscanf.c s21_sprintf.c
OBJ = $(SRC:.c=.o)

TEST_SRC = tests/test_runner.c tests/test_string.c tests/test_sscanf.c tests/test_sprintf.c
TEST_BIN = test.exe

.PHONY: all clean test gcov_report rebuild

all: $(TARGET)

$(TARGET): $(OBJ)
> $(AR) $(ARFLAGS) $@ $^
> rm -f $(OBJ)

%.o: %.c s21_string.h
> $(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_SRC) $(TARGET) s21_string.h
> $(CC) $(TEST_CFLAGS) $(CHECK_INC) $(TEST_SRC) $(TARGET) $(CHECK_LIB) -o $(TEST_BIN)

test: $(TEST_BIN)
> ./$(TEST_BIN)

gcov_report:
> $(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c s21_string.c -o s21_string.o
> $(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c s21_sscanf.c -o s21_sscanf.o
> $(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c s21_sprintf.c -o s21_sprintf.o
> $(AR) $(ARFLAGS) $(TARGET) s21_string.o s21_sscanf.o s21_sprintf.o
> $(CC) $(TEST_CFLAGS) $(COVERAGE_FLAGS) $(CHECK_INC) $(TEST_SRC) $(TARGET) $(CHECK_LIB) -o $(TEST_BIN)
> ./$(TEST_BIN)
> gcovr --root . --filter 's21_.*\.c' --html --html-self-contained --output coverage.html
> gcovr --root . --filter 's21_.*\.c' --print-summary
> rm -f $(TARGET) s21_string.o s21_sscanf.o s21_sprintf.o $(TEST_BIN)
> rm -f *.gcda *.gcno *.gcov

clean:
> rm -f $(TARGET) $(OBJ) $(TEST_BIN)
> rm -f *.gcda *.gcno *.gcov
> rm -f coverage.html coverage.css coverage.js coverage.functions.html
> rm -f coverage.*.html

rebuild: clean all