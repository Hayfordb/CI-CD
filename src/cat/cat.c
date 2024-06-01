#include "open.h"

int main(int argc, char* argv[]) {
  flags options = {0};

  parser(argc, argv, &options);

  read_with_flags(argc, argv, options);

  return 0;
}
