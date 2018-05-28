CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: heap

heap: main.o heap.o sort-utils.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c heap.h sort-utils.h
sort-utils.o: sort-utils.c sort-utils.h
heap.o: heap.c heap.h

test: heap
	./heap

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main heap sort-utils *.o