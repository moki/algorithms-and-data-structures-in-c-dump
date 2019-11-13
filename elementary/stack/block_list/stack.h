#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

typedef struct stack stack_t;

extern stack_t *stack_constructor(size_t entry_size, uint32_t block_size);

extern void stack_destructor(stack_t *stack);

extern void stack_push(stack_t *stack, void *entry);

extern void *stack_pop(stack_t *stack);

extern unsigned char stack_is_empty(stack_t *stack);

#endif