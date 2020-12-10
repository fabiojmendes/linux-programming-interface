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

struct node *allocate(struct node *prev, size_t size) {
  struct node *new;
  if (prev == NULL) {
    new = sbrk(0);
    head = new;
  } else {
    new = (void *)(prev + 1) + prev->size;
    prev->next = new;
  }

  void *buff = new + 1;
  int inc = (buff + size) - sbrk(0);
  if (inc > 0) {
    inc = inc > MIN_INC ? inc : MIN_INC;
    void *res = sbrk(inc);
    if (res == (void *)-1) {
      perror("error allocating more memory");
      exit(1);
    }
  }
  *new = (struct node){.prev = prev, .size = size, .used = 1};
  return new;
}

void *my_malloc(size_t size) {
  if (size % 16 != 0) {
    size += 16 - size % 16;
  }

  struct node *last = NULL;
  for (struct node *itr = head; itr; itr = itr->next) {
    if (itr->size >= size && !itr->used) {
      itr->used = 1;
      return itr + 1;
    }
    last = itr;
  }

  struct node *new = allocate(last, size);
  void *buff = new + 1;
  ((char *)buff)[size - 1] = 0xea;
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
