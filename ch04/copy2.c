#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFF_SIZE 1024 * 1024 * 16

void fatal(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <src> <dst>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  int src = open(argv[1], O_RDONLY);
  if (src == -1) {
    fatal("Error opening source file");
  }
  int dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (dst == -1) {
    fatal("Error opening destination file");
  }

  void *buff = malloc(BUFF_SIZE);
  while (1) {
    ssize_t n = read(src, buff, BUFF_SIZE);
    if (n == -1) {
      fatal("Error reading file %d\n");
    }
    if (n == 0)
      break;

    write(dst, buff, n);
  }
  free(buff);

  close(src);
  close(dst);
  return EXIT_SUCCESS;
}
