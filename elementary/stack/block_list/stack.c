#include "stack.h"

struct stack {
	uint32_t block_size;
	size_t entry_size;
	stack_t *previous;
	void   *base;
	void   *top;
};

stack_t *stack_constructor(size_t entry_size, uint32_t block_size) {
	stack_t *stack = malloc(sizeof(*stack));
	if (!stack) return NULL;

	stack->entry_size = entry_size;
	stack->block_size = block_size;
	stack->base = malloc(stack->entry_size * stack->block_size);
	if (!stack->base) return NULL;

	stack->top = stack->base;
	stack->previous = NULL;

	return stack;
}

void stack_destructor(stack_t *stack) {
	stack_t *trailing_cursor;

	while (stack) {
		trailing_cursor = stack->previous;
		free(stack->base);
		free(stack);

		stack = trailing_cursor;
	}
}

void stack_push(stack_t *stack, void *entry) {
	if (stack->top < stack->base + stack->block_size * stack->entry_size) {
		memcpy(stack->top, entry, stack->entry_size);
		stack->top = stack->base + 1 * stack->entry_size;
		return;
	}

	stack_t *block = malloc(sizeof(*block));

	block->block_size = stack->block_size;
	block->previous = stack->previous;
	block->base = stack->base;
	block->top  = stack->top;

	stack->previous = block;
	stack->base = malloc(stack->entry_size * stack->block_size);
	if (!stack->base) exit(EXIT_FAILURE);

	memcpy(stack->base, entry, stack->entry_size);

	stack->top = stack->base + 1 * stack->entry_size;
}

void *stack_pop(stack_t *stack) {
	if (stack->top == stack->base) {
		stack_t *previous = stack->previous;

		stack->previous = previous->previous;
		free(stack->base);

		stack->block_size = previous->block_size;
		stack->base = previous->base;
		stack->top = previous->top;

		free(previous);
	}

	stack->top = stack->top - 1 * stack->entry_size;

	return stack->top;
}

unsigned char stack_is_empty(stack_t *stack) {
	return stack->base == stack->top && stack->previous == NULL;
}