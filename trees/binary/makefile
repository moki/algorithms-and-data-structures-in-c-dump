CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: bst

bst: main.o bst.o lib/queue/queue.o lib/stack/stack.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c bst.h
bst.o: bst.c bst.h lib/queue/queue.h lib/stack/stack.h

lib/queue/queue.o: lib/queue/queue.c lib/queue/queue.h
lib/stack/stack.o: lib/stack/stack.c lib/stack/stack.h


test: bst
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./bst && dot -Tpng bst.gv -O && eog ./bst.gv.png

clean:
	$(RM) main bst item *.o vgcore.* *.gv *.gv.png lib/*/*.o