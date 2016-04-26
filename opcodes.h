#ifndef _opcodes_h
#define _opcodes_h
#include <SDL2/SDL.h>
#include <stdint.h>

//instructions site: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#00E0

#define X(x)   ((x & 0x0F00) >> 8)
#define Y(x)   ((x & 0x00F0) >> 4)
#define N(x)   (x & 0x000F)
#define KK(x)  (x & 0x00FF)
#define NNN(x) (x & 0x0FFF)


void decode_opcode(chip8 * cpu);

void decode_opcode(chip8 * cpu) {
	switch(cpu->opcode & 0xF000) {
		switch(cpu->opcode & 0x000F) {
			case 0x00E0: //00E0: CLS - clear the display
				clear_screen(cpu);
				cpu->pc += 2;
			break;

			case 0x00EE: //00EE: RET - return from a subroutine, sets PC = stack[sp] then sp--
				cpu->pc = cpu->stack[cpu->sp];
				--cpu->sp;
				cpu->pc += 2;
			break;

			default:
				log_err("Unknown opcde: %X", cpu->opcode);

			}

		case 0x1000: //1nnn: JP addr - jump to location nnn
			cpu->pc = NNN(cpu->opcode);
		break;

		case 0x2000: //2nnn: CALL addr - call subroutine at nnn
			//add error checking to prevent stck overflow
			if((cpu->sp + 1) >= sizeof(cpu->stack)) {
				log_err("Stack overflow exiting...");
				exit(1);
				break;
			}	

			cpu->sp++;
			cpu->stack[cpu->sp] = cpu->pc;
			cpu->pc = NNN(cpu->opcode);
		break;

		case 0x3000: //3xkk: SE Vx, byte - skip next instruction if Vx == kk
			cpu->pc += cpu->V[X(cpu->opcode)] == KK(cpu->opcode) ? 4 : 2; //Ternary truth test, read X ? Y : Z as "if X then Y else Z".
		break;

		case 0x4000: //4XKK: SNE Vx, byte - skip next instruction if Vx != kk
			cpu->pc += cpu->V[X(cpu->opcode)] != KK(cpu->opcode) ? 4 : 2;
		break;

		case 0x5000: //5XY0: SE Vx, Vy - skips next instrution if Vx == Vy
			cpu->pc += cpu->V[X(cpu->opcode)] == cpu->V[Y(cpu->opcode)] ? 4 : 2;
		break;

		case 0x6000: //6XKK: LD Vx, byte - set V == KK
			cpu->V[X(cpu->opcode)] = KK(cpu->opcode);
			cpu->pc += 2;
		break;

		case 0x7000: //7XKK: ADD Vx, byte - set Vx = Vx + kk
			cpu->V[X(cpu->opcode)] += KK(cpu->opcode);
			cpu->pc += 2;
		break;

		switch(cpu->opcode & 0x000F) {
			case 0x0000: //8XY0: LD Vx, Vy - set Vx = Vy
				cpu->V[X(cpu->opcode)] = cpu->V[Y(cpu->opcode)];
				cpu->pc += 2;
			break;

			case 0x0001: //8XY1: OR Vx, Vy - set Vx = Vx Or Vy
				cpu->V[X(cpu->opcode)] |= cpu->V[Y(cpu->opcode)];
				cpu->pc += 2;
			break;

			case 0x0002: //8XY2: AND Vx, Vy - set Vx = Vx AND Vy
				cpu->V[X(cpu->opcode)] &= cpu->V[Y(cpu->opcode)];
				cpu->pc += 2;
			break;

			case 0x0003: //8XY3: XOR Vx, Vy - set Vx = Vx XOR Vy
				cpu->V[X(cpu->opcode)] ^= cpu->V[Y(cpu->opcode)];
				cpu->pc += 2;
			break;


			case 0x0004: //8XY4: ADD Vx, Vy - set Vx = Vx + Vy, set VF = carry
			{
				int vx = cpu->V[X(cpu->opcode)];
				int vy = cpu->V[Y(cpu->opcode)];

				int result = vx + vy;

				cpu->V[0xF] = result > 255 ? 1 : 0; //VF is doubled as a carry flag register

				//only the lower 8 bits are stored ad kept in Vx
				cpu->V[X(cpu->opcode)] = result & 0xFF;

				cpu->pc += 2;
			}
			break;

			case 0x0005: //8XY5: SUB Vx, Vy - set Vx = Vx - Vy, set VF = NOT borrow
			
				cpu->V[0xF] = cpu->V[X(cpu->opcode)] > cpu->V[Y(cpu->opcode)] ? 1 : 0;

				cpu->V[X(cpu->opcode)] = cpu->V[X(cpu->opcode)] - cpu->V[Y(cpu->opcode)];

				cpu->pc += 2;
			break;
			
			//CHECK THIS INSTRUCTION
			case 0x0006: //8XY6: SHR Vx {, Vy} - Vx = Vx SHR 1.
				cpu->V[0xF] = cpu->V[X(cpu->opcode)] & 0x01;
				cpu->V[X(cpu->opcode)] >>= 1;
				cpu->pc += 2;
			break;

			case 0x0007: //8XY7: SUBN Vx, Vy - set Vx = Vy - Vx, set VF NOT borrow
				cpu->V[0xF] = cpu->V[Y(cpu->opcode)] > cpu->V[X(cpu->opcode)] ? 1 : 0;
				cpu->V[X(cpu->opcode)] = cpu->V[Y(cpu->opcode)] - cpu->V[X(cpu->opcode)];
				cpu->pc += 2;
			break;

			//CHECK THISINSTRUCTION
			case 0x000E: //8XYE: SHL Vx {, Vy} - set Vx = Vx SHL 1
				cpu->V[0xF] = (cpu->V[X(cpu->opcode)] & 0x80) >> 7;
				cpu->V[X(cpu->opcode)] <<= 1;
				cpu->pc += 2;
			break;

			default:
				log_err("Unknown opcode: %X", cpu->opcode);


		}	

		//TODO: complete next 16 instructions, then test 

	}
}


#endif