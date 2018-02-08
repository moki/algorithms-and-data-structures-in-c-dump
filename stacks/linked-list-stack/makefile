CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: stack

stack: main.o stack.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c stack.h
stack.o: stack.c stack.h

test: stack
	./stack

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main stack *.o