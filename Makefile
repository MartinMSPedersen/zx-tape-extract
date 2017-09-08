CC = gcc
CFLAGS = -Wall -pedantic -ggdb -Werror  -Wextra
LDFLAGS = -lspectrum
 
all:	zx-tape-extract tap2bas

tap2bas:	tap2bas.o
	$(CC) $(CFLAGS) tap2bas.o -o tap2bas $(LDFLAGS)

zx-tape-extract:	zx-tape-extract.o 
	$(CC) $(CFLAGS) zx-tape-extract.o -o zx-tape-extract $(LDFLAGS)

tap2bas.o: tap2bas.c tap2bas.h
	$(CC) $(CFLAGS) -c tap2bas.c

zx-tape-extract.o: zx-tape-extract.c zx-tape-extract.h
	$(CC) $(CFLAGS) -c zx-tape-extract.c

.PHONY:	clean all

clean:
	/bin/rm -f zx-tape-extract.o zx-tape-extract *~ tap2bas tap2bas.o




