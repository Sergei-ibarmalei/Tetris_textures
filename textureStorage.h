#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
//#include <zlib.h>

#include "logs.h"
#ifdef LOGS
#include <iostream>
#endif


namespace tetris
{
	class TexturesStorage
	{
	private:
		SDL_Texture** storage{ nullptr };
		bool initOk{ true };

		void readData(uint8_t* buffer, const char* fileName, int dataLength);
	public:
		TexturesStorage();
		~TexturesStorage();
		TexturesStorage(const TexturesStorage&) = delete;
		TexturesStorage& operator=(const TexturesStorage&) = delete;
		TexturesStorage(TexturesStorage&&) = delete;

	};
}



