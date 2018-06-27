CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: digital-search-tree

digital-search-tree: main.o digital-search-tree.o string.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c digital-search-tree.h
digital-search-tree.o: digital-search-tree.c digital-search-tree.h item.h string.h
string.o: string.c string.h

test: digital-search-tree
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./digital-search-tree

clean:
	$(RM) main digital-search-tree item *.o vgcore.*