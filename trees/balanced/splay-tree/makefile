CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: splay-tree

splay-tree: main.o splay-tree.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c splay-tree.h
splay-tree.o: splay-tree.c splay-tree.h __debug__.h

test: splay-tree
	valgrind --tool=memcheck --leak-check=yes ./splay-tree

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main splay-tree *.o