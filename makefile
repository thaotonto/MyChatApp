CC=gcc
# CFLAGS=-w -std=c99

all: server client

server: tcp_server.c user.c message.c controller.c listUserOnline.c
	$(CC) $^ -o $@ $(CFLAGS) `mysql_config --cflags --libs`

client: client.c
	$(CC) -o $@ $^ $(CFLAGS)

clean: *
	rm -rf client server