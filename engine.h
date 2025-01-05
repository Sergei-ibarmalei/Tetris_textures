#pragma once

#include <fstream>
#include "logs.h"
#include "pixel.h"


#ifdef LOGS
#include <iostream>
#endif


bool readEncrypted(const char* encryptedFileName, uint8_t* buffer,
	int bufferLength, const char cryptKey);

//bool initiatePixelArray(tetris::pixel* pixelArray, const int pixelArrayLength,
//	const SDL_Point& start, const int pixelSide, const int width,
//	const int height);

//void showPixelArray(SDL_Renderer* r, tetris::PixelArray* array,
//	const int arrayLength);