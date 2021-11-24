#include <stdbool.h>
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
  mqd_t q = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR, NULL);
  if (q == -1) {
    fatal("open");
  }

  while (true) {
    struct message msg;
    ssize_t size = mq_receive(q, (char *)&msg, QUEUE_SIZE, 0);
    if (size == -1) {
      fatal("message recv");
    }
    if (size == 0) {
      printf("End of the queue\n");
      break;
    }
    printf("Message received: [%d: %d %s]\n", msg.seq, msg.temp, msg.data);
  }

  return EXIT_SUCCESS;
}
