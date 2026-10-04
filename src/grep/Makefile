CC=gcc
FLAGS= -Wall -Wextra -Werror -std=c11 -pedantic

all: s21_grep

s21_grep: s21_string.o s21_grep.o
	$(CC) $(FLAGS) s21_grep.o s21_string.o -o s21_grep

s21_string.o: s21_string.c
	$(CC) $(FLAGS) -c s21_string.c

s21_grep.o: s21_grep.c
	$(CC) $(FLAGS) -c s21_grep.c

test: all
	./test.sh

clean:
	rm -rf s21_string.o s21_grep.o s21_grep

rebuild: clean all
