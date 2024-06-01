#include "open.h"

void parser(int argc, char* argv[], flags* check) {
  struct option longOptions[] = {{0, 0, 0, 0}};

  int current_Flag = 0;
  int counter_Pattern = 0;
  while ((current_Flag = getopt_long(argc, argv, "ivclnhse:f:o", longOptions,
                                     NULL)) != -1) {
    if (current_Flag == 'e') {
      check->e = 1;
      if (counter_Pattern == 0) {
        strcat(check->pattern, optarg);
        counter_Pattern++;
      } else {
        strcat(check->pattern, "|");
        strcat(check->pattern, optarg);
      }
    }
    if (current_Flag == 'i') check->i = 1;
    if (current_Flag == 'v') check->v = 1;
    if (current_Flag == 'c') check->c = 1;
    if (current_Flag == 'l') check->l = 1;
    if (current_Flag == 'n') check->n = 1;
    if (current_Flag == 'h') check->h = 1;
    if (current_Flag == 's') check->s = 1;
    if (current_Flag == 'f') check->f = 1;
    if (current_Flag == 'o') check->o = 1;
  }
}

void grepping(int argc, char** argv, flags* check) {
  FILE* tp;
  char* line = NULL;
  size_t length = 0;
  regex_t regex;
  int file_counter = 1;
  int files_from_user = argc - (optind + 1);
  int flag_c_counter = 0;
  int flag_n_counter = 0;
  int flag_last_line = 0;
  if (argc > 3) {
    if (!check->e) {
      strcat(check->pattern, argv[optind]);
    } else {
      file_counter = 0;
      files_from_user = argc - optind;
    }
    if (check->i == 1) {
      if (regcomp(&regex, check->pattern, REG_ICASE) != 0) {
        regfree(&regex);
        exit(1);
      }
    } else {
      if (regcomp(&regex, check->pattern, REG_EXTENDED) != 0) {
        regfree(&regex);
        exit(1);
      }
    }
    while ((tp = fopen(argv[optind + file_counter], "rb")) != NULL) {
      while ((getline(&line, &length, tp)) != -1) {
        flag_n_counter++;
        check->match = regexec(&regex, line, 0, NULL, 0);
        if (check->v) check->match = check->match ? 0 : 1;
        if (check->match == 0) {
          if (files_from_user > 1 && !check->h) {
            printf("%s:", argv[optind + file_counter]);
          }
          if (line[strlen(line) - 1] == '\n') {
            flag_last_line = 1;
          } else
            flag_last_line = 0;
          if (check->l) {
            flag_last_line = 1;
            printf("%s\n", argv[optind + file_counter]);
            break;
          } else if (check->c)
            flag_c_counter++;
          else if (check->h)
            printf("%s", line);
          else if (check->n)
            printf("%d:%s", flag_n_counter, line);
          else if (check->i)
            printf("%s", line);
          else
            printf("%s", line);
        }
      }
      if (flag_last_line != 1) {
        printf("\n");
      }
      if (check->c && !check->l) printf("%d\n", flag_c_counter);
      file_counter++;
      fclose(tp);
      if (argv[optind + file_counter] == NULL) break;
    }
    free(line);
    regfree(&regex);
  } else if (!check->s)
    printf("SHLYAPA!");
}
