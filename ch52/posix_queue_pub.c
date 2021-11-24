#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <mqueue.h>

#include "posix_queue.h"

void fatal(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main() {
  mqd_t q = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR, NULL);
  if (q == -1) {
    fatal("open");
  }

  for (int i = 0; i < 100; i++) {
    struct message msg = {.seq = i, .temp = i / 2};
    snprintf(msg.data, sizeof(msg.data), "Message: %d", i);
    printf("Sending: %s\n", msg.data);
    int err = mq_send(q, (char *)&msg, sizeof(msg), 0);
    if (err) {
      fatal("sending");
    }
  }

  // Send empty message as EOF
  int err = mq_send(q, "", 0, 0);
  if (err) {
    fatal("sending end");
  }

  mq_close(q);
  mq_unlink(QUEUE_NAME);

  return EXIT_SUCCESS;
}
