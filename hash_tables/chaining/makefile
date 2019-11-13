CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: hash-table.out

hash-table.out: main.o hash-table.o lib/djb2/djb2.o lib/_random/_random.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c hash-table.h lib/djb2/djb2.h lib/_random/_random.h
hash-table.o: hash-table.c hash-table.h

lib/djb2/djb2.o: lib/djb2/djb2.c lib/djb2/djb2.h
lib/_random/_random.o: lib/_random/_random.c lib/_random/_random.h


test: hash-table.out
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./hash-table.out
	# valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./hash-table && dot -Tpng hash-table.gv -O && eog ./hash-table.gv.png

clean:
	$(RM) main hash-table.out *.o vgcore.* *.gv *.gv.png lib/*/*.o