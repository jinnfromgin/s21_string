CFLAGS = -c -std=c11 -Wall -Werror -Wextra
CC= gcc
FLAGS = -std=c11
SOURCE = s21_*.c
OS := $(shell uname -s)
ifeq ($(OS), Linux)
LDFLAGS=-lcheck -lm -lsubunit
endif
ifeq ($(OS), Darwin)
LDFLAGS=-lcheck -lm
endif

all: s21_string.a 

s21_string.a:
	$(CC) $(CFLAGS) $(SOURCE)
	ar rc s21_string.a s21_*.o
	ranlib s21_string.a
	rm -rf *.o

test: s21_string.a 
	checkmk test.check > test.c
	checkmk clean_mode=1 test.check > test.c 
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c 
	rm -rf .clang-format
	$(CC) $(FLAGS) -o s21_test test.c s21_string.a $(LDFLAGS)
	./s21_test

clang :
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c *.h
	clang-format -n *.c *.h
	rm -rf .clang-format

gcov_report: $(SOURCE) 
	checkmk test.check > test.c
	checkmk clean_mode=1 test.check > test.c 
	cp ../materials/linters/.clang-format .clang-format
	clang-format -i *.c 
	rm -rf .clang-format
	$(CC) --coverage $(SOURCE) test.c -o s21_test -lcheck -lm
	./s21_test
	lcov -t "s21_test" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	open ./report/index.html
	rm -rf *.gcno *.gcda *.info s21_test

clean: 
	rm -rf *.a *.o test *.gcno *.gcda *.info s21_test report test.c