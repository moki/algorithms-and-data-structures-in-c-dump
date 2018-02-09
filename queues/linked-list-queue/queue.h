typedef struct queue *Queue;

typedef struct node *Node;

typedef struct sentinel *Sentinel;

struct node {
  Node next;
  int data;
} node;

Node _createNode(int, Node);

Queue QueueConstructor();

void QueueDestructor(Queue);

void QueueEnqueue(Queue, int);

Node QueueDequeue(Queue);