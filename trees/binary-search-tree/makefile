CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: binary-search-tree

binary-search-tree: main.o binary-search-tree.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c binary-search-tree.h
binary-search-tree.o: binary-search-tree.c binary-search-tree.h __debug__.h

test: binary-search-tree
	valgrind --tool=memcheck --leak-check=yes ./binary-search-tree

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main binary-search-tree *.o