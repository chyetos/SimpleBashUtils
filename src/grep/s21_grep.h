#ifndef S21_GREP_H
#define S21_GREP_H

#define MAX_TEMPLATES 512

#define MAX_LINE 2048
#define MAX_FILES 32

#include <getopt.h>
#include <regex.h>

#include "s21_string.h"

typedef struct grep_flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} grep_flags;

int read_flags(int argc, char **argv, const char *short_options,
               struct grep_flags *flags, string templates[MAX_TEMPLATES],
               int *templates_count);
void print_file(struct grep_flags flags, char *file_name,
                regex_t templates[MAX_TEMPLATES], int templates_count,
                int print_file_name);
void read_template(int *template_count, string templates[MAX_TEMPLATES],
                   char *file_name);
int print_line(char *line, regex_t templates[MAX_TEMPLATES],
               int templates_count, grep_flags flags);
void output_normal(grep_flags flags, FILE *fptr, char *file_name,
                   regex_t templates[MAX_TEMPLATES], int templates_count,
                   int print_file_name);
void output_count(int count, char *file_name, int print_file_name);
void output_filename(char *file_name);
void output_line(char *line, char *file_name, int line_ind);
void output_file(FILE *fptr, grep_flags flags, char *file_name,
                 regex_t templates[MAX_TEMPLATES], int templates_count,
                 int print_file_name);
void grep_file(grep_flags flags, char *file_name,
               regex_t templates[MAX_TEMPLATES], int templates_count,
               int print_file_name);

#endif