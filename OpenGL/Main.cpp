#include <stdio.h>
#include <sdl/SDL.h>
#include <string>


using namespace std;

const int Ancho_Pantalla = 640;
const int Largo_Pantalla = 480;


SDL_Window* ventana = NULL;
SDL_Surface* pantallaSuperficie = NULL;
SDL_Surface* SuperficieAjustada = NULL;
SDL_Surface* ImagenCargada = NULL;
SDL_Surface* SuperficieActual = NULL;
SDL_Surface* cargaSuperficie (string path);



bool inicio() {
	bool suceso = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL no puede inicializarse: %s\n", SDL_GetError());
	}

	else
	{
		ventana = SDL_CreateWindow("SDL Ajuste Imagen", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Ancho_Pantalla, Largo_Pantalla
			, SDL_WINDOW_SHOWN);
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

	SuperficieAjustada = cargaSuperficie("img/right.bmp");
	
	if (SuperficieAjustada == NULL)
	{
		printf("Es imposible cargar la imgane hagalo bien");
		suceso = false;
	}

	return suceso;
}


void cerrar() {
	SDL_FreeSurface(SuperficieAjustada);
	SuperficieAjustada = NULL;	

	SDL_DestroyWindow(ventana);
	ventana = NULL;
	SuperficieAjustada = NULL;	

	SDL_Quit();
}


SDL_Surface* cargaSuperficie(string path) {
	
	SDL_Surface* superficieOptimizada = NULL;
	SDL_Surface* superficieCargada = SDL_LoadBMP(path.c_str());
	if (superficieCargada == NULL)
	{
		printf("Imposible cargar la imagen %s! SDL ERROR: %s\n", 
			path.c_str(), SDL_GetError());
	}
	else
	{
		superficieOptimizada = SDL_ConvertSurface(superficieCargada,
							   pantallaSuperficie->format,0);
	
		if (superficieOptimizada==NULL)
		{
			printf("Imposible cargar la imagen %s! SDL ERROR: %s\n",
				path.c_str(), SDL_GetError());
		}
		SDL_FreeSurface(superficieOptimizada);
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
	i9o0	}
		else
		{

			bool salir = false;
			SDL_Event e;

			while (!salir)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						salir = true;
					}
				}
					SDL_Rect rectaAjustada;
					rectaAjustada.x = 0;
					rectaAjustada.y = 0;
					rectaAjustada.w = Ancho_Pantalla;
					rectaAjustada.h = Largo_Pantalla;

					SDL_BlitScaled(SuperficieAjustada,NULL, 
									pantallaSuperficie, &rectaAjustada);
					SDL_UpdateWindowSurface(ventana);
			
			}
		}
	}

	cerrar();
	return 0;
}


