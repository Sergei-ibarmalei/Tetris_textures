#pragma once

namespace tetris
{
	enum class TetraminoKind { plane, cube, l, g, right, left, t, all };
	constexpr int planeTetramino{ 0 };
	constexpr int cubeTetramino{ 1 };
	constexpr int lTetramino{ 2 };
	constexpr int gTetramino{ 3 };
	constexpr int rightTetramino{ 4 };
	constexpr int leftTetramino{ 5 };
	constexpr int tTetramino{ 6 };
	constexpr int allTetramino{ 7 };

	//enum class TetrisTextureRects {wall, room, plane, 
	//	cube, l, g, right, left,
	//				t, all};
	enum class TetrisTextureRects {l, g, right, left,
	cube, plane, t, room, wall, all};
	enum class TexturesStorageNames {tetris, all};
	enum class Direction {right, left, down, all};

}