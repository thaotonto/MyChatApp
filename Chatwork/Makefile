CC=gcc

all: client

client: main.c client_handler.c `pkg-config --cflags --libs gtk+-3.0`
	$(CC) -o $@ $^
