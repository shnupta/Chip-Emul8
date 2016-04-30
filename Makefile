CFLAGS=-Wall -g -L/usr/local/lib -lSDL2

all: main

main: cpu.c

clean:
	rm -f main
	rm -f cpu
