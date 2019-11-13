#include "stack.h"

struct stack {
	void *entry;
	stack_t *next;
};

stack_t *stack_constructor() {
	stack_t *stack = malloc(sizeof(*stack));
	if (!stack) return NULL;

	stack->next  = NULL;

	return stack;
}

void stack_destructor(stack_t *stack) {
	stack_t *cursor = stack;

	while (cursor) {
		cursor = stack->next;
		free(stack);
		stack = cursor;
	}
}

void stack_push(stack_t *stack, void *entry) {
	stack_t *new = malloc(sizeof(*new));

	new->entry = entry;
	new->next  = stack->next;

	stack->next = new;
}

void *stack_pop(stack_t *stack) {
	stack_t *top = stack->next;
	if (!top) return NULL;

	void *entry = top->entry;
	stack->next = top->next;
	free(top);

	return entry;
}

void *stack_peek(stack_t *stack) {
	stack_t *top = stack->next;
	if (!top)
		return NULL;
	else
		return top->entry;

	return top ? top->entry : NULL;
}

unsigned char stack_is_empty(stack_t *stack) {
	return !stack->next;
}