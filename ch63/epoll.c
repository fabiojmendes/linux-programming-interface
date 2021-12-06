#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/epoll.h>

#define MAX_EVENTS 10
#define BUF_SIZE 256

void fatal(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("Usage: %s <file-name>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int fifo = open(argv[1], O_RDONLY);
  if (fifo == -1) {
    fatal("open fifo");
  }

  int epfd = epoll_create(1);
  if (epfd == -1) {
    fatal("epoll creation failed");
  }

  struct epoll_event ev = {.events = EPOLLIN, .data.fd = fifo};
  int err = epoll_ctl(epfd, EPOLL_CTL_ADD, fifo, &ev);
  if (err == -1) {
    fatal("epoll ctl");
  }

  struct epoll_event *events = calloc(MAX_EVENTS, sizeof(struct epoll_event));
  char buf[BUF_SIZE];
  bool is_open = true;
  while (is_open) {
    int ready = epoll_wait(epfd, events, MAX_EVENTS, -1);
    for (int i = 0; i < ready; i++) {
      struct epoll_event e = events[i];
      printf("fd(%d) is ready %d\n", e.data.fd, e.events);
      if (e.events & EPOLLIN) {
        int res = read(e.data.fd, buf, BUF_SIZE);
        write(STDOUT_FILENO, buf, res);
      } else if (e.events & EPOLLHUP) {
        printf("closing fd(%d)\n", e.data.fd);
        close(e.data.fd);
        is_open = false;
      }
    }
  }

  printf("all fds are closed, exiting\n");
}