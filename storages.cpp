#include "storages.h"
#include <fstream>

#define int_ static_cast<int>

static SDL_Rect lRect{ 1, 1, 46, 46 };
static SDL_Rect gRect{ 1, 49, 46, 46 };
static SDL_Rect rightRect{ 1, 97, 46, 46 };
static SDL_Rect leftRect{ 1, 145, 46, 46 };
static SDL_Rect cubeRect{ 1, 193, 46, 46 };
static SDL_Rect planeRect{ 1, 241, 46, 46 };
static SDL_Rect tRect{ 1, 289, 46, 46 };
static SDL_Rect wallRect{ 1, 385, 46, 46 };
static SDL_Rect roomRect{ 1, 337, 46, 46 }; 


//static SDL_Rect lRect{ 1, 1, 46, 46 };
//static SDL_Rect gRect{ 1, 49, 45, 45 };
//static SDL_Rect rightRect{ 1, 95, 45, 45 };
//static SDL_Rect leftRect{ 1, 142, 45, 45 };
//static SDL_Rect cubeRect{ 1, 189, 45, 45 };
//static SDL_Rect planeRect{ 1, 236, 45, 45 };
//static SDL_Rect tRect{ 1, 283, 45, 45 };
//static SDL_Rect wallRect{ 1, 330, 45, 45 };
//static SDL_Rect roomRect{ 1, 377, 45, 45 };

namespace tetris
{
	Storage::Storage(SDL_Renderer* render)
	{
		if (!render)
		{
#ifdef LOGS
			std::cerr << "Cannot create storage, render is absent.\n";
#endif
			initOk = false; return;
		}
		this->render = render;
		if (!loadTextures())
		{
			initOk = false; return;
		}
		initRectStorage();
		initTetraminoStorage(allTextures[int_(TexturesStorageNames::tetris)]);

		
	}



	bool Storage::checkData(const char* fileName, uint8_t* buffer,
		size_t bufferLength, const uint32_t expectedCRC)
	{
		std::ifstream inFile(fileName, std::ios::binary);
		if (!inFile.is_open())
		{
#ifdef LOGS
			std::cerr << "Cannot open file: " << fileName << ", abort.\n";
#endif
			inFile.close();
			return false;
		}

		inFile.read(reinterpret_cast<char*>(buffer), bufferLength);
		uint32_t crc = crc32(0, buffer, static_cast<uInt>(bufferLength));
		if (crc != expectedCRC)
		{
#ifdef LOGS
			std::cerr << "Computed crc: " << std::hex << crc << std::endl;
			std::cerr << "Expected crc: " << std::hex << expectedCRC << std::endl;
			std::cerr << "Check data for file " << fileName << " is failed, abort.\n";
#endif
			inFile.close();
			return false;
		}

		// ecrypt data from file:
		char key = 'K';
		for (int i = 0; i < bufferLength; ++i)
		{
			buffer[i] ^= key;
		}
		inFile.close();
		return true;

	}



#ifdef LOGS
	SDL_Texture* Storage::makeTextureFromData(const char* fileName,
		const uint8_t* data, const int dataLength, SDL_Renderer* render)
#else
	SDL_Texture* Storage::makeTextureFromData(const uint8_t* data,
		const int dataLength, SDL_Renderer* render)
#endif
	{
		SDL_RWops* rwOps = SDL_RWFromConstMem(data, dataLength);
		if (!rwOps)
		{
#ifdef LOGS
			std::cerr << "Cannot make texture, " << fileName << ", abort.\n";
#endif
			return nullptr;
		}

		// Createing SDL_Surface from RWops:
		SDL_Surface* surface = IMG_Load_RW(rwOps, 1); // 1 for auto remove rwOps
		if (!surface)
		{
#ifdef LOGS
			std::cerr << "Cannot create surface by making texture from " <<
				fileName << ", abort.\n";
#endif
			return nullptr;
		}
		SDL_Texture* madeTexture = SDL_CreateTextureFromSurface(render, surface);
		if (!madeTexture)
		{
#ifdef LOGS
			std::cout << "Cannot make texture from surface by file " << fileName <<
				", abort.\n";
#endif
			return nullptr;
		}
		return madeTexture;
	}

	bool Storage::loadTextures()
	{
		allTextures = new SDL_Texture * [int_(TexturesStorageNames::all)]{ nullptr };

		const char* tetrispngDataFileName = "data.data";
		const auto  tetrispngDataLength{ 1882 }; // length of data.data file
		uint8_t* tetrispngBuffer = new uint8_t[tetrispngDataLength];
		uint32_t tetrispngcrc = 0x7eede3e6;
		if (!checkData(tetrispngDataFileName, tetrispngBuffer, 
			tetrispngDataLength, tetrispngcrc))
		{
			return false;
		}
		
#ifdef LOGS
		SDL_Texture* tetrisTexture = makeTextureFromData(tetrispngDataFileName,
			tetrispngBuffer, tetrispngDataLength, render);
#else
		SDL_Texture* tetrisTexture = makeTextureFromData(tetrispngBuffer,
			tetrispngDataLength, render);
#endif
		if (!tetrisTexture) return false;

		allTextures[static_cast<int>(TexturesStorageNames::tetris)] =
			tetrisTexture;

		
		delete[] tetrispngBuffer;
		tetrispngBuffer = nullptr;

		return true;

	}

	void Storage::initTetraminoStorage(SDL_Texture* tetrisTexture)
	{
		tetraminoStorage = new Tetramino * [int_(TetraminoKind::all)];
		tetraminoStorage[int_(TetraminoKind::plane)] =
			new TetraminoPlane(tetrisTexture, render, planeRect, planeInits, initsLength);
		if (tetraminoStorage[int_(TetraminoKind::plane)]->InitOk() == false)
		{
			initOk = false;
#ifdef LOGS
			std::cerr << "Cannot make plane tetramino, abort.\n";
#endif
			return;
		}

		tetraminoStorage[int_(TetraminoKind::cube)] =
			new TetraminoCube(tetrisTexture, render, cubeRect, cubeInits, initsLength);
		if (tetraminoStorage[int_(TetraminoKind::cube)]->InitOk() == false)
		{
			initOk = false;
#ifdef LOGS
			std::cerr << "Cannot make cube tetramino, abort.\n";
#endif
			return;
		}

		tetraminoStorage[int_(TetraminoKind::l)] =
			new TetraminoL(tetrisTexture, render, lRect, lInits, initsLength);
		if (tetraminoStorage[int_(TetraminoKind::l)]->InitOk() == false)
		{
			initOk = false; 
#ifdef LOGS
			std::cerr << "Cannot make L tetramino, abort.\n";
#endif
			return;
		}

		tetraminoStorage[int_(TetraminoKind::g)] =
			new TetraminoG(tetrisTexture, render, gRect, gInits, initsLength);
		if (tetraminoStorage[int_(TetraminoKind::g)]->InitOk() == false)
		{
			initOk = false; 
#ifdef LOGS
			std::cerr << "Cannot make G tetramino, abort.\n";
#endif
			return;
		}

		tetraminoStorage[int_(TetraminoKind::right)] =
			new TetraminoRight(tetrisTexture, render, rightRect, rightInits, initsLength);
		if (tetraminoStorage[int_(TetraminoKind::right)]->InitOk() == false)
		{
			initOk = false; 
#ifdef LOGS
			std::cerr << "Cannot make right tetramino, abort.\n";
#endif
			return;
		}

		tetraminoStorage[int_(TetraminoKind::left)] =
			new TetraminoLeft(tetrisTexture, render,  leftRect, leftInits, initsLength);
		if (tetraminoStorage[int_(TetraminoKind::left)]->InitOk() == false)
		{
			initOk = false; 
#ifdef LOGS
			std::cerr << "Cannot make left tetramino, abort.\n";
#endif
			return;
		}

		tetraminoStorage[int_(TetraminoKind::t)] =
			new TetraminoT(tetrisTexture, render, tRect, tInits, initsLength);
		if (tetraminoStorage[int_(TetraminoKind::t)]->InitOk() == false)
		{
			initOk = false; 
#ifdef LOGS
			std::cerr << "Cannot make t tetramino, abort.\n";
#endif
			return;
		}
	}

	Tetramino* Storage::operator[](const TetraminoKind k)
	{

		return tetraminoStorage[int_(k)];
	}

	void Storage::initRectStorage()
	{
		rectStorage = new SDL_Rect[int_(TetrisTextureRects::all)];
		rectStorage[int_(TetrisTextureRects::wall)] = wallRect;
		rectStorage[int_(TetrisTextureRects::room)] = roomRect;
		rectStorage[int_(TetrisTextureRects::plane)] = planeRect;
		rectStorage[int_(TetrisTextureRects::cube)] = cubeRect;
		rectStorage[int_(TetrisTextureRects::l)] = lRect;
		rectStorage[int_(TetrisTextureRects::g)] = gRect;
		rectStorage[int_(TetrisTextureRects::right)] = rightRect;
		rectStorage[int_(TetrisTextureRects::left)] = leftRect;
		rectStorage[int_(TetrisTextureRects::t)] = tRect;
	}

	const SDL_Rect Storage::operator[](const TetrisTextureRects r) const
	{
		return rectStorage[int_(r)];
	}

	SDL_Texture* Storage::operator[](const TexturesStorageNames name)
	{
		return allTextures[int_(name)];
	}

	Storage::~Storage()
	{
		delete[] rectStorage;
		rectStorage = nullptr;
		for (int t = 0; t < int_(TetraminoKind::all); ++t)
		{
			delete tetraminoStorage[t];
			tetraminoStorage[t] = nullptr;
		}
		delete[] tetraminoStorage;
		tetraminoStorage = nullptr;

		for (int i = 0; i < int_(TexturesStorageNames::all); ++i)
		{
			SDL_DestroyTexture(allTextures[i]);
			allTextures[i] = nullptr;
		}
		delete[] allTextures;
		allTextures = nullptr;
		render = nullptr;
	}
}

#undef int_