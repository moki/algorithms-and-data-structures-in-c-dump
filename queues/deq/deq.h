typedef struct deq *Deq;

Deq DeqConstructor(int);

void DeqDestructor(Deq);

void DeqInsertBack(Deq, int);

int DeqDeleteFront(Deq);

void DeqInsertFront(Deq, int);

int DeqDeleteBack(Deq);

int DeqIsEmpty(Deq);

int DeqIsFull(Deq);