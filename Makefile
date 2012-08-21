CC=gcc
CFLAGS=-Wall -g -O2 -std=c99 -I.
LDFLAGS=-lpcre
SOURCES=main.c
EXECUTABLE=build/gitproxy
DIR=build/

all: init sources

init:
	mkdir -p $(DIR)

sources:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(EXECUTABLE)

install: 
	cp build/gitproxy /usr/bin/gitproxy

clean:
	rm -rf $(DIR)