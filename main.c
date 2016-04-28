#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

chip8 cpu;

int main(int argc, char const *argv[])
{
	// TODO: implement GLUT graphics and input


	initialise_cpu(&cpu);
	load_rom(&cpu, "PONG");

	//emulate_cycle(&cpu);

	
	for(;;) {
		emulate_cycle(&cpu);
	}

	return 0;
}


//TODO: add graphics, input and window functions - reference main.cpp from other project

//FINAL TODO: add SDL_mixer.h for sounds???
