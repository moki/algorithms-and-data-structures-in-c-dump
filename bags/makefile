CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: bag

bag: main.o bag.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c bag.h
bag.o: bag.c bag.h

test: bag
	./bag

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main bag *.o