CC=cc
CFLAGS=-I/usr/local/include
LDFLAGS=-L/usr/local/lib -lc -lcurl -ljson-c
DEPS = versions.h
OBJ = main.o versions.o update.o
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LDFLAGS := -L/usr/lib/x86_64-linux-gnu -lc -lcurl -ljson
	CFLAGS :=
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

deinstall:
	rm /usr/local/bin/minecraft_update	
