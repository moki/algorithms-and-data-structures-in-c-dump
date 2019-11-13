CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: stack

stack: main.o stack.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c stack.h
stack.o: stack.c stack.h

test: stack
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./stack

clean:
	$(RM) main stack item *.o vgcore.*