#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

typedef struct stack stack_t;

typedef void (*entry_destroy_fn_t)(const void *key);

extern stack_t *stack_constructor(void *entry_destroy_fn);

extern void stack_destructor(stack_t *stack);

extern void stack_push(stack_t *stack, void *entry);

extern void *stack_pop(stack_t *stack);

extern void *stack_peek(stack_t *stack);

extern unsigned char stack_is_empty(stack_t *stack);

#endif