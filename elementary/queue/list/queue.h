#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct queue queue_t;

extern queue_t *queue_constructor();

extern void queue_destructor(queue_t *queue);

extern void queue_enqueue(queue_t *queue, void *entry);

extern void *queue_dequeue(queue_t *queue);

extern unsigned char queue_is_empty(queue_t *queue);

#endif