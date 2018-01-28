typedef struct queue *Queue;

Queue QueueConstructor(int);

void QueueDestructor(Queue);

void Enqueue(Queue, int);

int Dequeue(Queue);

int QueueIsEmpty(Queue);

int QueueIsFull(Queue);