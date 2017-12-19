typedef struct stack *Stack;

Stack StackConstructor(int);

void StackDestructor(Stack);

void StackPush(Stack, int);

int StackPop(Stack);

void StackTraverse(Stack, void (*f)(int));

int StackIsEmpty(Stack);