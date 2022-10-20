#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <sdl/SDL_ttf.h>
#include <sdl/SDL_mixer.h>
#include <stdio.h>
#include <string>

using namespace std;

const int Ancho_Pantalla = 640;
const int Largo_Pantalla = 480;

class LTextura {
public:
	LTextura();
	~LTextura();
	bool cargaArchivo(string path);
	void liberar();
	void render(int x, int y, SDL_Rect* clip = NULL);
	int obtenerAncho();
	int obtenerAlto();
private:
	SDL_Texture* mTextura;
	int mAncho;
	int mAlto;
};


#pragma region Variables

bool inicio();
bool cargar();
void cerrar();

SDL_Surface* gTeclaCambioSuperficie;
SDL_Window* ventana = NULL;
SDL_Renderer* gRenderizado = NULL;
LTextura gInicioTextura;

Mix_Music* gMusica = NULL;
Mix_Chunk* gEfecto = NULL;
Mix_Chunk* gAlto = NULL;
Mix_Chunk* gMedio = NULL;
Mix_Chunk* gBajo = NULL;

LTextura gFondoTextura;

#pragma endregion


#pragma region Texture Metodos


LTextura::LTextura() {
	mTextura = NULL;
	mAncho = 0;
	mAlto = 0;
}


LTextura::~LTextura() {
	liberar();
}


bool LTextura::cargaArchivo(string path) {
	liberar();
	SDL_Texture* nuevaTextura = NULL;
	SDL_Surface* cargaSuperficie = IMG_Load(path.c_str());
	if (cargaSuperficie == NULL)
	{
		printf("No se puede cargar la imagen %s! SDL_image Error: %s\n", IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(cargaSuperficie, SDL_TRUE, SDL_MapRGB(cargaSuperficie->format, 0, 0xFF, 0xFF));
		nuevaTextura = SDL_CreateTextureFromSurface(gRenderizado, cargaSuperficie);
		if (nuevaTextura == NULL)
		{
			printf("No se puede crear la textura, SDL_image Error: %s\n", IMG_GetError());
		}
		else
		{
			mAncho = cargaSuperficie->w;
			mAlto = cargaSuperficie->h;
		}
		SDL_FreeSurface(cargaSuperficie);

	}

	mTextura = nuevaTextura;
	return mTextura != NULL;
}


void LTextura::liberar() {
	if (mTextura != NULL)
	{
		SDL_DestroyTexture(mTextura);
		mTextura = NULL;
		mAncho = 0;
		mAlto = 0;
	}
}


void LTextura::render(int x, int y, SDL_Rect* clip) {
	SDL_Rect renderCuadrado = { x,y,mAncho,mAlto };
	if (clip != NULL)
	{
		renderCuadrado.w = clip->w;
			renderCuadrado.h = clip->h;
	}
	SDL_RenderCopy(gRenderizado, mTextura, clip, &renderCuadrado);
}


int LTextura::obtenerAncho() {
	return mAncho;
}


int LTextura::obtenerAlto() {
	return mAlto;
}



#pragma endregion 


bool inicio() {
	bool suceso = true;

	//Si quieres cargar todos los elementos para evitar tanto codigo usa 
	//SDL_INIT_EVERYTHING
		
	if (SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO) < 0) {
		printf("SDL no puede inicializarse: %s\n", SDL_GetError());
	}
	
	else
	{

		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: El filtrado de textura no esta habilitado");
		}


		ventana = SDL_CreateWindow("SDL_Evaluacion_Parcial: Jose_Carlos_Garcia_Leon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Ancho_Pantalla, Largo_Pantalla
			, SDL_WINDOW_SHOWN);
		if (ventana == NULL)
		{
			printf("La ventana no puede ser creada: %s\n", SDL_GetError());
		}
		else
		{
			gRenderizado = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderizado == NULL)
			{
				printf("El renderizado no puede ser creado, SDL ERROR: %s\n", SDL_GetError());
				suceso = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderizado,0xFF, 0xFF, 0xFF, 0xFF);
				int imagenBandera = IMG_INIT_PNG;

				if (!(IMG_Init(imagenBandera) & imagenBandera))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n",
						IMG_GetError());
					suceso = false;
				}
				if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) <0)
				{
					printf("no jalo el audio");
					suceso = false;
				}
			}
		}
	}
		return suceso;
}


bool cargar() {


	bool suceso = true;

	if (!gInicioTextura.cargaArchivo("au/inicio.png"))
	{
		printf("Es imposible cargar la img mi capo");
		suceso = false;
	}
	
	gMusica = Mix_LoadMUS("au/phony.wav");
	if (gMusica ==NULL)
	{
		printf("Es imposible cargar la img mi capo");
		suceso = false;
	}

	gEfecto = Mix_LoadWAV("au/raspon.wav");
	if (gEfecto == NULL)
	{
		printf("Es imposible cargar la img mi capo");
		suceso = false;
	}
	
	gAlto = Mix_LoadWAV("au/alto.wav");
	if (gAlto == NULL)
	{
		printf("Es imposible cargar la img mi capo");
		suceso = false;
	}

	gMedio = Mix_LoadWAV("au/medio.wav");
	if (gMedio == NULL)
	{
		printf("Es imposible cargar la img mi capo");
		suceso = false;
	}

	gBajo = Mix_LoadWAV("au/bajo.wav");
	if (gBajo == NULL)
	{
		printf("Es imposible cargar la img mi capo");
		suceso = false;
	}
		return suceso;

}


void cerrar() {

	SDL_DestroyRenderer(gRenderizado);
	SDL_DestroyWindow(ventana);

	ventana = NULL;
	gRenderizado = NULL;
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
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
			int vol= 0;
			LTextura* actualTextura = NULL;
			while (!salir)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						salir = true;
					}
					else if(e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_1:Mix_PlayChannel(-1,gAlto,0);
							break;
						case SDLK_2:Mix_PlayChannel(-1, gMedio, 0);
							break;
						case SDLK_3:Mix_PlayChannel(-1, gBajo, 0);
							break;
						case SDLK_4:Mix_PlayChannel(-1, gEfecto, 0);
							break;
						case SDLK_5:
							if (Mix_PlayingMusic()==0) {
								Mix_PlayMusic(gMusica,-1);
							}
							else
							{
								if (Mix_PausedMusic()==1)
								{
									Mix_ResumeMusic();
								}
								else
								{
									Mix_PauseMusic();
								}
							}

							break;
						case SDLK_7:
							vol+=10;
							Mix_Volume(-1,vol);
							break;
						case SDLK_8:
							vol-=10;
							Mix_Volume(-1, vol);
							break;

						case SDLK_6:Mix_HaltMusic();
							break;
						}
					}

				}

				SDL_SetRenderDrawColor(gRenderizado, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderizado);

				
				gInicioTextura.render(0,0);

				SDL_RenderPresent(gRenderizado);

			}
		}
	}

	cerrar();
	return 0;
}


