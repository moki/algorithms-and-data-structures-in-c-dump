typedef struct list *List;

typedef struct node *Node;

struct node {
  int data;
  Node prev;
  Node next;
} node;

#define TRAVERSE_NORMAL (0)
#define TRAVERSE_REVERSE (1)

Node createNewNode(int, Node, Node);

void _free(Node node);

List ListConstructor();

void ListDestructor(List);

void ListInsert(List, int);

void ListDelete(List, int);

Node ListSearch(List, int);

void ListTraverse(List, void (*f)(Node), int);

