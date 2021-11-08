#include <stdlib.h>

#include "queue.h"

/**
 *  should be freed
 */
struct queue *init_queue(struct queue *queue, int size) {
  void **data = malloc(sizeof(queue->data[0]) * size);
  *queue = (struct queue){.front = 0, .rear = 0, .data = data};
  return queue;
}

void enqueue(struct queue *queue, void *node) {
  queue->data[queue->rear++] = node;
}

void *dequeue(struct queue *queue) {
  if (queue->front == queue->rear) {
    return NULL;
  }
  return queue->data[queue->front++];
}

void destroy_queue(struct queue *queue) { free(queue->data); }
