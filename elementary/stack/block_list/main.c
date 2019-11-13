#include "stack.h"
#include <stdio.h>

typedef struct point {
	int x;
	int y;
} point;

int main(int argc, char **argv) {
	/* int test */
	// stack_t *stack = stack_constructor(sizeof(int));
	// if (!stack) exit(EXIT_FAILURE);

	// int x = 3;
	// stack_push(stack, &x);

	// int y = 5;
	// stack_push(stack, &y);

	// int z = 1;
	// stack_push(stack, &z);

	// int a = 13;
	// stack_push(stack, &a);

	// int b = 80;
	// stack_push(stack, &b);

	// int c = 77;
	// stack_push(stack, &c);

	// int d = 99;

	// stack_push(stack, &d);
	// int e = 666;
	// stack_push(stack, &e);

	// int r = *(int *) stack_pop(stack);
	// printf("%d\n", r);

	// r = *(int *) stack_pop(stack);
	// printf("%d\n", r);

	// r = *(int *) stack_pop(stack);
	// printf("%d\n", r);

	// r = *(int *) stack_pop(stack);
	// printf("%d\n", r);

	// r = *(int *) stack_pop(stack);
	// printf("%d\n", r);

	// r = *(int *) stack_pop(stack);
	// printf("%d\n", r);

	/* struct test */
	stack_t *stack = stack_constructor(sizeof(struct point), 2);
	if (!stack) exit(EXIT_FAILURE);

	struct point a = { 4, 10 };
	struct point b = { 1, 1 };
	struct point c = { 80, 99 };
	struct point d = { 5, 666 };

	stack_push(stack, &a);
	stack_push(stack, &b);
	stack_push(stack, &c);
	stack_push(stack, &d);

	struct point r;

	r = *(struct point *) stack_pop(stack);
	printf("Popped point x: %d, y: %d\n", r.x, r.y);

	r = *(struct point *) stack_pop(stack);
	printf("Popped point x: %d, y: %d\n", r.x, r.y);

	r = *(struct point *) stack_pop(stack);
	printf("Popped point x: %d, y: %d\n", r.x, r.y);

	r = *(struct point *) stack_pop(stack);
	printf("Popped point x: %d, y: %d\n", r.x, r.y);

	/* string test */
	/* todo entry struct wrapper to be ready for a variabl length strings */
	/* now required to have stack initialized with maximum possible width of a value in bytes */
	/* actaully its working accidentially somewhat, definetly needs stack_entry struct wrappers around items for this to work properly */
	/* needs way to store how much long is each key */
	/* another stack for that? xd */
	// stack_t *stack = stack_constructor(sizeof(char) * 21);
	// if (!stack) exit(EXIT_FAILURE);

	// char *a = "hey";
	// char *b = "there or here";
	// char *c = "brother my bro broski";
	// char *d = "what's up";

	// stack_push(stack, a);
	// stack_push(stack, b);
	// stack_push(stack, c);
	// stack_push(stack, d);

	// char *r = (char *) stack_pop(stack);
	// printf("%s\n", r);

	// r = (char *) stack_pop(stack);
	// printf("%s\n", r);

	// r = (char *) stack_pop(stack);
	// printf("%s\n", r);

	// r = (char *) stack_pop(stack);
	// printf("%s\n", r);

	stack_destructor(stack);
}