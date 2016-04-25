#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

chip8 cpu;

int main(int argc, char const *argv[])
{
	// TODO: implement OpenGL graphics and input

	initialise_cpu(&cpu);
	load_rom(&cpu, "PONG");

	for(;;) {
		emulate_cycle(&cpu);
	}

	return 0;
}
