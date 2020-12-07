#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void fatal(char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int dup(int fd) {
  int new_fd = fcntl(fd, F_DUPFD, 200);
  return new_fd;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fatal("usage");
  }
  int fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    fatal("open failed");
  }

  int fd2 = dup(fd);
  if (fd == -1) {
    fatal("dup failed");
  }

  char* test_fd1 = "test from fd1\n";
  write(fd, test_fd1, strlen(test_fd1));
  off_t offset = lseek(fd2, 0, SEEK_CUR);
  if (offset != (int)strlen(test_fd1)) {
    char buff[256];
    sprintf(buff, "offset for fd2 doesn't match: %ld", offset);
    fatal(buff);
  }
  char* test_fd2 = "test from fd2\n";
  write(fd2, test_fd2, strlen(test_fd2));

  close(fd2);
  close(fd);

  return EXIT_SUCCESS;
}
