#ifndef S21_STRING_H
#define S21_STRING_H

#include <stdio.h>

typedef struct {
  size_t count;
  size_t capacity;
  char *array;
} string;

string init_string(size_t capacity);
void append(string *str, const char *add);  // add to end
void insert(string *str, const char *add);  // add to start
void free_str(string str);

#endif