SOURCE=vfs.c main.c
CC=gcc
BIN=vfs
LIBS= 
CFLAGS=-g -Wall -Wextra -Os
all:
	$(CC) -o $(BIN) $(SOURCE) $(LIBS) $(CFLAGS)
clean:
	rm -f $(BIN)
