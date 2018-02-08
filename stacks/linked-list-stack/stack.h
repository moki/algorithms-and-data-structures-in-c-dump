typedef struct stack *Stack;

typedef struct node *Node;

struct node {
  int data;
  Node prev;
  Node next;
} node;

Node createNewNode(int, Node);

void _free(Node node);

Stack StackConstructor();

void StackDestructor(Stack);

void StackPush(Stack, int);

Node StackPop(Stack);

void StackTraverse(Stack, void (*f)(Node));

