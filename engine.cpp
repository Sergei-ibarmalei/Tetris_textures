#include "engine.h"

bool readEncrypted(const char* encryptedFileName, uint8_t* buffer,
	int bufferLength, const char cryptKey)
{
	std::ifstream inFile(encryptedFileName, std::ios::binary);
	if (!inFile.is_open())
	{
#ifdef LOGS
		std::cerr << "Cannot open " << encryptedFileName << ", abort.\n";
		return false;
#endif
	}

	// Reading data byte by byte:
	char byte;
	int index{ 0 };
	while (inFile.get(byte))
	{
		buffer[index++] = static_cast<uint8_t>(byte ^ cryptKey);
	}

	inFile.close();
	return true;
}

bool initiatePixelArray(tetris::pixel* pixelArray, const int pixelArrayLength,
	const SDL_Point& start, const int pixelSide, const int width, 
	const int height)
{
	if (!pixelArray)
	{
#ifdef LOGS
		std::cerr << "Cannot initiate pixel Array, it is nullptr.\n";
#endif
		return false;
	}
	if (width <= 0 || height <= 0)
	{
#ifdef LOGS
		std::cerr << "Cannot initiate pixel Array, width or height is zero.\n";
#endif
		return false;
	}
	
	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			const auto AT = row * width + col;
			pixelArray[AT].row = row;
			pixelArray[AT].col = col;
			pixelArray[AT].pixelRect.x = start.x + col * pixelSide;
			pixelArray[AT].pixelRect.y = start.y + row * pixelSide;
			pixelArray[AT].pixelRect.w = pixelArray[AT].pixelRect.h = pixelSide;
		}
	}
	return true;
	
}