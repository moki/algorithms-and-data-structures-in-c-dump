#include "stack.h"

static void _free(void *ptr);
static void stack_expand(stack_t *stack);
static void stack_shrink(stack_t *stack);

struct stack {
	void     *data;
	uint32_t  size;
	uint32_t  used;
	size_t 	  entry_size;
};

stack_t *stack_constructor(size_t entry_size) {
	if (!entry_size) return NULL;

	stack_t *stack = malloc(sizeof(*stack));
	if(!stack) return NULL;

	stack->entry_size = entry_size;
	stack->data = NULL;
	stack->size = stack->used = 0;

	return stack;
}

void stack_destructor(stack_t *stack) {
	if (!stack) return;

	if (stack->size)
		_free(stack->data);

	_free(stack);
}

void stack_push(stack_t *stack, void *entry) {
	if (stack->used >= stack->size)
		stack_expand(stack);

	memcpy((unsigned char *) stack->data + stack->used * stack->entry_size, entry, stack->entry_size);

	stack->used++;
}

void *stack_pop(stack_t *stack) {
	if (!stack->used) goto error;

	if (stack->used << 1 <= stack->size)
		stack_shrink(stack);

	stack->used--;
	return (unsigned char *) stack->data + stack->used * stack->entry_size;
error:
	stack_destructor(stack);
	exit(EXIT_FAILURE);
}

unsigned char stack_is_empty(stack_t *stack) {
	return !stack->used;
}

static void stack_expand(stack_t *stack) {
	if (!stack->size) {
		stack->size = 1;
		stack->data = malloc(stack->size * stack->entry_size);
		if (!stack->data) exit(EXIT_FAILURE);
		return;
	}

	stack->size <<= 1;
	stack->data = realloc(stack->data, stack->entry_size * stack->size);
	if (!stack->data) exit(EXIT_FAILURE);
}

static void stack_shrink(stack_t *stack) {
	stack->size >>= 1;
	stack->data = realloc(stack->data, stack->entry_size * stack->size);
	if (!stack->data) exit(EXIT_FAILURE);
}

static void _free(void *ptr) {
	free(ptr);
	ptr = NULL;
}