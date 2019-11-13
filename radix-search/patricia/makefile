CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: patricia

patricia: main.o patricia.o string.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c patricia.h
patricia.o: patricia.c patricia.h item.h string.h
string.o: string.c string.h

test: patricia
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./patricia

clean:
	$(RM) main patricia item *.o vgcore.*