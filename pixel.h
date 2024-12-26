#pragma once

#include <SDL.h>
#include "logs.h"

#ifdef LOGS
#include <iostream>
#endif


namespace tetris
{
	struct pixel
	{
		SDL_Rect pixelRect{};
		int row{ 0 };
		int col{ 0 };

#ifdef LOGS
		friend std::ostream& operator<<(std::ostream& os, const pixel& p);
#endif
	};

	class PixelArray
	{
	private:
		pixel* pixelArray{ nullptr };
		int    pixelArrayLength{ 0 };
	public:
		PixelArray(int pixelArrayLength);
		~PixelArray();
		PixelArray(const PixelArray&) = delete;
		PixelArray& operator=(const PixelArray&) = delete;
		PixelArray(PixelArray&&) = delete;
		pixel* operator[](const int index);
		pixel* GetArray() { return pixelArray; }

#ifdef LOGS
		friend std::ostream& operator<<(std::ostream& os, const PixelArray& pa);
#endif
	};
}