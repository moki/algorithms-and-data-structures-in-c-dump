CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: deq

deq: main.o deq.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c deq.h
deq.o: deq.c deq.h

test: deq
	./deq

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main deq *.o