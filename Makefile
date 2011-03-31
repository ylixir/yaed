CC=gcc
#INC=$(shell pkg-config --cflags gtk+-2.0 gtksourceview-2.0)
INC=`pkg-config --cflags gtk+-2.0 gtksourceview-2.0`
ifdef OSX
	LFLAGS=-arch i386 $(shell pkg-config --libs gtk+-2.0 gtksourceview-2.0)
	CFLAGS=-arch i386 -std=c99 -pedantic -Wall -Wextra -Werror -g $(INC)
else
#	LFLAGS=-export-dynamic $(shell pkg-config --libs gtk+-2.0 gtksourceview-2.0)
	LFLAGS=-export-dynamic `pkg-config --libs gtk+-2.0 gtksourceview-2.0`
	CFLAGS=-std=c99 -pedantic -Wall -Wextra -Werror -g $(INC)
endif

yaed: main.o spider.o source-model.o source-view.o tab-label.o tab-contents.o location-bar.o
	$(CC) -o $@ $^ $(LFLAGS)
mac:
	echo "make OSX=yes yaed; exit;"|~/.local/bin/jhbuild shell
%.o: %.c
	$(CC) -c $< $(CFLAGS)
main.c: spider.h
spider.c: spider.h
source-model.c: source-model.h spider.h
source-view.c: source-view.h tab-label.h tab-contents.h
tab-label.c: tab-label.h spider.h
tab-contents.c: tab-contents.h location-bar.h
location-bar.c: location-bar.h spider.h
spider.h: source-view.h
source-view.h: source-model.h
source-model.h:
tab-label.h: source-view.h source-model.h
tab-contents.h: source-view.h source-model.h
location-bar.h: source-model.h source-view.h
clean:
	rm -f yaed
	rm -f *.o
