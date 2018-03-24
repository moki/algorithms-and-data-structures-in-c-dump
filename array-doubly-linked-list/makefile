CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: list

list: main.o list.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c list.h
list.o: list.c list.h

test: list
	./list

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main list *.o