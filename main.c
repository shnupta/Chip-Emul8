#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

chip8 cpu;

int main(int argc, char const *argv[])
{
	// TODO: implement OpenGL graphics and input

	initialise_cpu(cpu); //TODO
	load_rom(cpu, "PONG"); //TODO

	emulate_cycle(cpu);

	
	/*emulation loop
	for(int i = 0; i < 4; i++) {
		emulate_cycle(cpu); //TODO

	}*/

	return 0;
}
