#include <stdio.h>
#include <stdlib.h>

#include <sys/poll.h>

int main() {

  struct pollfd *fds = calloc(10, sizeof(struct pollfd));
  for (int i = 0; i < 3; i++) {
    fds[i] = (struct pollfd){.fd = i, .events = POLLIN | POLLOUT};
  }

  int events = poll(fds, 3, -1);
  for (int i = 0; i < events; i++) {
    printf("revents: POLLIN(%d), POLLOUT(%d)\n", (fds[i].revents & POLLIN) > 1,
           (fds[i].revents & POLLOUT) > 1);
  }
}