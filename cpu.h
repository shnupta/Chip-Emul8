#ifndef _CPU_H
#define _CPU_H

typedef struct {
	unsigned short opcode; //an opcode is two bytes

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

void initialise_cpu(chip8 cpu);
void load_rom(chip8 cpu, char *name);

#endif