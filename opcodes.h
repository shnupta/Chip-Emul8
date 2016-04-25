#ifndef _opcodes_h
#define _opcodes_h
#include <SDL2/SDL.h>
#include <stdint.h>

//instrutions site: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#00E0

#define X(x)   ((x & 0x0F00) >> 8)
#define Y(x)   ((x & 0x00F0) >> 4)
#define N(x)   (x & 0x000F)
#define KK(x)  (x & 0x00FF)
#define NNN(x) (x & 0x0FFF)


void decode_opcode(chip8 * cpu);
void clear_screen(chip8 * cpu);

void decode_opcode(chip8 * cpu) {

	switch(cpu->opcode & 0xF000) {
		case 0x0000:
			switch(cpu->opcode & 0x00FF) {
				case 0x00E0://00E0: clears the screen
					clear_screen(cpu); //TODO
					cpu->pc += 2;
					break;

				case 0x00EE: //00EE: returns a subroutine
					cpu->pc = cpu->stack[cpu->sp];
					--cpu->sp;
					cpu->pc += 2;
					break;

				default:
					log_err("Unknown opcode (%X)", cpu->opcode);
					cpu->pc += 2;
					break;
			}
			break;

		case 0x1000: //1NNN: jumps to address NNN
			cpu->pc = NNN(cpu->opcode);
			break;

		case 0x2000: //2NNN: calls subroutine at NNN
			{
				if((cpu->sp + 1) >= 16) {
					log_err("Stack overflow. Exiting.");
					cpu->is_running = false;
					//exit(0);
					break;
				}
			}

			cpu->stack[cpu->sp] = cpu->pc;
			cpu->sp += 1;
			cpu->pc = NNN(cpu->opcode);
			break;

		case 0x3000: //3XKK: skips the next instruction if VX equals NN
			cpu->pc += cpu->V[X(cpu->opcode)] == KK(cpu->opcode) ? 4 : 2;
			break;

		case 0x4000: //4XKK: skips next instruction if VX not equals VY
			cpu->pc += cpu->V[X(cpu->opcode)] != KK(cpu->opcode) ? 4 : 2;
			break;

		case 0x5000: //5XY0: skips next instruction if VX equals VY
			cpu->pc += cpu->V[X(cpu->opcode)] == cpu->V[Y(cpu->opcode)] ? 4 : 2;
			break;

		case 0x6000: //6XKK: sets VX to KK
			cpu->V[X(cpu->opcode)] =  KK(cpu->opcode);
			cpu->pc += 2;
			break;

		case 0x7000: //7XKK: set VX = VX + KK
			cpu->V[X(cpu->opcode)] = cpu->V[X(cpu->opcode)] + KK(cpu->opcode);
			cpu->pc += 2;
			break;

		case 0x8000:
			switch(cpu->opcode & 0x000F) {
				case 0x0000: //8XY0: sets VX = VY
					cpu->V[X(cpu->opcode)] = Y(cpu->opcode);
					cpu->pc += 2;
					break;

				case 0x0001: //8XY1: set VX = VX or VY
					cpu->V[X(cpu->opcode)] |= Y(cpu->opcode);
					cpu->pc += 2;
					break;

				case 0x0002: //8XY2: set VX = VX and VY
					cpu->V[X(cpu->opcode)] &= Y(cpu->opcode);
					cpu->pc += 2;
					break;

				case 0x0003: //8XY3: set VX = VX Xor VY
					cpu->V[X(cpu->opcode)] ^= Y(cpu->opcode);
					cpu->pc += 2;
					break;

				case 0x0004: //8XY4: ADD set VX = VX + VY, Set VF = carry
				{
					int vx = cpu->V[X(cpu->opcode)];
					int vy = cpu->V[Y(cpu->opcode)];

					int results = vx + vy;

					cpu->V[0xF] = results > 255 ? 1 : 0;

					cpu->V[X(cpu->opcode)] = results & 0xFF;

					cpu->pc += 2;
				}
				break;

				case 0x0005: //8XY5: SUB vx vy
				{
					int vx = cpu->V[X(cpu->opcode)];
					int vy = cpu->V[Y(cpu->opcode)];

					cpu->V[0xF] = vy > vx ? 0 : 1;

					cpu->V[X(cpu->opcode)] = vx - vy;
					cpu->pc += 2;
				}
				break;

				case 0x0006: //8XY6: SHR vx {, VY}
					cpu->V[0xF] = cpu->V[X(cpu->opcode)] & 0x01;
					cpu->V[X(cpu->opcode)] >>= 1;
					cpu->pc += 2;
					break;

				case 0x0007: //8XY7: SUBN vx, vy
				{
					int vx = cpu->V[X(cpu->opcode)];
					int vy = cpu->V[Y(cpu->opcode)];

					cpu->V[0xF] = vy > vx ? 0 : 1;
					cpu->V[X(cpu->opcode)] = vx - vy;
					cpu->pc += 2;
				}
				break;

				case 0x000E: //8XYE: SHL vx {, vy}
					cpu->V[0xF] = (cpu->V[X(cpu->opcode)] & 0x80) >> 7;
					cpu->V[X(cpu->opcode)] <<= 1;
					cpu->pc += 2;
					break;

				default:
					log_err("Unknown instruction (%X)", cpu->opcode);
					cpu->pc += 2;
					break;
			}
			break;

			case 0x9000: //9XY0: SNE vx, vy
				cpu->pc += cpu->V[X(cpu->opcode)] != cpu->V[Y(cpu->opcode)] ? 4 : 2;
				break;

			case 0xA000: //LD I, addr
				//set I = nnn
				cpu->I = NNN(cpu->opcode);
				break;

			case 0xB000: //JP V0, addr
				//jump to location NNN + V0
				cpu->pc = NNN(cpu->opcode) + cpu->V[0x00];
				break;

			case 0xC000: //CXKK: rnd vx, byte
				//set vx = random byte and kk
				cpu->V[X(cpu->opcode)] = (rand() %256) && KK(cpu->opcode);
				cpu->pc += 2;
				break;

			case 0xD000: //DXYN DRW vx, vy, nibble
			//display n-byte sprite starting at memory location I at (vx, vy), set VF = collision.
			{
				uint8_t vx = cpu->V[X(cpu->opcode)];
				uint8_t vy = cpu->V[Y(cpu->opcode)];

				cpu->V[0xF] = 0;

				uint8_t pixel = 0;

				int width = 0;
				int height = 0;

				for (uint8_t y = 0; y < N(cpu->opcode); y++) {
					pixel = cpu->memory[cpu->I + y];
					for (uint8_t x = 0; x < 8; ++x) {
						if ((pixel & (0x80 >> x)) != 0) {
							width = (vx + x) > 64 ? 64 : vx + x;
							height = (vy + y) > 32 ? 32 : vy + y;

							if(cpu->gfx[(width + (height * 64))] == 1) {
								cpu->V[0xF] = 1;
							}

							cpu->gfx[width + (height * 64)] ^= 1;
						}
					}
				}

				cpu->render_flag = true;
				cpu->pc += 2;
			}

			case 0xE000: 
				switch(cpu->opcode & 0x00FF) {
					case 0x9E: //EX9E: SKP vx
						cpu->pc += cpu->key[cpu->V[X(cpu->opcode)]] ? 4 : 2;
						break;

					case 0xA1: //EXA1: SKNP vx
						cpu->pc += cpu->key[cpu->V[X(cpu->opcode)]] ? 2 : 4;
						break;

					default:
						log_err("Unknown instruction (%X)", cpu->opcode);
						cpu->pc += 2;
						break;
				}
				break;

			case 0xF000:
				switch(cpu->opcode & 0x00FF) {
					case 0x0007: //LD vx, dt
						cpu->V[X(cpu->opcode)] = cpu->delay_timer;
						cpu->pc += 2;
						break;

					case 0x000A: //LD vx, K
						cpu->wait_for_key = true;
						cpu->key_location = X(cpu->opcode);
						cpu->pc += 2;
						break;

					case 0x0015: //FX15: LD DT, vx
						cpu->delay_timer = cpu->V[X(cpu->opcode)];
						cpu->pc += 2;
						break;

					case 0x0018: //FX18: LD ST, vx
						cpu->sound_timer = cpu->V[X(cpu->opcode)];
						cpu->pc += 2;
						break;

					case 0x01E: //FX1E: ADD I, vx
						//set I = I + VX
						cpu->V[0xF] = 0;

						if((cpu->I + cpu->V[X(cpu->opcode)]) > 4095) {
							cpu->V[0xF] = 1;
						}

						cpu->I += cpu->V[X(cpu->opcode)];
						cpu->pc += 2;
						break;

					case 0x0029: //FX29: LD F, vx
						cpu->I = cpu->V[X(cpu->opcode)] * 5;
						cpu->pc += 2;
						break;

					case 0x0033: //FX33: LD B, vx
						cpu->memory[cpu->I] = cpu->V[X(cpu->opcode)] / 100;
						cpu->memory[cpu->I + 1] = (cpu->V[X(cpu->opcode)] % 100) / 10;
						cpu->memory[cpu->I + 2] = cpu->V[X(cpu->opcode)] % 10;

						cpu->pc += 2;
						break;

					case 0x0055: //LD [I], vx
						for(int i = 0; i <= X(cpu->opcode); i++) {
							cpu->memory[cpu->I + i] = cpu->V[i];
						}
						cpu->pc += 2;
						break;

					case 0x0065: //Fx65 LD vx, [I]
						for (int i = 0; i <= X(cpu->opcode); i++) {
							cpu->V[i] = cpu->memory[cpu->I + i];
						}
						cpu->pc += 2;
						break;

					default:
						log_err("Unknown instruction (%X)", cpu->opcode);
						cpu->pc += 2;
						break;
				}
				break;

			default:
				log_err("Unknown opcode 0x%X", cpu->opcode);
				cpu->pc += 2;

		}
		
}

void clear_screen(chip8 * cpu) {
	memset(cpu->gfx, 0, 64 * 32);

	cpu->render_flag = true;
}

#endif