CFLAGS=-Wall -g -I/System/Library/Frameworks/GLUT.framework/Headers -I/System/Library/Frameworks/SDL2_mixer.framework/Headers
LDFLAGS=-framework OpenGL	-framework	GLUT  -framework SDL2_mixer


build: ch8

ch8: ch8.c cpu.c

clean:
	rm -f main
	rm -f cpu
	rm -f ch8
