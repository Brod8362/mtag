CC=gcc
CFLAGS=-g3
LIBS=-lsqlite3

mtag: main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f mtag *.o

.PHONY: clean