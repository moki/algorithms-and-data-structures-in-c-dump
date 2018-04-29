CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: search

search: main.o search.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c search.h
search.o: search.c search.h

test: search
	./search

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main search *.o