typedef struct list *List;

List ListConstructor();

void ListDestructor(List);

void ListInsert(List, int);

void ListDelete(List, int);

int ListSearch(List, int);