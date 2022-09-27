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
//LTextura gImagenTextura;
//LTextura gFondoTextura;

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
	SDL_RenderCopy(gRenderizado, mTextura, NULL, &renderCuadrado);
}


int LTextura::obtenerAncho() {
	return mAncho;
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
			gRenderizado = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
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

	
	if (!gModularTextura.cargaArchivo("img/colors.png"))
	{
		printf("Es imposible cargar la imgane hagalo bien");
		suceso = false;
	}

	return suceso;
}


void cerrar() {

	gModularTextura.liberar();
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
			Uint8 r = 255;
			Uint8 g = 255;
			Uint8 b = 255;

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
						case SDLK_q:
							r += 32;
							break;
						case SDLK_w:
							g += 32;
							break;
						case SDLK_e:
							b += 32;
							break;
						case SDLK_a:
							r += 32;
							break;
						case SDLK_s:
							g += 32;
							break;
						case SDLK_d:
							b += 32;
							break;
						}
					}
					
				}

				SDL_SetRenderDrawColor(gRenderizado, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderizado);

				gModularTextura.asignacolor(r,g,b);
				gModularTextura.render(0, 0);

				SDL_RenderPresent(gRenderizado);
			}
		}
	}

	cerrar();
	return 0;
}


