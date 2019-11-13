CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: hash-table

hash-table: main.o hash-table.o uint32_t-djb2-hash.o uint32_t-sdbm-hash.o _random.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c hash-table.h uint32_t-djb2-hash.h uint32_t-sdbm-hash.h _random.h
hash-table.o: hash-table.c hash-table.h
uint32_t-djb2-hash.o: uint32_t-djb2-hash.c uint32_t-djb2-hash.h
uint32_t-sdbm-hash.o: uint32_t-sdbm-hash.c uint32_t-sdbm-hash.h
_random.o: _random.c _random.h


test: hash-table
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./hash-table

clean:
	$(RM) main hash-table uint32_t-djb2-hash uint32_t-sdbm-hash *.o vgcore.*