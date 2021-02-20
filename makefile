CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0) -g3 -Wall
LIBS=$(shell pkg-config --libs gtk+-3.0) -lsqlite3

mtag: main.o tags.o handlers.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f mtag *.o

.PHONY: clean