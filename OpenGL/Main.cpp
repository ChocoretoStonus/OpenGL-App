#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <sdl/SDL_ttf.h>
#include <stdio.h>
#include <string>


using namespace std;

const int Ancho_Pantalla = 640;
const int Largo_Pantalla = 480;

const int Boton_Ancho = 300;
const int Boton_Largo = 200;
const int Total_Botones = 4;

enum  LBotonSprite
{
	BOTON_SPRITE_MOUSE_SALIDA = 0,
	BOTON_SPRITE_MOUSE_SOBRE_MOVIMIENTO = 1,
	BOTON_SPRITE_MOUSE_ABAJO = 2,
	BOTON_SPRITE_MOUSE_ARRIBA = 3,
	BOTON_SPRITE_TOTAL = 4
};

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

class LBoton
{
public:
	LBoton();
	void asignaPosicion(int x, int y);
	void controlaEvento(SDL_Event* e);
	void render();
private:
	SDL_Point mPosicion;
	LBotonSprite mActualSprite;
};


bool inicio();
bool cargar();
void cerrar();

SDL_Surface* gTeclaCambioSuperficie;
SDL_Window* ventana = NULL;
SDL_Renderer* gRenderizado = NULL;
LTextura gBotonSpriteTextura;
SDL_Rect gSpriteClips[BOTON_SPRITE_TOTAL];

LBoton gBotones[Total_Botones];
LTextura gFondoTextura;


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


LBoton::LBoton()
{
	mPosicion.x = 0;
	mPosicion.y = 0;
	mActualSprite = BOTON_SPRITE_MOUSE_SALIDA;
}


void LBoton::asignaPosicion(int x, int y)
{
	mPosicion.x = x;
	mPosicion.y = y;
}


void LBoton::controlaEvento(SDL_Event* e) {
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		SDL_GetMouseState(&x,&y);
		bool dentro = true;
		if (x <mPosicion.x)
		{
			dentro = true;
		}
		else if(x>mPosicion.x)
		{
			dentro = false;
		}
		else if (y < mPosicion.y)
		{
			dentro = false;
		}
		else if (y > mPosicion.y + Boton_Largo)
		{
			dentro = false;
		}
		if (!dentro)
		{
			mActualSprite = BOTON_SPRITE_MOUSE_SALIDA;
		}
	}
	else
	{
		switch (e->type)
		{
		case SDL_MOUSEMOTION:
			mActualSprite = BOTON_SPRITE_MOUSE_SOBRE_MOVIMIENTO;
			break;
		case SDL_MOUSEBUTTONDOWN:
			mActualSprite = BOTON_SPRITE_MOUSE_ABAJO;
			break;
		case SDL_MOUSEBUTTONUP:
			mActualSprite = BOTON_SPRITE_MOUSE_ARRIBA;
			break;
		}
	}
}


void LBoton::render() {
	gBotonSpriteTextura.render(mPosicion.x,mPosicion.y,&gSpriteClips[mActualSprite]);
}


bool inicio() {
	bool suceso = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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
				if (TTF_Init() ==-1)
				{
					suceso = false;
					printf("no jalo el ttf");
				}
			}
		}
		return suceso;
	}
}


bool cargar() {


	bool suceso = true;

	if (!gBotonSpriteTextura.cargaArchivo("img/boton.png"))
	{
		printf("Es imposible cargar la img mi capo");
		suceso = false;
	}
	else
	{

		for (int i = 0; i < BOTON_SPRITE_TOTAL; ++i)
		{
			gSpriteClips[i].x = 0;
			gSpriteClips[i].y = i * 200;
			gSpriteClips[i].w = Boton_Ancho;
			gSpriteClips[i].h = Boton_Largo;
		}
		gBotones[0].asignaPosicion(0,0);
		gBotones[1].asignaPosicion(Ancho_Pantalla-Boton_Ancho,0);
		gBotones[2].asignaPosicion(0,Largo_Pantalla-Boton_Largo);
		gBotones[3].asignaPosicion(Ancho_Pantalla - Boton_Ancho, 
								   Largo_Pantalla - Boton_Largo);

	}
		return suceso;

}


void cerrar() {
	gBotonSpriteTextura.liberar();
	SDL_DestroyRenderer(gRenderizado);
	SDL_DestroyWindow(ventana);

	ventana = NULL;
	gRenderizado = NULL;
	IMG_Quit();
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

			bool salir = false;
			SDL_Event e;
			int frame = 0;
			while (!salir)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						salir = true;
					}

					for (int i = 0; i < Total_Botones; ++i)
					{
						gBotones[i].controlaEvento(&e);
					}
					
				}

				SDL_SetRenderDrawColor(gRenderizado, 0xFF, 0, 0xFF, 0xFF);
				SDL_RenderClear(gRenderizado);
				
				for (int i = 0; i < Total_Botones; ++i)
				{
					gBotones[i].render();
				}

				SDL_RenderPresent(gRenderizado);
				
			}
		}
	}

	cerrar();
	return 0;
}


