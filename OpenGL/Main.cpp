#pragma region Librerias


	#include <sdl/SDL.h>
	#include <sdl/SDL_image.h>
	#include <sdl/SDL_ttf.h>
	#include <sdl/SDL_mixer.h>
	#include <stdio.h>
	#include <string>
	#include <sstream>


#pragma endregion


#pragma region Clases


	using namespace std;

	const int Ancho_Pantalla = 800;
	const int Largo_Pantalla = 600;

	class LTextura {
	public:
		LTextura();
		~LTextura();
		bool cargaArchivo(string path);
		bool cargaRenderizadoTexto(string texuraTexto,SDL_Color textoColor);
		void liberar();
		void render(int x, int y, SDL_Rect* clip = NULL);
		int obtenerAncho();
		int obtenerLargo();
	private:
		SDL_Texture* mTextura;
		int mAncho;
		int mLargo;
	};


	class LTiempo
{
public:
	LTiempo();
	void iniciar();
	void detener();
	void pausar();
	void continuar();
	Uint32 obtenerEstado();
	bool enInicio();
	bool enPausa();
private:
	Uint32 mInicioEstado;
	Uint32 mPausaEstado;
	bool mPausa;
	bool mInicio;

};


#pragma endregion


#pragma region Variables

	bool inicio();
	bool cargar();
	void cerrar();

	SDL_Window* ventana = NULL;
	SDL_Renderer* gRenderizado = NULL;
	LTextura gInicioTextura;

	TTF_Font* gFuente = NULL;
	LTextura gTiempoTextoTextura;
	LTextura gRapidoTextoTextura;
	LTextura gPausaRapidoTextura;
	LTextura gInicioRapidoTextura;

	LTextura gFondoTextura;

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


		bool LTextura::cargaRenderizadoTexto(string textoTextura, SDL_Color textoColor) {

			liberar();
			SDL_Surface* textoSuperficie = TTF_RenderText_Solid(gFuente,textoTextura.c_str(),textoColor);


			if (textoSuperficie==NULL)
			{
				printf("No jala este pedo");
			}
			else
			{
				mTextura = SDL_CreateTextureFromSurface(gRenderizado,textoSuperficie);
				if (mTextura ==NULL)
				{
					printf("No jalo el pedo este");
				}
				else
				{
					mAncho = textoSuperficie->w;
					mLargo = textoSuperficie->h;
				}
				SDL_FreeSurface(textoSuperficie);
			}
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



	#pragma region LTiempo


		LTiempo::LTiempo()
		{
			mInicioEstado = 0;
			mPausaEstado = 0;

			mPausa = false;
			mInicio = false;

		}


		void LTiempo::iniciar()
		{
			mInicio = true;
			mPausa = false;

			mInicioEstado = SDL_GetTicks();
			mPausaEstado = 0;
		}


		void LTiempo::detener() {
			mInicio = false;
			mPausa = false;

			mInicioEstado = 0;
			mPausaEstado = 0;
		}


		void LTiempo::pausar() {
			if (mInicio && !mPausa)
			{
				mPausa = true;
			
				mPausaEstado = SDL_GetTicks() - mInicioEstado;
				mInicioEstado = 0;
			}
		}


		void LTiempo::continuar() {
			if (mInicio && mPausa)
			{
				mPausa = false;

				mInicioEstado = SDL_GetTicks() - mPausaEstado;
				mPausaEstado = 0;
			}
		}


		Uint32 LTiempo::obtenerEstado() {
			Uint32 tiempo = 0;
			if (mInicio)
			{
				if (mPausa)
				{
					tiempo = mPausaEstado;
				}
				else
				{
					tiempo = SDL_GetTicks() - mInicioEstado;
				}
			}
			return tiempo;
		}


		bool LTiempo::enInicio() {
			return mInicio;
		}


		bool LTiempo::enPausa() {
			return mPausa && mInicio;
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
					if (TTF_Init() == -1)
					{
						printf("no jala la fuente");
						suceso = false;
					}
				}
			}
		}
			return suceso;
	}


	bool cargar() {


		bool suceso = true;

		gFuente = TTF_OpenFont("fonts/ComicSans.ttf",18);

		if (gFuente ==  NULL)
		{
			printf("Es imposible cargar");
			suceso = false;
		}
		else
		{
			SDL_Color textoColor = {0,0,0,255};

			if (!gInicioRapidoTextura.cargaRenderizadoTexto("Presiona S para inicio/detener",textoColor))
			{
				printf("No se ");
				suceso = false;
			}
			
			if (!gPausaRapidoTextura.cargaRenderizadoTexto("pucha P para pausar o continuar", textoColor))
			{
				printf("No se ");
				suceso = false;
			}

		}
	
		return suceso;

	}


	void cerrar() {

		gTiempoTextoTextura.liberar();
		gRapidoTextoTextura.liberar();
		gInicioRapidoTextura.liberar();
		gPausaRapidoTextura.liberar();

		TTF_CloseFont(gFuente);
		gFuente = NULL;

		SDL_DestroyRenderer(gRenderizado);
		SDL_DestroyWindow(ventana);

		ventana = NULL;
		gRenderizado = NULL;
		IMG_Quit();
		SDL_Quit();
		TTF_Quit();
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
				SDL_Color textoColor = {0,0,0,255};
				LTiempo tiempo;
				stringstream tiempoTexto;
				LTextura* actualTextura = NULL;
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
							if (e.key.keysym.sym == SDLK_s)
							{
								if (tiempo.enInicio())
								{
									tiempo.detener();

								}
								else
								{
									tiempo.iniciar();
								}
							}
							else if (e.key.keysym.sym == SDLK_p)
							{
								if (tiempo.enPausa())
								{
									tiempo.continuar();
								}
								else
								{
									tiempo.pausar();
								}
							}
						}

					}
					tiempoTexto.str("");
					tiempoTexto<<"Tiempo en miliosegundos"<<(tiempo.obtenerEstado()/1000.f);

					if (!gTiempoTextoTextura.cargaRenderizadoTexto(tiempoTexto.str().c_str(),textoColor))
					{
						printf("no jalo este pedo en el tiempo");
					}

					SDL_SetRenderDrawColor(gRenderizado, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderizado);
					//gInicioTextura.render(0,0);
					
					gInicioRapidoTextura.render((Largo_Pantalla - gInicioRapidoTextura.obtenerLargo()) / 2,
						0);
					
					gPausaRapidoTextura.render((Largo_Pantalla - gPausaRapidoTextura.obtenerLargo())/2,
						gInicioRapidoTextura.obtenerAncho());

					gTiempoTextoTextura.render((Ancho_Pantalla - gRapidoTextoTextura.obtenerAncho())/2,
						(Largo_Pantalla - gRapidoTextoTextura.obtenerLargo())/2);

					SDL_RenderPresent(gRenderizado);

				}
			}
		}

		cerrar();
		return 0;
	}



#pragma endregion
