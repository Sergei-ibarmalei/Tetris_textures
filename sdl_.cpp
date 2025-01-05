
#include "sdl_.h"
#include <fstream>
#include "videoconst.h"


namespace tetris
{
	Sdl::Sdl()
	{
		const char* appName = "Tetris";


		if (SDL_Init(SDL_INIT_VIDEO) < 0 ||
			!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
#ifdef LOGS
			std::cerr << "Cannot initiate SDL, abort.\n";
#endif
			initOk = false;
			return;
		}

		window = SDL_CreateWindow(appName,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_W,
			SCREEN_H,
			SDL_WINDOW_SHOWN);
		if (!window)
		{
#ifdef LOGS
			std::cerr << "Cannot create window, abort.\n";
#endif
			initOk = false;
			return;
		}

		render = SDL_CreateRenderer(window,
			-1,
			SDL_RENDERER_ACCELERATED |
			SDL_RENDERER_PRESENTVSYNC);
		if (!render)
		{
#ifdef LOGS
			std::cerr << "Cannot create render, abort.\n";
#endif
			initOk = false;
			return;
		}

		int imageFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imageFlags) & imageFlags))
		{
#ifdef LOGS
			std::cerr << "SDL_image could not initialize, abort.\n";
#endif
			initOk = false;
			return;
		}

		//makeTexture();

	}

	Sdl::~Sdl()
	{
		if (render)
		{
			SDL_DestroyRenderer(render);
			render = nullptr;
		}
		if (window)
		{
			SDL_DestroyWindow(window);
			window = nullptr;
		}
		
		IMG_Quit();
		SDL_Quit();
	}

//	void Sdl::makeTexture()
//	{
//		
//		const char* dataTextureFileName = "data.data";
//		const auto dataTextureFileLength{ 1831 };
//		uint8_t buffer[dataTextureFileLength]{};
//
//		std::ifstream inFile(dataTextureFileName, std::ios::binary);
//		if (!inFile.is_open())
//		{
//#ifdef LOGS
//			std::cerr << "Cannot open data file, abort.\n";
//#endif
//			initOk = false;
//			return;
//		}
//
//		// decrypt data file:
//		char byte;
//		char key = 'K';
//		int index = 0;
//
//		while (inFile.get(byte))
//		{
//			buffer[index++] = static_cast<uint8_t>(byte ^ key);
//		}
//		// end decrypt data file
//
//		inFile.close();
//
//		// Create SDL_RWops from decrypt data:
//		SDL_RWops* rwOps = SDL_RWFromConstMem(buffer, dataTextureFileLength);
//		if (!rwOps)
//		{
//#ifdef LOGS
//			std::cerr << "Cannot create SDL_RWops, abort.\n";
//#endif
//			initOk = false;
//			return;
//		}
//
//		// Creating SDL_Surface from RWops:
//		SDL_Surface* surface = IMG_Load_RW(rwOps, 1); //1 for auto remove rwOps
//		if (!surface)
//		{
//#ifdef LOGS
//			std::cerr << "Cannot create surface, abort.\n";
//#endif
//			initOk = false;
//			return;
//		}
//
//		tetrisTexture = SDL_CreateTextureFromSurface(render, surface);
//		SDL_FreeSurface(surface);
//		if (!tetrisTexture)
//		{
//#ifdef LOGS
//			std::cerr << "Cannot create texture from surface, abort.\n";
//#endif
//			initOk = false;
//			return;
//		}
//	}
}