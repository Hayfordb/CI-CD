#ifndef _OPEN_H_
#define _OPEN_H_
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int b;  // печать не пустых строк
  int e;  // добавить в конец строки $
  int n;  // печать всех строк
  int s;  // ужать пустые строки в 1
  int t;  // замена /t на ^I
  int v;  // принтует не принтуемый символ
} flags;

void parser(int argc, char* argv[], flags* check);
void read_with_flags(int argc, char* argv[], flags opt);

#endif