CFLAGS=-Wall -std=c99 -g -lGLU -lGL -lglut 
SRC=ch8.c cpu.c

build: ch8

ch8: 
	cc $(SRC) $(CFLAGS) -o ch8
clean:
	rm -f main
	rm -f cpu
	rm -f ch8
