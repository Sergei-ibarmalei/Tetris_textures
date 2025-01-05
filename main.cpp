#include <string>
#include <fstream>
#include <zlib.h>

#include "sdl_.h"
#include "prog.h"


uint32_t calculate_crc32(const std::string& filepath)
{
	const size_t buffer_size = 2230;
	uint8_t buffer[buffer_size];

	std::ifstream infile(filepath, std::ios::binary);
	if (!infile.is_open())
	{
		std::cout << "error by reading file, abort.\n";
		return 0u;
	}
	uint32_t crc = 0;
	infile.read(reinterpret_cast<char*>(buffer), buffer_size);
	std::streamsize bytes_read = infile.gcount();

	crc = crc32(crc, buffer, static_cast<uInt>(bytes_read));
	
	return crc;
	//std::cout << "CRC32 for file " << filepath << " : " << std::hex << std::endl;
	//infile.close();
	//return 1;
}

int main(int argc, char** argv)
{
	using namespace tetris;
	//std::string filedatapath = "data.data";
	//std::cout << std::hex << calculate_crc32(filedatapath) << std::endl;



	Sdl sdl;
	if (sdl.InitOk() == false) return -1;
	Prog* tetrisProg = new Prog(sdl.Render());
	if (tetrisProg->InitOk() == false) return -1;




	//Storage storage(sdl.Render());
	//TetrisRoom tetrisRoom(storage[TetrisTextureRects::room],
	//	storage[TetrisTextureRects::wall], storage[TexturesStorageNames::tetris], sdl.Render());
	//if (tetrisRoom.InitOk() == false) return -1;
	////TetraminoPlane* plane = static_cast<TetraminoPlane*>(storage[TetraminoKind::plane]);
	//TetraminoT* currentTetramino = static_cast<TetraminoT*>(storage[TetraminoKind::t]);
	//

	//bool quit = false;
	//while (!quit)
	//{
	//	while (SDL_PollEvent(sdl.Event()) != 0)
	//	{
	//		if (sdl.Event()->type == SDL_QUIT) quit = true;
	//	}
	//	SDL_SetRenderDrawColor(sdl.Render(), 0u, 0u, 0u, 0xffu);
	//	SDL_RenderClear(sdl.Render());
	//	tetrisRoom.Show();
	//	currentTetramino->Show();
	//	SDL_RenderPresent(sdl.Render());
	//}


	return 0;
}