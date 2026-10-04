#include "s21_string.h"

#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 128

void expand(string *str);

string init_string(size_t capacity) {
  string str = {0};
  str.capacity = capacity;
  str.count = 0;
  str.array = calloc(sizeof(char), capacity);

  return str;
}

void append(string *str, const char *add) {
  size_t add_size = strlen(add);
  while (str->count + add_size > str->capacity) expand(str);

  str->count += add_size;
  strcat(str->array, add);
}

void insert(string *str, const char *add) {
  size_t add_size = strlen(add);
  while (str->count + add_size > str->capacity) expand(str);

  str->count += add_size;
  char temp[str->count];

  strcpy(temp, add);
  strcat(temp, str->array);
  strcpy(str->array, temp);
}

void free_str(string str) {
  if (str.array != NULL) free(str.array);
}

void expand(string *str) {
  str->capacity *= 2;
  char *temp = str->array;
  str->array = calloc(sizeof(char), str->capacity);
  if (str->array != NULL) {
    strcpy(str->array, temp);
    if (temp != NULL) free(temp);
  }
}
