#pragma once
#include "logs.h"
#include "engine.h"
#include "tetrisroomconst.h"



namespace tetris
{
	class TetrisRoom
	{
	private:
		int tetrisRoomPixelArrayLength{ 0 };
		PixelArray* tetrisRoomPixelArray{ nullptr };
	public:
		TetrisRoom();
		TetrisRoom(const TetrisRoom&) = delete;
		TetrisRoom& operator=(const TetrisRoom&) = delete;
		TetrisRoom(TetrisRoom&&) = delete;
		~TetrisRoom();

#ifdef LOGS
		friend std::ostream& operator<<(std::ostream& os, const TetrisRoom& tr);
#endif
	};


}