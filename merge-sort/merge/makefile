CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3

all: merge

merge: main.o merge.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c merge.h merge.c
merge.o: merge.c merge.h

test: merge
	./merge

# This lets you type "make clean" and get rid of anything you can
# rebuild.  The $(RM) variable is predefined to "rm -f"
clean:
	$(RM) main merge *.o