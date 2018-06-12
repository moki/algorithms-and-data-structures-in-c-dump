CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: skip-list

skip-list: main.o skip-list.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c skip-list.h
skip-list.o: skip-list.c skip-list.h

test: skip-list
	valgrind --tool=memcheck --leak-check=yes ./skip-list

clean:
	$(RM) main skip-list *.o vgcore.*