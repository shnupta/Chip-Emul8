#include "dbg.h"
#include "cpu.h"
#include <string.h>
#include <stdlib.h>

void initialise_cpu(chip8 cpu) {
	memset(cpu.V, 0 , 16);
	cpu.I = 0;
	cpu.pc = 0x200;
	cpu.sp = 0;
	memset(cpu.gfx, 0, 64 * 32);
	cpu.delay_timer = 0;
	cpu.sound_timer = 0;
	cpu.opcode = 0;
	memset(cpu.key, 0, 16);

	// TODO: load fonts

}


void load_rom(chip8 cpu, char *rom_name) {
	FILE *file;
	char *buffer;
	unsigned long file_len;

	//Open file
	file = fopen(rom_name, "rb");
	if (!file)
	{
		log_err("Unable to open file %s", rom_name);
		return;
	}
	
	//Get file length
	fseek(file, 0, SEEK_END);
	file_len=ftell(file);
	fseek(file, 0, SEEK_SET);

	//Allocate memory
	buffer=(char *)malloc(file_len+1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!");
                                fclose(file);
		return;
	}

	//Read file contents into buffer
	fread(buffer, file_len, 1, file);
	fclose(file);



	for(int i = 0; i < file_len; i++) {
		cpu.memory[i + 512] = buffer[i];
	}

	free(buffer);

}









