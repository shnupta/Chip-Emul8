#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include <SDL2/SDL.h>

#define SCREEN_W 640
#define SCREEN_H 320
#define W 64
#define H 32

void draw(SDL_Renderer * renderer, uint32_t ms, chip8 * cpu);

chip8 cpu;

int main(int argc, char const *argv[])
{

	initialise_cpu(&cpu);
	load_rom(&cpu, "PONG");

	// TODO: implement graphics and input
	SDL_Window *window;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow (
		"Chip-Emul8",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_W,
		SCREEN_H,
		SDL_WINDOW_SHOWN
	);

	SDL_Renderer *renderer;
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	uint32_t ms = 15;


	
	for(;;) {
		emulate_cycle(&cpu);

		if(cpu.draw_flag) {
			draw(renderer, ms, &cpu);
		}
	}

	SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}


void draw_sprite(SDL_Renderer *renderer, SDL_Rect r) {
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &r);
}

void draw(SDL_Renderer *renderer, uint32_t ms, chip8 * cpu)
{
    SDL_Rect r;

    /* clear window */
    SDL_RenderClear(renderer);

    for (int i = 0; i < SCREEN_H; i++) {
        for (int j = 0; j < SCREEN_W; j++){
            if (cpu->gfx[(j/10) + (i/10) * 64] >= 1) {
                r.y = i;
                r.x = j;
                r.w = 1;
                r.h = 1;
                draw_sprite(renderer, r);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderPresent(renderer);

    SDL_Delay(ms);
}


//TODO: add graphics, input and window functions

//FINAL TODO: add SDL_mixer.h for sounds???
