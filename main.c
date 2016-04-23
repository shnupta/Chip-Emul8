#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "opcodes.h"

chip8 cpu;

int main(int argc, char const *argv[])
{
	// TODO: implement OpenGL graphics and input

	initialise_cpu(cpu); //TODO
	load_rom(cpu, "PONG"); //TODO


	/*
	//emulation loop
	for(;;) {
		emulate_cycle(cpu); //TODO

		if(draw_flag(cpu)) { //TODO
			draw_graphics(); //TODO
		}


		set_keys(cpu); //TODO
	}*/

	return 0;
}
