#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Pass at least one file\n");
    exit(EXIT_FAILURE);
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "Error opening file (%d)\n", errno);
    exit(EXIT_FAILURE);
  }

  char buff[1025];
  while (1) {
    int n = read(fd, buff, 1024);
    if (n == -1) {
      fprintf(stderr, "Error reading file (%d)\n", errno);
      exit(EXIT_FAILURE);
    }
    if (n == 0) break;

    buff[n] = '\0';
    printf("%s", buff);
  }
  close(fd);

  puts("Finish");
}
