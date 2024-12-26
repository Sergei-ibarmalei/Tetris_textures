#include "tetrisRoom.h"

namespace tetris
{
	TetrisRoom::TetrisRoom()
	{
		tetrisRoomPixelArrayLength = ROOMWIDTH_PIXELS * ROOMHEIGHT_PIXELS;
		tetrisRoomPixelArray = new PixelArray(tetrisRoomPixelArrayLength);
		SDL_Point roomLeft{ ROOMLEFTSIDE_X, ROOMLEFTSIDE_Y };

		initiatePixelArray(tetrisRoomPixelArray->GetArray(),
			tetrisRoomPixelArrayLength,
			roomLeft,
			ROOMPIXELSIDE,
			ROOMWIDTH_PIXELS,
			ROOMHEIGHT_PIXELS);

	}

	TetrisRoom::~TetrisRoom()
	{
		delete tetrisRoomPixelArray;
		tetrisRoomPixelArray = nullptr;
	}



#ifdef LOGS
	std::ostream& operator<<(std::ostream& os, const TetrisRoom& tr)
	{
		return os << *tr.tetrisRoomPixelArray;
	}
#endif
}