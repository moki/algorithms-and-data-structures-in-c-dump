#include "queue.h"

struct queue {
	void	*entry;
	queue_t *next;
};

queue_t *queue_constructor() {
	queue_t *queue, *sentinel;

	sentinel = malloc(sizeof(*sentinel));
	queue 	 = malloc(sizeof(*queue));

	queue->next = sentinel;
	sentinel->next = sentinel;

	return queue;
}

void queue_destructor(queue_t *queue) {
	while (!queue_is_empty(queue))
		(void) queue_dequeue(queue);

	free(queue->next);
	free(queue);
}

void queue_enqueue(queue_t *queue, void *entry) {
	queue_t *last = queue->next;

	queue_t *new  = malloc(sizeof(*new));
	new->entry = entry;
	new->next  = last->next;

	queue->next = new;
	last->next = new;
}

void *queue_dequeue(queue_t *queue) {
	queue_t *sentinel = queue->next->next;
	queue_t *first    = sentinel->next;
	void 	*entry    = first->entry;

	sentinel->next = first->next;

	if (first == queue->next)
		queue->next = sentinel;

	free(first);
	first = NULL;

	return entry;
}

unsigned char queue_is_empty(queue_t *queue) {
	return queue->next == queue->next->next;
}