#include "stack.h"

struct stack {
	void 	 *entry;
	stack_t  *next;
	entry_destroy_fn_t entry_destroy_fn;
};

stack_t *stack_constructor(void *entry_destroy_fn) {
	stack_t *stack = malloc(sizeof(*stack));
	if (!stack) return NULL;

	stack->entry_destroy_fn = (entry_destroy_fn_t) entry_destroy_fn;

	stack->next = NULL;

	return stack;
}

void stack_destructor(stack_t *stack) {
	if (!stack) return;
	stack_t *cursor = stack;
	entry_destroy_fn_t free_fn = stack->entry_destroy_fn;

	while (cursor) {
		cursor = stack->next;
		if (free_fn && cursor->entry)
			free_fn(cursor->entry);
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