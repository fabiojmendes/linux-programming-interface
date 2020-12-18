#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 128

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int fd = open(argv[1], O_WRONLY | O_CREAT, 0644);
  if (fd == -1) {
    fprintf(stderr, "Error opening the file %d\n", errno);
    exit(EXIT_FAILURE);
  }
  char buff[BUFF_SIZE];
  while (1) {
    ssize_t n = read(STDIN_FILENO, buff, BUFF_SIZE);
    if (n == -1) {
      fprintf(stderr, "Error reading file %d\n", errno);
      exit(EXIT_FAILURE);
    }
    if (n == 0) break;
    write(fd, buff, n);
    write(STDOUT_FILENO, buff, n);
  }

  close(fd);
  return EXIT_SUCCESS;
}
