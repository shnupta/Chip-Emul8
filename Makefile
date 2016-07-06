CFLAGS=-Wall -g -lGL -lGLU -lglut


build: ch8

ch8: ch8.c cpu.c

clean:
	rm -f main
	rm -f cpu
	rm -f ch8
