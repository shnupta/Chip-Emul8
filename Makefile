CFLAGS=-Wall -g

all: main

main: cpu.c

clean:
	rm -f main
	rm -f cpu
