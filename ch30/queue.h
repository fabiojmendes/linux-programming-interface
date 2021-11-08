struct queue {
  int front, rear;
  void **data;
};

struct queue *init_queue(struct queue *queue, int size);

void enqueue(struct queue *queue, void *node);

void *dequeue(struct queue *queue);

void destroy_queue(struct queue *queue);
