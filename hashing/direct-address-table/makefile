CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: direct-address-table

direct-address-table: main.o direct-address-table.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c direct-address-table.h
direct-address-table.o: direct-address-table.c direct-address-table.h

test: direct-address-table
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./direct-address-table

clean:
	$(RM) main direct-address-table *.o vgcore.*