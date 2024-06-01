#include "open.h"

void parser(int argc, char* argv[], flags* check) {
  struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                                 {"number", 0, NULL, 'n'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {0, 0, 0, 0}};

  int current_flag = 0;

  while ((current_flag =
              getopt_long(argc, argv, "benstvET", longOptions, NULL)) != -1) {
    if (current_flag == 'b') check->b = 1;
    if (current_flag == 'e') {
      check->e = 1;
      check->v = 1;
    }
    if (current_flag == 'n') check->n = 1;
    if (current_flag == 's') check->s = 1;
    if (current_flag == 't') {
      check->t = 1;
      check->v = 1;
    }
    if (current_flag == 'v') check->v = 1;
    if (current_flag == 'E') check->e = 1;
    if (current_flag == 'T') check->t = 1;
  }
}

void read_with_flags(int argc, char* argv[], flags opt) {
  int empty_line_counter = 1;
  char ch = 0;
  int newline = 1;
  int counter = 1;

  FILE* tp;

  if (argc >= 2) {
    for (int i = 1; i <= argc; i++) {
      if ((tp = fopen(argv[i], "r")) != NULL) {
        while ((ch = fgetc(tp)) != EOF) {
          if (opt.s == 1 && ch == '\n') {
            empty_line_counter++;
          } else {
            empty_line_counter = 0;
          }
          if (opt.s == 1 && empty_line_counter > 2) continue;
          if (opt.n == 1 && newline == 1 && opt.b != 1) {
            printf("%6d\t", counter++);
          }
          if (opt.b == 1 && newline == 1 && ch != '\n')
            printf("%6d\t", counter++);

          if (opt.e == 1 && ch == '\n') {
            printf("$");
          }

          if (ch == '\n')
            newline = 1;
          else
            newline = 0;

          if (opt.t == 1 && ch == '\t') {
            printf("^I");
            continue;
          }
          if (opt.v == 0) printf("%c", ch);

          if (opt.v == 1 && ch == 127) printf("^?");
          if (opt.v == 1 && ch == 10) {
            printf("%c", ch);
          }
          if (opt.v == 1 && ch == 9) {
            printf("%c", ch);
          }
          if (opt.v == 1 && ch < 32 && ch != 10 && ch != 9)
            printf("^%c", ch + 64);
          if (opt.v == 1 && ch >= 32 && ch < 127) {
            printf("%c", ch);
          }
        }
        fclose(tp);
        if (argv[i + 1] == NULL) break;
      }
      empty_line_counter = 1;
      newline = 1;
      counter = 1;
    }
  } else
    printf("SHLYAPA!");
}
