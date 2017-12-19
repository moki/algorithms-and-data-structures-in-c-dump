typedef struct bag *Bag;

Bag BagConstructor(void);

void BagDestructor(Bag);

void BagAdd(Bag, int);

void BagTraverse(Bag, void (*f)(int));

int BagIsEmpty(Bag);

int BagSize(Bag);