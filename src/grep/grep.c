#include "open.h"

int main(int argc, char** argv) {
  flags grep_Flag = {0};

  parser(argc, argv, &grep_Flag);

  grepping(argc, argv, &grep_Flag);

  return 0;
}