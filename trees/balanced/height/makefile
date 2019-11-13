CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: hbt

hbt: main.o hbt.o lib/stack/stack.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c hbt.h
hbt.o: hbt.c hbt.h lib/stack/stack.h

lib/stack/stack.o: lib/stack/stack.c lib/stack/stack.h


test: hbt
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./hbt && dot -Tpng hbt.gv -O && eog ./hbt.gv.png

clean:
	$(RM) main hbt item *.o vgcore.* *.gv *.gv.png lib/*/*.o