#include "dbg.h"
#include "cpu.h"
#include "opcodes.h"
#include <string.h>
#include <stdlib.h>

unsigned char font_set[80] =
{
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

static int key_map[0x10] = {
    SDLK_0,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_4,
    SDLK_5,
    SDLK_6,
    SDLK_7,
    SDLK_8,
    SDLK_9,
    SDLK_a,
    SDLK_b,
    SDLK_c,
    SDLK_d,
    SDLK_e,
    SDLK_f
};

void initialise_cpu(chip8 cpu) {
	memset(cpu.memory, 0, 4096); //reset memory
	memset(cpu.V, 0, 16); //reset registers
	memset(cpu.stack, 0, 16); //reset stack
	memset(cpu.key, 0, 16); //reset keys
	//defaults
	cpu.I = 0;
	cpu.pc = 0x200;
	cpu.sp = 0;
	cpu.delay_timer = 0;
	cpu.sound_timer = 0;
	cpu.opcode = 0;

	// load fonts
	for (int i = 0; i < 80; i++) {
		cpu.memory[i] = font_set[i];
	}

	memset(cpu.gfx, 0, 64 * 32); //reset screen

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
		//printf("%04x\n", cpu.memory[i + 512] & 0xF000);
	}

	free(buffer);

}


void emulate_cycle(chip8 cpu) {
	//fetch opcode

	//decode opcode
	decode_opcode(cpu);
}










