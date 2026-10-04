#include "s21_grep.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_string.h"

void reg_templates(regex_t templates[MAX_TEMPLATES],
                   string templates_str[MAX_TEMPLATES], int templates_count,
                   grep_flags flags) {
  int val = (flags.i ? REG_ICASE : 0) | REG_EXTENDED;
  for (int i = 0; i < templates_count; i++) {
    regcomp(&templates[i], templates_str[i].array, val);
  }
}

int main(int argc, char *argv[]) {
  const char *short_options = "e:iovclnhsf:";
  grep_flags flags = {0};

  regex_t templates[MAX_TEMPLATES] = {0};
  string templates_str[MAX_TEMPLATES];
  for (int i = 0; i < MAX_TEMPLATES; i++) {
    templates_str[i] = init_string(64);
  }

  int templates_count = 0;

  string files[MAX_FILES] = {0};
  for (int i = 0; i < MAX_FILES; i++) files[i] = init_string(64);

  int files_count = 0;

  while (read_flags(argc, argv, short_options, &flags, templates_str,
                    &templates_count) != -1);

  int start_index = optind;
  if (templates_count == 0) {
    append(&templates_str[templates_count++], argv[start_index++]);
  }

  for (int i = start_index; i < argc; i++) {
    append(&files[files_count++], argv[i]);
  }

  reg_templates(templates, templates_str, templates_count, flags);

  for (int i = 0; i < files_count; i++)
    grep_file(flags, files[i].array, templates, templates_count,
              files_count > 1 && !flags.h);

  for (int i = 0; i < MAX_TEMPLATES; i++) {
    free_str(templates_str[i]);
    regfree(&templates[i]);
  }

  for (int i = 0; i < MAX_FILES; i++) free_str(files[i]);
}

void output_line(char *line, char *file_name, int line_ind) {
  char *n = strchr(line, '\n');
  if (file_name != NULL) {
    printf("%s:", file_name);
  }
  if (line_ind != -1) {
    printf("%d:", line_ind);
  }
  printf("%s", line);
  if (n == NULL) putchar('\n');
}

int is_template_in_line(char *line, regex_t templates[MAX_TEMPLATES],
                        int templates_count, grep_flags flags) {
  int found = 0;
  for (int i = 0; i < templates_count; i++) {
    int value = regexec(&templates[i], line, 0, NULL, 0);
    if (value == 0) found = 1;
  }
  if (flags.v) found = !found;
  return found;
}

void print_substrings(char *line, regex_t templates[MAX_TEMPLATES],
                      int templates_count, char *file_name, int line_ind) {
  int status = 0;
  while (status == 0) {
    regmatch_t pmatch;
    int max_so = __INT_MAX__;
    int max_eo = 0;
    for (int i = 0; i < templates_count; i++) {
      int find = regexec(&templates[i], line, 1, &pmatch, 0);
      if (find == 0 && (pmatch.rm_so < max_so ||
                        (pmatch.rm_so == max_so &&
                         pmatch.rm_eo - pmatch.rm_so > max_eo - max_so))) {
        max_so = pmatch.rm_so;
        max_eo = pmatch.rm_eo;
      }
    }
    if (max_eo > 0) {
      char res[MAX_LINE] = {0};
      strncpy(res, line + max_so, max_eo - max_so);
      line += max_eo;

      strcat(res, "\n");
      output_line(res, file_name, line_ind);
    } else {
      status = 1;
    }
  }
}

void output_file(FILE *fptr, grep_flags flags, char *file_name,
                 regex_t templates[MAX_TEMPLATES], int templates_count,
                 int print_file_name) {
  char line[MAX_LINE] = {0};
  int counter = 0;
  int line_ind = 0;
  while (fgets(line, sizeof(line), fptr)) {
    line_ind++;
    int need_to_print = !flags.c && !flags.l;
    int find = is_template_in_line(line, templates, templates_count, flags);
    counter += find;
    if (find && need_to_print) {
      if (flags.o)
        print_substrings(line, templates, templates_count,
                         print_file_name ? file_name : NULL,
                         flags.n ? line_ind : -1);
      else
        output_line(line, print_file_name ? file_name : NULL,
                    flags.n ? line_ind : -1);
    }
  }
  if (flags.l) {
    if (counter > 0) printf("%s\n", file_name);
  } else if (flags.c)
    output_count(counter, file_name, print_file_name);
}

void grep_file(grep_flags flags, char *file_name,
               regex_t templates[MAX_TEMPLATES], int templates_count,
               int print_file_name) {
  FILE *fptr = fopen(file_name, "r");
  if (fptr != NULL) {
    output_file(fptr, flags, file_name, templates, templates_count,
                print_file_name);
    fclose(fptr);
  } else if (!flags.s) {
    printf("grep: %s: No such file or directory\n", file_name);
  }
}

void output_count(int count, char *file_name, int print_file_name) {
  if (print_file_name) {
    printf("%s:", file_name);
  }
  printf("%d\n", count);
}

int read_flags(int argc, char **argv, const char *short_options,
               struct grep_flags *flags, string templates_str[MAX_TEMPLATES],
               int *templates_count) {
  int rez = getopt_long(argc, argv, short_options, NULL, NULL);
  switch (rez) {
    case 'e':
      flags->e = 1;
      append(&templates_str[(*templates_count)++], optarg);
      break;
    case 'i':
      flags->i = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    case 'c':
      flags->c = 1;
      break;
    case 'l':
      flags->l = 1;
      break;
    case 'n':
      flags->n = 1;
      break;
    case 's':
      flags->s = 1;
      break;
    case 'h':
      flags->h = 1;
      break;
    case 'o':
      flags->o = 1;
      break;
    case 'f':
      flags->f = 1;
      read_template(templates_count, templates_str, optarg);
      break;
  }
  return rez;
}

void read_template(int *templates_count, string templates_str[MAX_TEMPLATES],
                   char *file_name) {
  FILE *fptr = fopen(file_name, "r");
  if (fptr != NULL) {
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fptr)) {
      char *find = strchr(line, '\n');
      if (find != NULL) *find = '\0';
      append(&templates_str[(*templates_count)++], line);
    }
    fclose(fptr);
  } else {
    printf("grep: %s: No such file or directory\n", file_name);
  }
}