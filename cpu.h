#ifndef _CPU_H
#define _CPU_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint16_t opcode; //an opcode is two bytes

	bool is_running;
	bool render_flag;

	bool wait_for_key;
	bool key_location;

	unsigned char memory[4096]; //has 4K memory

	unsigned char V[16]; //general purpose regsters

	unsigned short I; //index register
	unsigned short pc; //program counter

	unsigned char gfx[64 * 32]; //pixels of screen

	unsigned char delay_timer;
	unsigned char sound_timer;

	unsigned short stack[16];
	unsigned short sp;

	unsigned char key[16]; //hex keypad

} chip8;

//0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
//0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
//0x200-0xFFF - Program ROM and work RAM


void initialise_cpu(chip8 cpu);
void load_rom(chip8 cpu, char *name);
void emulate_cycle(chip8 cpu);

#endif