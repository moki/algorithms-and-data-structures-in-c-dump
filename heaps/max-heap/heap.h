#include <stddef.h>

typedef struct heap *Heap;

Heap HeapConstructorArray(int *, size_t);

Heap HeapConstructor();

void HeapInsert(Heap, int);

int HeapPeekMax(Heap);

int HeapExtractMax(Heap);

int HeapIsEmpty(Heap);

int HeapSize(Heap);

void HeapDestructor(Heap);

void HeapRawDisplay(Heap);

void heapify(Heap, int);

Heap HeapSort(int *, size_t);