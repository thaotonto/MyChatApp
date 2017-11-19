#   Network programming - 20171
#   Homework 4: Session management
#   Author: Hoang Van Hai

CC=gcc
# CFLAGS=-w -std=c99

all: server client

server: tcp_server.c user.c message.c controller.c  
	$(CC) $^ -o $@ $(CFLAGS) `mysql_config --cflags --libs`

client: client.c
	$(CC) -o $@ $^ $(CFLAGS)

clean: *
	rm -rf client server