CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: red-black-tree

red-black-tree: main.o red-black-tree.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c red-black-tree.h
red-black-tree.o: red-black-tree.c red-black-tree.h __debug__.h

test: red-black-tree
	valgrind --tool=memcheck --leak-check=yes ./red-black-tree

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main red-black-tree *.o vgcore.*