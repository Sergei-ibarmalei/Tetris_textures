#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "logs.h"

#ifdef LOGS
#include <iostream>
#endif


namespace tetris
{

	struct matrixPixel
	{
		int row{ 0 };
		int col{ 0 };
		bool filled{ false };

#ifdef LOGS
		friend std::ostream& operator<<(std::ostream& os, const matrixPixel& mp);
#endif
	};

	struct texturePixel
	{
		SDL_Texture* pixelTexture{ nullptr };
		SDL_Rect sourceRect{};
		SDL_Rect pixelRect{};
		//bool filled{ false };
		int row{ 0 };
		int col{ 0 };

		~texturePixel()
		{
			pixelTexture = nullptr;
		}

		texturePixel& operator=(const matrixPixel& mp);



#ifdef LOGS
		friend std::ostream& operator<<(std::ostream& os, const texturePixel& p);
#endif
	};




}