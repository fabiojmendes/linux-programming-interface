#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 256

void fatal(char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
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

  char buff[BUFF_SIZE];
  while (1) {
    ssize_t n = read(src, buff, BUFF_SIZE);
    if (n == -1) {
      fatal("Error reading file %d\n");
    }
    if (n == 0) break;

    off_t seek = 0;
    off_t wpos = -1;
    size_t writes = 0;
    for (int i = 0; i < n; i++) {
      if (buff[i] == 0) {
        if (writes != 0) {
          write(dst, &buff[wpos], writes);
          writes = 0;
          wpos = -1;
        }
        seek++;
      } else {
        if (seek != 0) {
          lseek(dst, seek, SEEK_CUR);
          seek = 0;
        }
        if (wpos == -1) {
          wpos = i;
        }
        writes++;
      }
    }

    if (writes > 0) {
      write(dst, &buff[wpos], writes);
    } else {
      lseek(dst, seek, SEEK_CUR);
    }
  }

  close(src);
  close(dst);
  return EXIT_SUCCESS;
}
