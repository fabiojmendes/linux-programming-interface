#pragma once

#include <sys/types.h>
#define QUEUE_SIZE 8192

#define QUEUE_NAME "/test_queue"

struct message {
  int seq;
  u_int16_t temp;
  char data[64];
};
