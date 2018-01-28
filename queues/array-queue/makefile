CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: queue

queue: main.o queue.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c queue.h
queue.o: queue.c queue.h

test: queue
	./queue

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main queue *.o