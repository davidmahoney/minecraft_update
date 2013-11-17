CC=cc
CFLAGS=-I/usr/local/include
LDFLAGS=-L/usr/local/lib -lc -lcurl -ljson-c
DEPS = versions.h
OBJ = main.o versions.o update.o

.o:	
	$(CC) -c -o $@ $< $(CFLAGS)

all: main.o versions.o update.o
	$(CC) $(LDFLAGS) -o minecraft_update $(OBJ)

clean:
	rm *.o minecraft_update

install: all
	install minecraft_update /usr/local/bin/minecraft_update

deinstall:
	rm /usr/local/bin/minecraft_update	
