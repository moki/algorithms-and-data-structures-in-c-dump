CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: rbt

rbt: main.o rbt.o lib/stack/stack.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c rbt.h
rbt.o: rbt.c rbt.h lib/stack/stack.h

lib/stack/stack.o: lib/stack/stack.c lib/stack/stack.h


test: rbt
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./rbt
	# valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./rbt && dot -Tpng rbt.gv -O && eog ./rbt.gv.png

clean:
	$(RM) main rbt item *.o vgcore.* *.gv *.gv.png lib/*/*.o