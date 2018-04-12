PREFIX = /usr

CC = cc
CFLAGS 	= -std=c99 -pedantic -Wall -Werror

SRC = juu.c
OBJ = $(SRC:.c=.o)

juu: $(OBJ)
	$(CC) -o $@ $(OBJ)

$(OBJ):

.c.o:
	$(CC) -c $(CFLAGS) $<

install: juu
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f juu $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/juu

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/juu

clean:
	rm -f juu $(OBJ)

.PHONY: install uninstall clean