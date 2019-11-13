#include "queue.h"
#include <stdio.h>

int main(int argc, char **argv) {
	queue_t *queue = queue_constructor();

	typedef struct point {
		int x;
		int y;
	} point;

	struct point a = { 1, 5};
	struct point b = { 389, -666};
	struct point c = { -251, 30};

	queue_enqueue(queue, &a);
	queue_enqueue(queue, &b);
	queue_enqueue(queue, &c);

	struct point result;

	result = *(struct point *) queue_dequeue(queue);
	printf("dequeued point x: %d, y: %d\n", result.x, result.y);

	result = *(struct point *) queue_dequeue(queue);
	printf("dequeued point x: %d, y: %d\n", result.x, result.y);

	result = *(struct point *) queue_dequeue(queue);
	printf("dequeued point x: %d, y: %d\n", result.x, result.y);

	queue_destructor(queue);
}