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

CHECK_INC = -I"C:/msys64/ucrt64/include"
CHECK_LIB = -L"C:/msys64/ucrt64/lib" -lcheck

TARGET = s21_string.a
SRC = s21_string.c
OBJ = s21_string.o

TEST_SRC = tests/test_s21_string.c
TEST_BIN = test.exe

.PHONY: all clean test gcov_report rebuild

all: $(TARGET)

$(TARGET): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(OBJ): $(SRC) s21_string.h
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

$(TEST_BIN): $(TEST_SRC) $(TARGET) s21_string.h
	$(CC) $(TEST_CFLAGS) $(CHECK_INC) $(TEST_SRC) $(TARGET) $(CHECK_LIB) -o $(TEST_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

gcov_report:
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c $(SRC) -o $(OBJ)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJ)
	$(CC) $(TEST_CFLAGS) $(COVERAGE_FLAGS) $(CHECK_INC) \
		$(TEST_SRC) $(TARGET) $(CHECK_LIB) -o $(TEST_BIN)
	./$(TEST_BIN)
	gcovr --root . --filter 's21_string\.c' --html --html-self-contained --output coverage.html
	gcovr --root . --filter 's21_string\.c' --print-summary

clean:
	rm -f $(TARGET) $(OBJ) $(TEST_BIN)
	rm -f *.gcda *.gcno *.gcov
	rm -f coverage.html coverage.css coverage.js coverage.functions.html
	rm -f coverage.*.html

rebuild: clean all