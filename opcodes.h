#ifndef _opcodes_h
#define _opcodes_h
#include <stdint.h>
#include <stdlib.h>

//instructions site: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#00E0

#define X(x)   ((x & 0x0F00) >> 8)
#define Y(x)   ((x & 0x00F0) >> 4)
#define N(x)   (x & 0x000F)
#define KK(x)  (x & 0x00FF)
#define NNN(x) (x & 0x0FFF)


void decode_opcode(chip8 * cpu);

void decode_opcode(chip8 * cpu) {
	switch(cpu->opcode & 0xF000) {
		case 0x0000: //0nnn
			switch(cpu->opcode) {
				case 0x00E0: //00E0: CLS - clear the display
					clear_screen(cpu);
					cpu->pc += 2;
				break;

				case 0x00EE: //00EE: RET - return from a subroutine, sets PC = stack[sp] then sp--
					cpu->sp -= 1;
					cpu->pc = cpu->stack[cpu->sp];
					cpu->pc += 2;

					
				break;

				default:
					log_err("Unknown opcode: %X", cpu->opcode);
					cpu->pc += 2;
					break;

				}
		break;

		case 0x1000: //1nnn: JP addr - jump to location nnn
			cpu->pc = NNN(cpu->opcode);
		break;

		case 0x2000: //2nnn: CALL addr - call subroutine at nnn
			//add error checking to prevent stck overflow
			
			
			cpu->stack[cpu->sp] = cpu->pc;
			cpu->sp += 1;
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

		case 0x8000: 
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
					cpu->V[X(cpu->opcode)] = result &0xFF;

					cpu->pc += 2;
				}
			break;

			case 0x0005: //8XY5: SUB Vx, Vy - set Vx = Vx - Vy, set VF = NOT borrow
			{
				int vx = cpu->V[X(cpu->opcode)];
                int vy = cpu->V[Y(cpu->opcode)];

                cpu->V[0xF] = vx < vy ? 0 : 1;

                cpu->V[X(cpu->opcode)] = vx -vy;

                cpu->pc += 2;
            }
			break;
			
			
			case 0x0006: //8XY6: SHR Vx {, Vy} - Vx = Vx SHR 1.
				cpu->V[0xF] = cpu->V[X(cpu->opcode)] & 0x01;
				cpu->V[X(cpu->opcode)] >>= 1;
				cpu->pc += 2;
			break;

			case 0x0007: //8XY7: SUBN Vx, Vy - set Vx = Vy - Vx, set VF NOT borrow
			{
				int vx = cpu->V[X(cpu->opcode)];
				int vy = cpu->V[Y(cpu->opcode)];

				cpu->V[0xF] = vx > vy ? 0 : 1;

				cpu->V[X(cpu->opcode)] = vy - vx;

				cpu->pc += 2;
			}
			break;

			
			case 0x000E: //8XYE: SHL Vx {, Vy} - set Vx = Vx SHL 1
				cpu->V[0xF] = (cpu->V[X(cpu->opcode)] & 0x80) >> 7;
				cpu->V[X(cpu->opcode)] <<= 1;
				cpu->pc += 2;
			break;

			default:
				log_err("Unknown opcode: %X", cpu->opcode);
				cpu->pc += 2;
		}
		break;	

		case 0x9000: //9XY0: SNE Vx,Vy - skip next instruction if Vx != VY
			cpu->pc += cpu->V[X(cpu->opcode)] != cpu->V[Y(cpu->opcode)] ? 4 : 2;
		break;

		case 0xA000: //ANNN: LD I, addr  set  = NNN
			cpu->I = NNN(cpu->opcode);
			cpu->pc += 2;
		break;

		case 0xB000: //BNNN: JP v0, addr - jump to location nnn + v0
			cpu->pc = NNN(cpu->opcode) + cpu->V[0x0];
		break;

		case 0xC000: //CXKK - RNDVx, byte - set Vx = random byte AND KK
			cpu->V[X(cpu->opcode)] = (rand() % 256) & KK(cpu->opcode);
			cpu->pc += 2;
		break;

		case 0xD000: //DXYN: DRW Vx, Vy, nibble - display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
		{
			int height = cpu->opcode &0x000F;
			int x_coord = cpu->V[X(cpu->opcode)];
			int y_coord = cpu->V[Y(cpu->opcode)];

			// because the sprite is represented by hexadecimal numbers
			// bitwise operators are necessary to obtain each pixel
			int ands[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };

			cpu->V[0xF] = 0;

			for(int i = 0; i < height; i++) {
				for(int j = 0; j < 8; j++) {
					if(x_coord + j == 64) {
						x_coord = -j;
					}
					if(y_coord + i == 32) {
						y_coord = -i;
					}

					if(cpu->gfx[x_coord + j][y_coord + i] == 1 && ((cpu->memory[cpu->I + i] & ands[j]) >> (8 - j - 1)) == 1) {
						cpu->V[0xF] = 1;
					}

					cpu->gfx[x_coord + j][y_coord + i] = cpu->gfx[x_coord + j][y_coord + i] ^ ((cpu->memory[cpu->I + i] & ands[j]) >> (8 - j - 1));
				}
				x_coord = cpu->V[X(cpu->opcode)];
				y_coord = cpu->V[Y(cpu->opcode)];
			}

			cpu->draw_flag = true;
			cpu->pc += 2;
		}
		break;

		case 0xE000:
			switch(cpu->opcode & 0x00FF) {
				case 0x009E: //EX9E: SKP Vx - skip next instruction if key with the value of Vx is pressed.
					cpu->pc += cpu->key == cpu->V[X(cpu->opcode)] ? 4 : 2;
				break;

				case 0x00A1: //EXA1: SKNP Vx - Skip next instruction if key with the value of Vx is not pressed.
					cpu->pc += cpu->key != cpu->V[X(cpu->opcode)] ? 4 : 2;
				break;

				default:
					log_err("Unknown opcode %X", cpu->opcode);
					exit(1);
			}
		break;


		case 0xF000:
			switch(cpu->opcode & 0x00FF) {
				case 0x0007: //FX07: LD Vx, DT - Set Vx = delay timer value.
					cpu->V[X(cpu->opcode)] = cpu->delay_timer;
					cpu->pc += 2;
				break;

				case 0x000A: //Ld Vx, K - Wait for a key press, store the value of the key in Vx.
				{	
					if(cpu->key == 0) {
						return;
					} else {
						cpu->V[X(cpu->opcode)] = cpu->key;
					}
					cpu->pc += 2;
				}
				break;

				case 0x0015: //FX15: LD T, Vx - set delay timer = Vx
					cpu->delay_timer = cpu->V[X(cpu->opcode)];
					cpu->pc += 2;
				break;

				case 0x0018: //FX18: LD ST, Vx - set sound timer = Vx
					cpu->sound_timer = cpu->V[X(cpu->opcode)];
					cpu->pc += 2;
				break;

				case 0x001E: //FX1E: ADD I, Vx - set I = I + Vx
				
					cpu->I += cpu->V[X(cpu->opcode)];
					cpu->pc += 2;
				break;

				case 0x0029: //FX29: LD , Vx - set I = location of sprite for digit Vx.
					cpu->I = cpu->V[X(cpu->opcode)] * 5;
					cpu->pc += 2;
				break;

				case 0x0033: //FX33: LD B, Vx - Store BCD representation of Vx in memory locations I, I+1, and I+2.
					cpu->memory[cpu->I] = cpu->V[X(cpu->opcode)] / 100; //store the hundreds value of vx in memory location I
					cpu->memory[cpu->I + 1] = (cpu->V[X(cpu->opcode)] % 10) % 10; //store the tens value of vx in memory location I + 1
					cpu->memory[cpu->I + 2] = cpu->V[X(cpu->opcode)] % 10; //store the units value of vx in memory location I + 2
					cpu->pc += 2;
				break;

				case 0x0055: //FX55: LD [I], Vx - Store registers V0 through Vx in memory starting at location I.
					for (int i = 0; i < X(cpu->opcode) + 1; i++) {
						cpu->memory[cpu->I + i] = cpu->V[i];
					}
					cpu->I = cpu->I + X(cpu->opcode) + 1;
					cpu->pc += 2;
				break;

				case 0x0065: //FX6: Ld Vx, [I] - Read registers V0 through Vx from memory starting at location I.
					for (int i = 0; i < X(cpu->opcode) + 1; i++) {
						cpu->V[i] = cpu->memory[cpu->I + i];
					}
					cpu->I = cpu->I + X(cpu->opcode) + 1;
					cpu->pc += 2;
				break;

				default:
					log_err("Unknown opcode %X", cpu->opcode);
					exit(1);


			}
		break;

		default:
			log_err("Unknown opcode %X", cpu->opcode);
			exit(1);

	}
}


#endif