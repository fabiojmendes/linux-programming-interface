#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

void fatal(char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
  if (argc < 3) {
    fatal("usage");
  }
  int fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    fatal("open failed");
  }

  off_t o = atoll(argv[2]);
  if (lseek(fd, o, SEEK_SET) == -1) {
    fatal("seek failed");
  }

  if (write(fd, "test", 4) == -1) {
    fatal("write failed");
  }

  return EXIT_SUCCESS;
}
