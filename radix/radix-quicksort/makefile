CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: sort

sort: main.o sort.o sort-utils.o binary-utils.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c sort.h sort-utils.h binary-utils.h
sort.o: sort.c sort.h sort-utils.c binary-utils.c

test: sort
	./sort

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main sort sort-utils binary-utils *.o