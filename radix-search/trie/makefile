CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: trie

trie: main.o trie.o string.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c trie.h
trie.o: trie.c trie.h item.h string.h
string.o: string.c string.h

test: trie
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./trie

clean:
	$(RM) main trie item *.o vgcore.*