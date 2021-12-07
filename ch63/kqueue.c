#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ev.h>
#include <sys/event.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#define NUMBER_EVENTS 16
#define BUF_SIZE 256

void fatal(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  printf("Hello kqueue\n");
  if (argc < 2) {
    printf("Usage: %s <file-name>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int fifo = open(argv[1], O_RDONLY);
  if (fifo == -1) {
    fatal("open fifo");
  }

  struct kevent ev;
  EV_SET(&ev, fifo, EVFILT_READ, EV_ADD, 0, 0, 0);

  int kq = kqueue();
  if (kq == -1) {
    fatal("creating queue");
  }

  char buf[BUF_SIZE];
  struct kevent evlist[NUMBER_EVENTS];

  while (true) {
    int evs = kevent(kq, &ev, 1, evlist, NUMBER_EVENTS, NULL);
    printf("Events: %d\n", evs);
    for (int i = 0; i < evs; i++) {
      struct kevent e = evlist[i];
      if (e.flags & EV_ERROR) {
        fprintf(stderr, "EV_ERROR: %s\n", strerror(e.data));
        exit(EXIT_FAILURE);
      }

      if (e.flags & EV_RE) {
        ssize_t bytes = read(e.ident, buf, BUF_SIZE);
        write(STDOUT_FILENO, buf, bytes);
      }
    }
  }

  return EXIT_SUCCESS;
}
