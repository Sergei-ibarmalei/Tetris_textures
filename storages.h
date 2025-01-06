#pragma once


#include <SDL.h>
#include <zlib.h>

#include "rects.h"
#include "names.h"
#include "tetrisRoom.h"


namespace tetris
{
	class Storage
	{
	private:
		SDL_Texture** allTextures{ nullptr };
		SDL_Rect*  rectStorage{ nullptr };
		Tetramino** tetraminoStorage{ nullptr };
		SDL_Renderer* render{ nullptr };
		bool loadTextures();
		void initRectStorage();
		void initTetraminoStorage(SDL_Texture* tetrisTexture);
		bool checkData(const char* fileName, uint8_t* buffer,
			size_t bufferLength, const uint32_t expectedCRC);
#ifdef LOGS
		SDL_Texture* makeTextureFromData(const char* fileName, const uint8_t* data,
			const int dataLength, SDL_Renderer* render);
#else
		SDL_Texture* makeTextureFromData(const uint8_t* data, 
			const int dataLength, SDL_Renderer* render);
#endif 
		bool initOk{ true };
	public:
		explicit Storage(SDL_Renderer* render);
		~Storage();
		Storage(const Storage&) = delete;
		Storage& operator=(const Storage&) = delete;
		Storage(Storage&&) = delete;

		const SDL_Rect operator[](const TetrisTextureRects r) const;
		Tetramino* operator[](const TetraminoKind r);
		SDL_Texture* operator[](const TexturesStorageNames name);
		bool InitOk() const { return initOk; }


		SDL_Rect* RectStorage() const { return rectStorage; }
		SDL_Texture** AllTextures()  { return allTextures; }
	};


}