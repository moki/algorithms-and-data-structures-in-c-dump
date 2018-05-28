#include <stddef.h>

typedef struct heap *Heap;

// void heap_heapify(void *base, size_t num, size_t s,
// 		  int (*cmp)(const void *, const void *),
// 		  void (swp)(void *, void *, int s));

void heap_sort(void *base, size_t num, size_t s,
	       int (*cmp)(const void *, const void *),
	       void (swp)(void *, void *, int s));