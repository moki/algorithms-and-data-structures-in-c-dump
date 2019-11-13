CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: trie

trie: main.o trie.o lib/stack/stack.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c trie.h
trie.o: trie.c trie.h lib/stack/stack.h

lib/stack/stack.o: lib/stack/stack.c lib/stack/stack.h


test: trie
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./trie
	# valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./trie && dot -Tpng trie.gv -O && eog ./trie.gv.png

clean:
	$(RM) main trie *.o vgcore.* *.gv *.gv.png lib/*/*.o