#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MIN_INC 64

struct node {
  size_t size;
  struct node *prev;
  struct node *next;
  int used;
};

extern char end;

struct node *head = NULL;

void *current_brk = NULL;

void *my_malloc(size_t size) {
  if (head == NULL) {
    current_brk = sbrk(256);
    fprintf(stderr, "Starting heap at %p end symbol at %p\n", //
            current_brk, &end);
    head = (struct node *)current_brk;
    *head = (struct node){.used = 1};
    current_brk += 256;
  }

  if (size % 16 != 0) {
    size += 16 - size % 16;
  }

  struct node *itr = head;
  while (itr) {
    if (itr->size >= size && !itr->used) {
      itr->used = 1;
      return itr + 1;
    }
    if (itr->next == NULL) {
      break;
    }
    itr = itr->next;
  }

  struct node *new = (void *)(itr + 1) + itr->size;
  void *buff = new + 1;
  int inc = (buff + size) - current_brk;
  if (inc > 0) {
    inc = inc > MIN_INC ? inc : MIN_INC;
    current_brk = sbrk(inc);
    if (current_brk == (void *)-1) {
      perror("error allocating more memory");
      exit(1);
    }
    current_brk += inc;
  }
  itr->next = new;
  *new = (struct node){.prev = itr, .size = size, .used = 1};
  char *mark = (char *)buff;
  mark[size - 1] = 0xea;
  return buff;
}

void my_free(void *ptr) {
  struct node *itr = (struct node *)ptr - 1;
  itr->used = 0;
}

int main() {
  for (int i = 0; i < 1024; i++) {
    size_t str_size = 15; // +
    char *str = my_malloc(str_size + 1);
    memset(str, 'A', str_size);
    str[str_size] = '\0';

    write(STDOUT_FILENO, str, str_size);
  }

  return EXIT_SUCCESS;
}
