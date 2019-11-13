CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: queue

queue: main.o queue.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c queue.h
queue.o: queue.c queue.h

test: queue
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./queue

clean:
	$(RM) main queue item *.o vgcore.*