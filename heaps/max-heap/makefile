CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: heap

heap: main.o heap.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c heap.h
heap.o: heap.c heap.h

test: heap
	./heap

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main heap *.o