default: c

all: c

CC=gcc

c: server client

client: client.c
	$(CC) client.c -o client

server: server.c
	$(CC) server.c -o server

clean:
	rm -f server client *.o
