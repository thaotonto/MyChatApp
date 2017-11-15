

CC=gcc
CFLAGS=-w -std=c99

all: server client

server: tcp_server.c user.c session.c app.c
	$(CC) -o $@ $^ $(CFLAGS)

client: tcp_client.c
	$(CC) -o $@ $^ $(CFLAGS)

clean: *
	rm -rf client server