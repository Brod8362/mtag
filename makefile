CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0) -g3
LIBS=-lsqlite3

mtag: main.o tags.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f mtag *.o

.PHONY: clean