CC = gcc
AR = ar
ARFLAGS = rcs

CFLAGS = -std=c11 -Wall -Wextra -Werror
TEST_CFLAGS = -std=c11 -Wall -Wextra \
	-Wno-memset-transposed-args \
	-Wno-stringop-truncation \
	-Wno-stringop-overflow \
	-I.

COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage

<<<<<<< HEAD
# === Определение ОС и флагов Check ===
ifeq ($(OS),Windows_NT)
    # Windows (MSYS2 UCRT64)
    CHECK_INC = -I"C:/msys64/ucrt64/include"
    CHECK_LIB = -L"C:/msys64/ucrt64/lib" -lcheck
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        # Linux (Ubuntu/Debian)
        CHECK_INC =
        CHECK_LIB = -lcheck -lsubunit -lm -lpthread -lrt
    endif
    ifeq ($(UNAME_S),Darwin)
        # macOS
        CHECK_INC =
        CHECK_LIB = -lcheck -lm -lpthread
    endif
endif
=======
CHECK_INC = -I"C:/msys64/ucrt64/include"
CHECK_LIB = -L"C:/msys64/ucrt64/lib" -lcheck
>>>>>>> 2947239a248f6f6a9a219a550d405dfa1d143b61

TARGET = s21_string.a
SRC = s21_string.c s21_sscanf.c s21_sprintf.c
OBJ = $(SRC:.c=.o)

TEST_SRC = tests/test_runner.c \
	tests/test_string.c \
	tests/test_sscanf.c \
	tests/test_sprintf.c
TEST_BIN = test.exe

.PHONY: all clean test gcov_report rebuild

all: $(TARGET)

$(TARGET): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^
	rm -f $(OBJ)

%.o: %.c s21_string.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_SRC) $(TARGET) s21_string.h
	$(CC) $(TEST_CFLAGS) $(CHECK_INC) $(TEST_SRC) $(TARGET) $(CHECK_LIB) -o $(TEST_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

gcov_report:
<<<<<<< HEAD
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c s21_string.c -o s21_string.o
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c s21_sscanf.c -o s21_sscanf.o
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c s21_sprintf.c -o s21_sprintf.o
	$(AR) $(ARFLAGS) $(TARGET) s21_string.o s21_sscanf.o s21_sprintf.o
=======
	@for src in $(SRC); do \
		obj=$${src%.c}.o; \
		$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c $$src -o $$obj; \
	done
	$(AR) $(ARFLAGS) $(TARGET) $(OBJ)
>>>>>>> 2947239a248f6f6a9a219a550d405dfa1d143b61
	$(CC) $(TEST_CFLAGS) $(COVERAGE_FLAGS) $(CHECK_INC) \
		$(TEST_SRC) $(TARGET) $(CHECK_LIB) -o $(TEST_BIN)
	./$(TEST_BIN)
	gcovr --root . --filter 's21_.*\.c' --html \
		--html-self-contained --output coverage.html
	gcovr --root . --filter 's21_.*\.c' --print-summary
<<<<<<< HEAD
	rm -f $(TARGET) s21_string.o s21_sscanf.o s21_sprintf.o $(TEST_BIN)
=======
	rm -f $(TARGET) $(OBJ) $(TEST_BIN)
>>>>>>> 2947239a248f6f6a9a219a550d405dfa1d143b61
	rm -f *.gcda *.gcno *.gcov

clean:
	rm -f $(TARGET) $(OBJ) $(TEST_BIN)
	rm -f *.gcda *.gcno *.gcov
	rm -f coverage.html coverage.css coverage.js coverage.functions.html
	rm -f coverage.*.html

<<<<<<< HEAD
rebuild: clean all
=======
rebuild: clean all
>>>>>>> 2947239a248f6f6a9a219a550d405dfa1d143b61
