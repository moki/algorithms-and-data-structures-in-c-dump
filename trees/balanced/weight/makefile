CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: wbt

wbt: main.o wbt.o lib/stack/stack.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c wbt.h
wbt.o: wbt.c wbt.h lib/stack/stack.h

lib/stack/stack.o: lib/stack/stack.c lib/stack/stack.h


test: wbt
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./wbt && dot -Tpng wbt.gv -O && eog ./wbt.gv.png

clean:
	$(RM) main wbt item *.o vgcore.* *.gv *.gv.png lib/*/*.o