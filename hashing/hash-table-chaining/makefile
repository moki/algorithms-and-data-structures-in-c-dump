CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: hash-table

hash-table: main.o hash-table.o uint32-djb2-hash.o _random.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c hash-table.h _random.h
hash-table.o: hash-table.c hash-table.h
uint32-djb2-hash.o: uint32-djb2-hash.c uint32-djb2-hash.h
_random.o: _random.c _random.h

test: hash-table
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./hash-table

clean:
	$(RM) main hash-table uint32-djb2-hash _random *.o vgcore.*