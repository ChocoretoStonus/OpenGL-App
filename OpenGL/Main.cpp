#include <stdio.h>
#include <sdl/SDL.h>

int main(int arg, char** argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED,
						 SDL_WINDOWPOS_CENTERED,800,600,0);
	SDL_Delay(6000);
	printf("La ventana no puede ser creada %\n", SDL_GetError());
	return 0;
}