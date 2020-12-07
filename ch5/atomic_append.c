#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void fatal(char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
  if (argc < 3) {
    fatal("usage");
  }
  int flags = O_WRONLY | O_CREAT | O_TRUNC;
  if (argc < 4 || strcmp(argv[3], "x") != 0) {
    flags |= O_APPEND;
  }

  int fd = open(argv[1], flags, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    fatal("open failed");
  }

  int bytes = atoi(argv[2]);
  for (int i = 0; i < bytes; i++) {
    if (!(flags & O_APPEND)) {
      lseek(fd, 0, SEEK_END);
    }
    write(fd, "x", 1);
  }

  close(fd);
  return EXIT_SUCCESS;
}
