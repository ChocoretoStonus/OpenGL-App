#include <stdio.h>
#include <sdl/SDL.h>
#include <string>


using namespace std;

const int Ancho_Pantalla = 640;
const int Largo_Pantalla = 480;


enum TeclaCambioSuperficie
{
	Tecla_Cambio_Default,

	Tecla_Cambio_Izquierda,
	Tecla_Cambio_Derecha,
	Tecla_Cambio_Total
};


SDL_Window* ventana = NULL;
SDL_Surface* pantallaSuperficie = NULL;
SDL_Surface* ImagenCargada = NULL;
SDL_Surface* gTeclaCambioSuperficie[Tecla_Cambio_Total];
SDL_Surface* SuperficieActual = NULL;
SDL_Surface* cargaSuperficie (string path);



bool inicio() {
	bool suceso = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL no puede inicializarse: %s\n", SDL_GetError());
	}

	else
	{
		ventana = SDL_CreateWindow("SDL IMAGEN BMP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Ancho_Pantalla, Largo_Pantalla
			, SDL_WINDOW_OPENGL);
		if (ventana == NULL)
		{
			printf("La ventana no puede ser creada: %s\n", SDL_GetError());
		}
		else
		{
			pantallaSuperficie = SDL_GetWindowSurface(ventana);
		}
		SDL_ShowCursor(0);
		return suceso;
	}
}


bool cargar() {
	bool suceso = true;
	ImagenCargada = SDL_LoadBMP("img/T .T.bmp");
	if (ImagenCargada == NULL)
	{
		printf("No se puede cargar la imagen BMP: %s\n de la carpeta img/T .T.bmp", SDL_GetError());
		suceso = false;
	}
	return suceso;
}

void cerrar() {
	SDL_FreeSurface(ImagenCargada);
	ImagenCargada = NULL;

	SDL_DestroyWindow(ventana);
	ventana = NULL;

	pantallaSuperficie = NULL;

	SDL_Quit();
}


	
int main(int arg, char** argv) {
	if (!inicio())
	{
		printf("Fallo al iniciar!!\n");
	}
	else
	{
		if (!cargar())
		{
			printf("Fallo al cargar la imagen!!\n");
		}
		else
		{
			SDL_BlitSurface(ImagenCargada, NULL, pantallaSuperficie, NULL);
			SDL_UpdateWindowSurface(ventana);

			SDL_Delay(5000);
		}
	}

	cerrar();
	return 0;
}


