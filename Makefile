CC = gcc
CFLAGS = -Wall -pedantic -ggdb -Werror 
LDFLAGS = -lspectrum
  
main:	zx-tape-extract.o 
	$(CC) $(CFLAGS) zx-tape-extract.o -o zx-tape-extract $(LDFLAGS)

zx-tape-extract.o: zx-tape-extract.c zx-tape-extract.h
	$(CC) $(CFLAGS) -c zx-tape-extract.c

.PHONY:	clean 

clean:
	/bin/rm -f zx-tape-extract.o zx-tape-extract *~




