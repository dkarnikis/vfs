SOURCE=vfs.c test.c
CC=gcc
BIN=vfs
LIBS= 
CFLAGS=-g -Wall -Wextra -O3
all:
	$(CC) -o $(BIN) $(SOURCE) $(LIBS) $(CFLAGS)
clean:
	rm -f $(BIN)
