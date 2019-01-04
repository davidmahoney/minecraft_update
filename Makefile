CC=cc
CFLAGS=-I/usr/local/include -g -Wall
LDFLAGS=-L/usr/local/lib -lc -lcurl -ljson-c
DEPS = versions.h
OBJ = main.o versions.o update.o
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LDFLAGS := -L/usr/lib/x86_64-linux-gnu -lc -lcurl -ljson-c
	CFLAGS := -g -Wall
endif

.o:	
	$(CC) -c -o $@ $< $(CFLAGS)

all: main.o versions.o update.o
#	$(CC) $(LDFLAGS) -o minecraft_update $(OBJ)
	$(CC) -o minecraft_update $(OBJ) $(LDFLAGS)

clean:
	rm *.o minecraft_update

install: all
	install minecraft_update /usr/local/bin/minecraft_update
	install -g 0 -o 0 -m 0644 man/minecraft_update.1 /usr/local/man/man1/
	gzip /usr/local/man/man1/minecraft_update.1

deinstall:
	rm /usr/local/bin/minecraft_update	
	rm /usr/local/man/man1/minecraft_update.1.gz
