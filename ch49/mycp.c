#include <fcntl.h>
#include <memory.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Usage: mycp <src> <dst>\n");
    exit(EXIT_FAILURE);
  }
  int src = open(argv[1], O_RDONLY);
  if (src == -1) {
    perror(argv[1]);
    exit(EXIT_FAILURE);
  }
  int dst = open(argv[2], O_CREAT | O_RDWR, 0644);
  if (dst == -1) {
    perror(argv[2]);
    exit(EXIT_FAILURE);
  }

  struct stat s;
  fstat(src, &s);

  ftruncate(dst, s.st_size);

  void *src_mem = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE, src, 0);
  if (src_mem == MAP_FAILED) {
    perror("src alloc");
    exit(EXIT_FAILURE);
  }
  void *dst_mem = mmap(NULL, s.st_size, PROT_WRITE, MAP_SHARED, dst, 0);
  if (dst_mem == MAP_FAILED) {
    perror("dst alloc");
    exit(EXIT_FAILURE);
  }

  memcpy(dst_mem, src_mem, s.st_size);

  munmap(src_mem, s.st_size);
  close(src);
  munmap(dst_mem, s.st_size);
  close(dst);

  return 0;
}
