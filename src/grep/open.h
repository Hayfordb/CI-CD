#ifndef _OPEN_H_
#define _OPEN_H_
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;  // Шаблон
  int i;  // Игнорирует различия регистра. +
  int v;  // Инвертирует смысл поиска соответствий. +
  int c;  // Выводит только количество совпадающих строк +
  int l;  //  Выводит только совпадающие файлы +
  int n;  // Предваряет каждую строку вывода номером строки из файла ввода.
  int h;  // Выводит совпадающие строки, не предваряя их именами файлов
  int s;  // Подавляет сообщения об ошибках о несуществующих или нечитаемых
          // файлах
  int f;  // Получает регулярные выражения из файла
  int o;  // Печатает только совпадающие (непустые) части совпавшей строки
  char pattern[4096];  // match|grep
  int match;
} flags;

void parser(int argc, char** argv, flags* check);
void grepping(int argc, char** argv, flags* check);
#endif