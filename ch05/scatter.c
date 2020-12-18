#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

void fatal(char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

ssize_t my_writev(int fd, const struct iovec* in, int iovcnt) {
  size_t bufsize = 0;
  for (int i = 0; i < iovcnt; i++) {
    bufsize += in[i].iov_len;
  }
  void* buff = malloc(bufsize);
  void* pos = buff;
  for (int i = 0; i < iovcnt; i++) {
    memcpy(pos, in[i].iov_base, in[i].iov_len);
    pos += in[i].iov_len;
  }

  ssize_t cnt = write(fd, buff, bufsize);
  free(buff);
  return cnt;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fatal("usage: scatter args...");
  }
  int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    fatal("open failed");
  }

  int buffers = argc - 2;
  struct iovec* vector = malloc(sizeof(struct iovec) * buffers);
  for (int i = 0; i < buffers; i++) {
    int j = i + 2;
    vector[i] = (struct iovec){.iov_base = argv[j], .iov_len = strlen(argv[j])};
  }

  ssize_t bytes = my_writev(fd, vector, buffers);
  printf("%zd bytes written\n", bytes);

  free(vector);
  close(fd);

  return EXIT_SUCCESS;
}
