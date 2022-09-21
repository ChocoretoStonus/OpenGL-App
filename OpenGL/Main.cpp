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
	void render(int x, int y);
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
LTextura gImagenTextura;
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


void LTextura::render(int x, int y) {
	SDL_Rect renderCuadrado = { x,y,mAncho,mAlto };
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

	if (!gFondoTextura.cargaArchivo("img/fondo.png"))
	{
		printf("Es imposible cargar la imgane hagalo bien");
		suceso = false;
	}

	if (!gImagenTextura.cargaArchivo("img/mono.png"))
	{
		printf("Es imposible cargar la imgane hagalo bien");
		suceso = false;
	}

	return suceso;
}


void cerrar() {

	//SDL_DestroyTexture(gTexura);
	//SDL_DestroyRenderer(gRenderizado);
	gImagenTextura.liberar();
	gFondoTextura.liberar();
	SDL_DestroyRenderer(gRenderizado);
	SDL_DestroyWindow(ventana);

	ventana = NULL;
	gRenderizado = NULL;
	//gTexura = NULL;

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

			while (!salir)
			{
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_ESCAPE:
							salir = true;
							break;

						default:
							salir = true;
							break;
						}
					}
				}

				SDL_RenderClear(gRenderizado);

				gFondoTextura.render(0, 0);

				gImagenTextura.render(320, 240);
				gImagenTextura.render(450, 240);
				gImagenTextura.render(220, 240);



				SDL_Rect llenarRectangulo = { 0,Largo_Pantalla - 50,Ancho_Pantalla,Largo_Pantalla };

				SDL_SetRenderDrawColor(gRenderizado, 255, 0, 0, 0xFF);
				SDL_RenderFillRect(gRenderizado, &llenarRectangulo);


				SDL_Rect controlRectangulo = { Ancho_Pantalla / 6,Largo_Pantalla / 6,Ancho_Pantalla * 2 / 3,Largo_Pantalla * 2 / 3 };
				SDL_SetRenderDrawColor(gRenderizado, 0xFF, 0xFF, 0xFF, 0xFF);

				SDL_SetRenderDrawColor(gRenderizado, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderDrawRect(gRenderizado, &controlRectangulo);

				SDL_SetRenderDrawColor(gRenderizado, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderDrawLine(gRenderizado, 0, Largo_Pantalla / 2, Ancho_Pantalla, Largo_Pantalla / 2);

				SDL_SetRenderDrawColor(gRenderizado, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderDrawLine(gRenderizado, 0, 0, Ancho_Pantalla, Largo_Pantalla);

				SDL_SetRenderDrawColor(gRenderizado, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderDrawLine(gRenderizado, 0, Largo_Pantalla, Ancho_Pantalla, 0);



				for (int i = 0; i < Largo_Pantalla; i += 4)
				{
					SDL_RenderDrawPoint(gRenderizado, Ancho_Pantalla / 2, i);
				}

				SDL_RenderPresent(gRenderizado);
			}
		}
	}

	cerrar();
	return 0;
}


