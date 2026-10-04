#include "s21_cat.h"

#include <string.h>

int main(int argc, char *argv[]) {
  const char *short_options = "benstET";
  const struct option long_options[LONG_OPTIONS] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0}};

  cat_flags flags = {0};

  int files_count = 0;
  char *files[MAX_FILES] = {0};

  while (read_flags(argc, argv, short_options, long_options, &flags) != -1);

  for (int i = optind; i < argc; i++) {
    files[files_count++] = argv[i];
  }

  int lines_count = 0;
  int met_empty = 0;
  int new_line = 1;
  for (int i = 0; i < files_count; i++)
    print_file(flags, files[i], &met_empty, &lines_count, &new_line);
}

int read_flags(int argc, char **argv, const char *short_options,
               const struct option long_options[LONG_OPTIONS],
               cat_flags *flags) {
  int rez = getopt_long(argc, argv, short_options, long_options, NULL);
  switch (rez) {
    case 'b':
      flags->b = 1;
      break;
    case 'e':
      flags->v = 1;
      flags->e = 1;
      break;
    case 'E':
      flags->e = 1;
      break;
    case 'n':
      flags->n = 1;
      break;
    case 's':
      flags->s = 1;
      break;
    case 't':
      flags->v = 1;
      flags->t = 1;
      break;
    case 'T':
      flags->t = 1;
      break;
  }
  return rez;
}

void output_file(cat_flags flags, FILE *fptr, int *met_empty, int *lines_count,
                 int *new_line) {
  int c;
  while ((c = fgetc(fptr)) != EOF) {
    int empty_line = *new_line && c == '\n';
    int need_to_print = 1;
    if (empty_line && flags.s) {
      if (!*met_empty)
        *met_empty = 1;
      else
        need_to_print = 0;

    } else
      *met_empty = 0;
    if (*new_line && need_to_print) {
      if (flags.b) {
        if (!empty_line) printf("%6d\t", ++(*lines_count));
      } else if (flags.n) {
        printf("%6d\t", ++(*lines_count));
      }
      *new_line = 0;
    }
    if (c == '\n') {
      *new_line = 1;
      if (flags.e && need_to_print) putchar('$');
    }
    if (c == '\t' && flags.t) {
      printf("^I");
      need_to_print = 0;
    }
    if (flags.v && need_to_print) {
      if (c < 32 && c != '\n' && c != '\t') {
        printf("^");
        c += 64;
      } else if (c == 127 || c == 255) {
        if (c == 255) printf("M-");
        printf("^?");
        need_to_print = 0;
      } else if (c >= 128 && c < 160) {
        printf("M-^");
        c -= 64;
      } else if (c >= 160) {
        printf("M-");
        c -= 128;
      }
    }
    if (need_to_print) putchar(c);
  }
}

void print_file(cat_flags flags, char *file_name, int *met_empty,
                int *lines_count, int *new_line) {
  FILE *fptr = fopen(file_name, "r");
  if (fptr != NULL) {
    output_file(flags, fptr, met_empty, lines_count, new_line);
    fclose(fptr);
  } else
    printf("cat: %s: No such file or directory\n", file_name);
}