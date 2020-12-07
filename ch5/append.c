#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void fatal(char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fatal("usage");
  }
  int fd = open(argv[1], O_WRONLY | O_APPEND);
  if (fd == -1) {
    fatal("open failed");
  }

  off_t pos = lseek(fd, 0, SEEK_CUR);
  if (pos == -1) {
    fatal("seek failed");
  }
  printf("seek offset: %ld\n", pos);

  pos = lseek(fd, 0, SEEK_SET);
  if (pos == -1) {
    fatal("seek failed");
  }
  printf("seek offset: %ld\n", pos);

  if (write(fd, "test", 4) == -1) {
    fatal("write failed");
  }

  pos = lseek(fd, 0, SEEK_CUR);
  if (pos == -1) {
    fatal("seek failed");
  }
  printf("seek offset: %ld\n", pos);

  return EXIT_SUCCESS;
}
