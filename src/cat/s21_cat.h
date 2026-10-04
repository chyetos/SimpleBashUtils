#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>

typedef struct cat_flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} cat_flags;

#define MAX_FILES 256
#define MAX_LINE 1024
#define LONG_OPTIONS 4

int read_flags(int argc, char **argv, const char *short_options,
               const struct option long_options[LONG_OPTIONS],
               struct cat_flags *flags);
void print_file(struct cat_flags flags, char *file_name, int *met_empty,
                int *lines_count, int *new_line);
void output_file(cat_flags flags, FILE *fptr, int *met_empty, int *lines_count,
                 int *new_line);

#endif