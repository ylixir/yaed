CC=gcc
LFLAGS=-export-dynamic $(shell pkg-config --libs gtk+-2.0 gtksourceview-2.0)
INC=$(shell pkg-config --cflags gtk+-2.0 gtksourceview-2.0)
CFLAGS=-std=c99 -ansi -pedantic -Wall -Wextra -Werror -g $(INC)

yaed: main.o
	$(CC) -o $@ main.o $(LFLAGS)
%.o: %.c
	$(CC) -c $< $(CFLAGS)
main.c:

clean:
	rm -f yaed
	rm -f *.o
