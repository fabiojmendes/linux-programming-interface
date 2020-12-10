#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct node {
  size_t size;
  struct node *prev;
  struct node *next;
  int used;
};

struct node *head = NULL;

struct node *allocate(struct node *prev, size_t size) {
  struct node *new = sbrk(sizeof(struct node) + size);
  if (new == (void *)-1) {
    perror("error allocating more memory");
    exit(1);
  }

  *new = (struct node){.prev = prev, .size = size};
  if (prev != NULL) {
    prev->next = new;
  }

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
  new->used = 1;
  if (head == NULL) {
    head = new;
  }
  void *buff = new + 1;
  ((char *)buff)[size - 1] = 0xea;
  return buff;
}

void my_free(void *ptr) {
  struct node *itr = (struct node *)ptr - 1;
  itr->used = 0;
}

int main(int argc, char *argv[]) {
  size_t str_size = argc > 1 ? atoi(argv[1]) : 16;
  int strings = argc > 2 ? atoi(argv[2]) : 1024;

  for (int i = 0; i < strings; i++) {
    char *str = my_malloc(str_size);
    memset(str, 'A', str_size - 1);
    str[str_size - 1] = '\0';

    write(STDOUT_FILENO, str, str_size);
    my_free(str);
  }

  return EXIT_SUCCESS;
}
