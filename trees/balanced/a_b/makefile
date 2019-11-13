CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: abt

abt: main.o abt.o lib/stack/stack.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c abt.h
abt.o: abt.c abt.h lib/stack/stack.h

lib/stack/stack.o: lib/stack/stack.c lib/stack/stack.h


test: abt
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./abt
	# valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./abt && dot -Tpng abt.gv -O && eog ./abt.gv.png

clean:
	$(RM) main abt item *.o vgcore.* *.gv *.gv.png lib/*/*.o