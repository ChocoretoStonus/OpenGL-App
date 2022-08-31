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

	gTeclaCambioSuperficie[Tecla_Cambio_Default] = cargaSuperficie("img/T .T.bmp");
	if (gTeclaCambioSuperficie[Tecla_Cambio_Default] == NULL)
	{
		printf("No se puede cargar la Default!!\n");
		suceso = false;
	}

	gTeclaCambioSuperficie[Tecla_Cambio_Izquierda] = cargaSuperficie("img/left.bmp");
	if (gTeclaCambioSuperficie[Tecla_Cambio_Izquierda] == NULL)
	{
		printf("No se puede cargar la Izquierda!!\n");
		suceso = false;
	}

	gTeclaCambioSuperficie[Tecla_Cambio_Derecha] = cargaSuperficie("img/right.bmp");
	if (gTeclaCambioSuperficie[Tecla_Cambio_Derecha] == NULL)
	{
		printf("No se puede cargar la Derecha!!\n");
		suceso = false;
	}
	return suceso;
}


void cerrar() {
	for (int i = 0; i < Tecla_Cambio_Total; i++)
	{
		SDL_FreeSurface(gTeclaCambioSuperficie[i]);
		gTeclaCambioSuperficie[i] = NULL;
	}

	SDL_DestroyWindow(ventana);
	ventana = NULL;

	SDL_Quit();
}


SDL_Surface* cargaSuperficie(string path) {
	SDL_Surface* superficieCargada = SDL_LoadBMP(path.c_str());
	if (superficieCargada == NULL)
	{
		printf("Imposible cargar la imagen %s! SDL ERROR: %s\n", 
			path.c_str(), SDL_GetError());
	}
	return superficieCargada;
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

			bool salir = false;
			SDL_Event e;
			SuperficieActual = gTeclaCambioSuperficie[Tecla_Cambio_Default];

			while (!salir)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						salir = true;
					}
					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
							case SDLK_LEFT:
								SuperficieActual = gTeclaCambioSuperficie[Tecla_Cambio_Izquierda];
								break;

							case SDLK_RIGHT:
								SuperficieActual = gTeclaCambioSuperficie[Tecla_Cambio_Derecha];
								break;

							default:
								SuperficieActual = gTeclaCambioSuperficie[Tecla_Cambio_Default];
								break;
						}
						SDL_BlitSurface(SuperficieActual, NULL, pantallaSuperficie,NULL);
						SDL_UpdateWindowSurface(ventana);
					}
				}
			}
		}
	}

	cerrar();
	return 0;
}


