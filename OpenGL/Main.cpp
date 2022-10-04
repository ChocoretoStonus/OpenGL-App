#include <stdio.h>
#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include <string>


using namespace std;

const int Ancho_Pantalla = 640;
const int Largo_Pantalla = 480;
SDL_Surface* gTeclaCambioSuperficie;

class LTextura {
public:
	LTextura();
	~LTextura();
	bool cargaArchivo(string path);
	void liberar();
	void asignacolor(Uint8 rojo, Uint8 verde, Uint8 azul);
	void asignaModoBlend(SDL_BlendMode mezcla);
	void asignaAlpha(Uint8 alpha);
	void render(int x, int y, SDL_Rect* clip = NULL);
	int obtenerAncho();
	int obtenerAlto();
private:
	SDL_Texture* mTextura;
	int mAncho;
	int mAlto;
};

bool inicio();
bool cargar();
void cerrar();
SDL_Window* ventana = NULL;
SDL_Renderer* gRenderizado = NULL;

LTextura gModularTextura;
LTextura gFondoTextura;
//LTextura gImagenTextura;

const int FRAMES_ANIMATION_CAMINAR = 4;
SDL_Rect gSpriteClips[FRAMES_ANIMATION_CAMINAR];
LTextura gSpriteTextura;

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


void LTextura::asignacolor(Uint8 rojo, Uint8 verde, Uint8 azul) {
	SDL_SetTextureColorMod(mTextura,rojo,verde,azul);
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

void LTextura::asignaModoBlend(SDL_BlendMode mezcla) {
	SDL_SetTextureBlendMode(mTextura,mezcla);
}

void LTextura::asignaAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(mTextura, alpha);
}

int LTextura::obtenerAlto() {
	return mAlto;
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
			}
		}
		return suceso;
	}
}


bool cargar() {


	bool suceso = true;

	
	if (!gSpriteTextura.cargaArchivo("img/caminar.png"))
	{
		printf("Es imposible cargar la imagen hagalo bien");
		suceso = false;
	}
	else
	{
		gSpriteClips[0].x = 0;
		gSpriteClips[0].y = 0;
		gSpriteClips[0].w = 64;
		gSpriteClips[0].h = 205;

		gSpriteClips[1].x = 64;
		gSpriteClips[1].y = 0;
		gSpriteClips[1].w = 64;
		gSpriteClips[1].h = 205;

		gSpriteClips[2].x = 128;
		gSpriteClips[2].y = 0;
		gSpriteClips[2].w = 64;
		gSpriteClips[2].h = 205;

		gSpriteClips[3].x = 196;
		gSpriteClips[3].y = 0;
		gSpriteClips[3].w = 64;
		gSpriteClips[3].h = 205;
	}

	return suceso;
}


void cerrar() {

	gModularTextura.liberar();
	gSpriteTextura.liberar();
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
					
				}

				SDL_SetRenderDrawColor(gRenderizado, 0xFF, 0, 0xFF, 0xFF);
				SDL_RenderClear(gRenderizado);
				
				SDL_Rect* actualClip = &gSpriteClips[frame/9];
				gSpriteTextura.render((Ancho_Pantalla - actualClip ->w)/2,
					(Largo_Pantalla-actualClip ->h)/2,actualClip);

				SDL_RenderPresent(gRenderizado);
				
				//printf("no: ", frame, "\n");
				frame++;
				if (frame/9 >=FRAMES_ANIMATION_CAMINAR)
				{
					frame = 0;
				}

			}
		}
	}

	cerrar();
	return 0;
}


