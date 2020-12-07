#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void fatal(char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    errno = EINVAL;
    fatal("bad usage");
  }
  printf("Hello! %s\n", argv[1]);
  return EXIT_SUCCESS;
}
