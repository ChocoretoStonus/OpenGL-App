#pragma region Librerias


	#include <sdl/SDL.h>
	#include <sdl/SDL_image.h>
	#include <stdio.h>
	#include <string>


#pragma endregion


#pragma region Clases


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
		int obtenerLargo();
	private:
		SDL_Texture* mTextura;
		int mAncho;
		int mLargo;
	};

	class Punto
	{
	public:
		static const int PUNTO_ANCHO = 20;
		static const int PUNTO_LARGO = 20;
		static const int PUNTO_VEL = 10;
		Punto();
		void manejarEvento(SDL_Event& e);
		void mover();
		void render();

	private:
		int mPosX, mPosY;
		int mVelX, mVelY;
	};



#pragma endregion


#pragma region Variables

	bool inicio();
	bool cargar();
	void cerrar();

	SDL_Window* ventana = NULL;
	SDL_Renderer* gRenderizado = NULL;
	LTextura gPuntoTextura;


#pragma endregion


#pragma region Texture Metodos


	#pragma region LTextura


		LTextura::LTextura() {
			mTextura = NULL;
			mAncho = 0;
			mLargo = 0;
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
					mLargo = cargaSuperficie->h;
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
				mLargo = 0;
			}
		}


		void LTextura::render(int x, int y, SDL_Rect* clip) {
			SDL_Rect renderCuadrado = { x,y,mAncho,mLargo };
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


		int LTextura::obtenerLargo() {
			return mLargo;
		}


	#pragma endregion


	#pragma region Punto

		Punto::Punto()
		{
			mPosX = 0;
			mPosY = 0;
			mVelX = 0;
			mVelY = 0;
		}


		void Punto::manejarEvento(SDL_Event& e) {
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					mVelY -= PUNTO_VEL;
					break;
				case SDLK_DOWN:
					mVelY += PUNTO_VEL;
					break;
				case SDLK_RIGHT:
					mVelX += PUNTO_VEL;
					break;
				case SDLK_LEFT:
					mVelX -= PUNTO_VEL;
					break;
				}
			}
			else if (e.type == SDL_KEYUP && e.key.repeat == 0)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP: mVelY += PUNTO_VEL; break;
				case SDLK_DOWN: mVelY -= PUNTO_VEL; break;
				case SDLK_RIGHT: mVelX -= PUNTO_VEL; break;
				case SDLK_LEFT: mVelX += PUNTO_VEL; break;
				}
			}
		}


		void Punto::mover() {
			mPosX += mVelX;

			if ((mPosX < 0) || (mPosX + PUNTO_ANCHO > Ancho_Pantalla))
			{
				mPosX -= mVelX;
			}
			mPosY += mVelY;

			if ((mPosY < 0) || (mPosY + PUNTO_LARGO > Largo_Pantalla))
			{
				mPosY -= mVelY;
			}
		}


		void Punto::render() {
			gPuntoTextura.render(mPosX,mPosY);
		}


	#pragma endregion


#pragma endregion 


#pragma region Metodos Main


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
				}
			}
		}
			return suceso;
	}


	bool cargar() {


		bool suceso = true;

		if (!gPuntoTextura.cargaArchivo("img/punto.bmp"))
		{
			printf("no sirvio la img"); suceso = false;
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
				Punto punto;
				while (!salir)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						if (e.type == SDL_QUIT)
						{
							salir = true;
						}
						punto.manejarEvento(e);
					}
					punto.mover();

					SDL_SetRenderDrawColor(gRenderizado, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderizado);

					punto.render();
					
					SDL_RenderPresent(gRenderizado);

				}
			}
		}

		cerrar();
		return 0;
	}


#pragma endregion
