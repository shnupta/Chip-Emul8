#ifndef _CPU_H
#define _CPU_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint16_t opcode; //an opcode is two bytes

	uint8_t memory[4096]; //has 4K memory

	uint8_t V[16]; //general purpose regsters, VF doubles as a a carry flag

	uint16_t I; //index register
	uint16_t pc; //program counter

	uint8_t gfx[64][32]; //pixels of screen

	uint8_t delay_timer;
	uint8_t sound_timer;

	uint16_t stack[16];
	uint16_t sp;

	uint8_t keys[16]; //hex keypad

	unsigned char key;

	bool draw_flag;

} chip8;

//0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
//0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
//0x200-0xFFF - Program ROM and work RAM


void initialise_cpu(chip8 * cpu);
bool load_rom(chip8 * cpu, const char *name);
void emulate_cycle(chip8 * cpu);
void clear_screen(chip8 *cpu);
void update_timers(chip8 * cpu);
void handle_input(chip8 *cpu, unsigned char key);

#endif
