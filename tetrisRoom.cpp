#include "tetrisRoom.h"

static SDL_Rect tmpRect{};
static SDL_Point startRoomLeftUp{ ROOMLEFTSIDE_X, ROOMLEFTSIDE_Y };
static SDL_Point startWallsLeftUp{ LEFTWALL_LEFTX, LEFTWALL_LEFTY };
static SDL_Point startWallsDownUp{ DOWNWALL_LEFTX, DOWNWALL_LEFTY };
static SDL_Point startWallsRightUp{ RIGHTWALL_LEFTX, RIGHTWALL_LEFTY };

//enum class direction {right, left, down, all};

//static void shiftTetraminoLeft(tetris::matrixPixel* matrix,
//	const int matrixLength)
//{
//	if (!matrix)
//	{
//#ifdef LOGS
//		std::cerr << "Cannot shift, data is absent.\n";
//#endif
//		return;
//	}
//	for (int i = 0; i < matrixLength; ++i)
//	{
//		matrix[i].col--;
//	}
//}
//
//void shiftTetraminoRight(tetris::matrixPixel* matrix,
//	const int matrixLength)
//{
//	if (!matrix)
//	{
//#ifdef LOGS
//		std::cerr << "Cannot shift, data is absent.\n";
//#endif
//		return;
//	}
//	for (int i = 0; i < matrixLength; ++i)
//	{
//		matrix[i].col++;
//	}
//}

bool checkHitWall(tetris::Direction dir, tetris::texturePixel* tetramino, 
	const tetris::texturePixel* workSpace)
{
	if (!tetramino || !workSpace)
	{
#ifdef LOGS
		std::cerr << "Cannot check hit wall, data is absent.\n";
#endif
		return false;
	}

	bool canMove = true;

	switch (dir)
	{
	case tetris::Direction::right:
	{
		for (int i = 0; i < REALTETRAMINO_LENGTH; ++i)
		{
			const auto nextCol = tetramino[i].col + 1;
			const auto currentRow = tetramino[i].row;

			if (nextCol == ROOMWIDTH_PIXELS ||
				workSpace[currentRow * ROOMWIDTH_PIXELS + nextCol].pixelTexture)
			{
				canMove = false; break;
			}
		}

		break;
	}
	case tetris::Direction::left:
	{
		for (int i = 0; i < REALTETRAMINO_LENGTH; ++i)
		{
			const auto prevCol = tetramino[i].col - 1;
			const auto currentRow = tetramino[i].row;
			if (prevCol < 0 ||
				workSpace[currentRow * ROOMWIDTH_PIXELS + prevCol].pixelTexture)
			{
				canMove = false; break;
			}
		}
		break;
	}
	case tetris::Direction::down:
	{
		for (int i = 0; i < REALTETRAMINO_LENGTH; ++i)
		{
			const auto nextRow = tetramino[i].row + 1;
			const auto currentCol = tetramino[i].col;
			if (nextRow == ROOMHEIGHT_PIXELS ||
				workSpace[nextRow * ROOMWIDTH_PIXELS + currentCol].pixelTexture)
			{
				canMove = false; break;
			}

		}
		break;
	}
	default:
		break;
	}

	return canMove;
}

void geomtryInitRoom(tetris::texturePixel* room)
{
	if (!room)
	{
#ifdef LOGS
		std::cout << "Cannot init room, data is absent.\n";
#endif
		return;
	}
	for (int row = 0; row < ROOMHEIGHT_PIXELS; ++row)
	{
		for (int col = 0; col < ROOMWIDTH_PIXELS; ++col)
		{
			const auto AT = row * ROOMWIDTH_PIXELS + col;
			room[AT].row = row;
			room[AT].col = col;
			room[AT].pixelRect.x =
				startRoomLeftUp.x + col * ROOMPIXELSIDE;
			room[AT].pixelRect.y =
				startRoomLeftUp.y + row * ROOMPIXELSIDE;
			room[AT].pixelRect.w =
				room[AT].pixelRect.h =
				ROOMPIXELSIDE;
		}
	}
}

void Showing(SDL_Renderer* render, tetris::texturePixel* array, const int length)
{
	if (!render || !array) return; 

	for (int i = 0; i < length; ++i)
	{
		if (array[i].pixelTexture)
			SDL_RenderCopy(render, array[i].pixelTexture, &array[i].sourceRect,
				&array[i].pixelRect);
	}
}

namespace tetris
{
	TetrisRoom::TetrisRoom(const SDL_Rect sourceRectsForeground,
		const SDL_Rect sourceRectsWalls,
		SDL_Texture* texture,
		SDL_Renderer* render)
	{
		if (!texture)
		{
#ifdef LOGS
			std::cerr << "Cannot create tetris room, texture is absent.\n";
#endif
			initOk = false; return;
		}
		if (!render)
		{
#ifdef LOGS
			std::cerr << "Cannot create tetris room, render is absent\n";
#endif
			initOk = false; return;
		}
		tetrisTexture = texture;
		tetrisRender = render;

		tetrisRoomPixelArrayLength = ROOMWIDTH_PIXELS * ROOMHEIGHT_PIXELS;
		tetrisBorderRoomPixelArrayLength = WALLPIXELARRAY_LENGTH;
		tetrisRoomPixelArray = new texturePixel[tetrisRoomPixelArrayLength];

		tetrisBorderRoomPixelArray = new texturePixel[tetrisBorderRoomPixelArrayLength];
		if (!tetrisRoomPixelArray || !tetrisBorderRoomPixelArray)
		{
#ifdef LOGS
			std::cerr << "Cannot create tetris room, memory access violation.\n";
#endif
			initOk = false; return;
		}

		init(sourceRectsWalls, sourceRectsForeground);
	}

	void TetrisRoom::init(const SDL_Rect& wallSourceRect, const SDL_Rect& roomSourceRect) const
	{
		// initiate SDL_Rects in tetris room:
		// ROOM:
		geomtryInitRoom(tetrisRoomPixelArray);
		for (int row = 0; row < ROOMHEIGHT_PIXELS; ++row)
		{
			for (int col = 0; col < ROOMWIDTH_PIXELS; ++col)
			{
				const auto AT = row * ROOMWIDTH_PIXELS + col;
				tetrisRoomPixelArray[AT].pixelTexture = tetrisTexture;
				tetrisRoomPixelArray[AT].sourceRect = roomSourceRect;
			}
		}

		// initiate SDL_Rects in left wall:

		const auto sideWallHeight = ROOMHEIGHT_PIXELS + 1;
		const auto floorWallWidth = ROOMWIDTH_PIXELS;

		for (int i = 0; i < sideWallHeight; ++i)
		{
			tetrisBorderRoomPixelArray[i].pixelRect.x = startWallsLeftUp.x;
			tetrisBorderRoomPixelArray[i].pixelRect.y =
				startWallsLeftUp.y + (i * ROOMPIXELSIDE);
			tetrisBorderRoomPixelArray[i].pixelRect.w =
				tetrisBorderRoomPixelArray[i].pixelRect.h = ROOMPIXELSIDE;

			tetrisBorderRoomPixelArray[i].pixelTexture = tetrisTexture;
			tetrisBorderRoomPixelArray[i].sourceRect = wallSourceRect;
		}
		int j = 0;
		for (int i = sideWallHeight; i < (sideWallHeight + floorWallWidth); ++i)
		{
			tetrisBorderRoomPixelArray[i].pixelRect.x =
				startWallsDownUp.x + (j++ * ROOMPIXELSIDE);
			tetrisBorderRoomPixelArray[i].pixelRect.y = startWallsDownUp.y;
			tetrisBorderRoomPixelArray[i].pixelRect.w =
				tetrisBorderRoomPixelArray[i].pixelRect.h =
				ROOMPIXELSIDE;


			tetrisBorderRoomPixelArray[i].pixelTexture = tetrisTexture;
			tetrisBorderRoomPixelArray[i].sourceRect = wallSourceRect;
		}
		j = 0;
		for (int i = (sideWallHeight + floorWallWidth); i < WALLPIXELARRAY_LENGTH; ++i)
		{

			tetrisBorderRoomPixelArray[i].pixelRect.x = startWallsRightUp.x;
			tetrisBorderRoomPixelArray[i].pixelRect.y =
				startWallsRightUp.y + (j++ * ROOMPIXELSIDE);
			tetrisBorderRoomPixelArray[i].pixelRect.w =
				tetrisBorderRoomPixelArray[i].pixelRect.h = ROOMPIXELSIDE;


			tetrisBorderRoomPixelArray[i].pixelTexture = tetrisTexture;
			tetrisBorderRoomPixelArray[i].sourceRect = wallSourceRect;
		}
	}

	void TetrisRoom::Show()
	{
		Showing(this->tetrisRender, tetrisBorderRoomPixelArray,
			tetrisBorderRoomPixelArrayLength);
		Showing(this->tetrisRender, tetrisRoomPixelArray,
			tetrisRoomPixelArrayLength);
	}

	TetrisRoom::~TetrisRoom()
	{
		delete[] tetrisRoomPixelArray;
		tetrisRoomPixelArray = nullptr;
		delete[] tetrisBorderRoomPixelArray;
		tetrisBorderRoomPixelArray = nullptr;
		tetrisTexture = nullptr;
		tetrisRender = nullptr;
	}






	Tetramino::Tetramino(TetraminoKind kind,
		SDL_Texture* texture,
		SDL_Renderer* render,
		const SDL_Rect sourceRect,
		const SDL_Point* initPoints,
		const int initPointsLength)
	{
		if (!texture)
		{
#ifdef LOGS
			std::cerr << "Cannot create tetramino, texture is absent.\n";
#endif
			initOk = false;
			return;
		}
		if (!render)
		{
#ifdef LOGS
			std::cerr << "Cannot create tetramino, render is absen.\n";
#endif
			initOk = false;
			return;
		}
		if (!initPoints)
		{
#ifdef LOGS
			std::cerr << "Cannot create tetramino, data is absent.\n";
#endif
			initOk = false; return;
		}
		tetrisTexture = texture;
		this->render = render;
		this->tetraminoKind = kind;
		tetraminoSourceRect = sourceRect;
		switch (this->tetraminoKind)
		{
		case TetraminoKind::plane:
		{
			matrix = new matrixPixel[PLANE_MATRIXLENGTH];
			rotatedMatrix = new matrixPixel[PLANE_MATRIXLENGTH];
			matrixSide = 4;
			rotatedMatrixSide = 4;
			break;
		}
		case TetraminoKind::cube:
		{
			matrix = new matrixPixel[CUBE_MATRIXLENGTH];
			rotatedMatrix = new matrixPixel[CUBE_MATRIXLENGTH];
			matrixSide = 2;
			rotatedMatrixSide = 2;
			break;
		}
		case TetraminoKind::l:
		{
			matrix = new matrixPixel[LGRIGHTLEFTT_MATRIXLENGTH];
			rotatedMatrix = new matrixPixel[LGRIGHTLEFTT_MATRIXLENGTH];
			matrixSide = 3;
			rotatedMatrixSide = 3;
			break;
		}
		case TetraminoKind::g:
		{
			matrix = new matrixPixel[LGRIGHTLEFTT_MATRIXLENGTH];
			rotatedMatrix = new matrixPixel[LGRIGHTLEFTT_MATRIXLENGTH];
			matrixSide = 3;
			rotatedMatrixSide = 3;
			break;
		}
		case TetraminoKind::right:
		{
			matrix = new matrixPixel[LGRIGHTLEFTT_MATRIXLENGTH];
			rotatedMatrix = new matrixPixel[LGRIGHTLEFTT_MATRIXLENGTH];
			matrixSide = 3;
			rotatedMatrixSide = 3;
			break;
		}
		case TetraminoKind::left:
		{
			matrix = new matrixPixel[LGRIGHTLEFTT_MATRIXLENGTH];
			rotatedMatrix = new matrixPixel[LGRIGHTLEFTT_MATRIXLENGTH];
			matrixSide = 3;
			rotatedMatrixSide = 3;
			break;
		}
		case TetraminoKind::t:
		{
			matrix = new matrixPixel[LGRIGHTLEFTT_MATRIXLENGTH];
			rotatedMatrix = new matrixPixel[LGRIGHTLEFTT_MATRIXLENGTH];
			matrixSide = 3;
			rotatedMatrixSide = 3;
			break;
		}
		default:
			break;

		}
		if (!matrix || !rotatedMatrix)
		{
#ifdef LOGS
			std::cerr << "Cannot create tetramino, memory access damage.\n";
#endif
			initOk = false; return;
		}

		realTetramino = new texturePixel[REALTETRAMINO_LENGTH];
		if (!realTetramino)
		{
#ifdef LOGS
			std::cerr << "Cannot create real tetramino, abort.\n";
#endif
			initOk = false; return;
		}
		matrixLength = matrixSide * matrixSide;
		rotatedMatrixLength = rotatedMatrixSide * rotatedMatrixSide;

		initMatrix(initPoints, initPointsLength);
		makeRealTetramino();
		recomputeRects();

	}

	void Tetramino::initMatrix(const SDL_Point* initPoints, const int initPointsLength)
	{
		if (!initPoints)
		{
#ifdef LOGS
			std::cerr << "Cannot init matrix, data is absent.\n";
#endif
			return;
		}

		const auto startRow = initPoints[0].x;
		const auto startCol = initPoints[0].y;
		const auto lengthWidth = startRow + matrixSide;
		const auto lengthHeight = startCol + matrixSide;


		for (int row = startRow, mr = 0; row < lengthWidth; ++row, ++mr)
		{
			for (int col = startCol, mcol = 0; col < lengthHeight; ++col, ++mcol)
			{
				matrix[mr * this->matrixSide + mcol].row = row;
				matrix[mr * this->matrixSide + mcol].col = col;
			}
		}

		for (int i = 1; i < initPointsLength; ++i)
		{
			const auto row = initPoints[i].x;
			const auto col = initPoints[i].y;
			for (int j = 0; j < matrixLength; ++j)
			{
				if (matrix[j].row == row && matrix[j].col == col)
				{
					matrix[j].filled = true;
				}
			}
		}

		// fill real tetramino by texture and source rects
		for (int i = 0; i < REALTETRAMINO_LENGTH; ++i)
		{
			realTetramino[i].pixelTexture = tetrisTexture;
			realTetramino[i].sourceRect = tetraminoSourceRect;
		}
	}


	void Tetramino::recomputeRects()
	{
		for (int i = 0; i < REALTETRAMINO_LENGTH; ++i)
		{
			realTetramino[i].pixelRect.x =
				ROOMLEFTSIDE_X + (realTetramino[i].col * ROOMPIXELSIDE);
			realTetramino[i].pixelRect.y =
				ROOMLEFTSIDE_Y + (realTetramino[i].row * ROOMPIXELSIDE);
			realTetramino[i].pixelRect.w = realTetramino[i].pixelRect.h =
				ROOMPIXELSIDE;
		}
	}

	void Tetramino::makeRealTetramino()
	{
		int realTetraminoIndex{ 0 };

		for (int i = 0; i < matrixLength; ++i)
		{
			if (matrix[i].filled)
			{
				realTetramino[realTetraminoIndex] = matrix[i];
				realTetraminoIndex++;
				if (realTetraminoIndex == 4) break;
			}
		}
	}




	void Tetramino::Show()
	{
		Showing(this->render, realTetramino, REALTETRAMINO_LENGTH);
	}

	void Tetramino::Move(Direction dir, const texturePixel* workSpace)
	{
		if (!workSpace)
		{
#ifdef LOGS
			std::cerr << "Cannot move, data is absent.\n";
#endif
			return;
		}
		if (fixed) return;
		if (!checkHitWall(dir, realTetramino, workSpace))
		{
			if (dir == Direction::down) fixed = true;
			return;
		}

		switch (dir)
		{
		case Direction::down:
		{
			for (int i = 0; i < matrixLength; ++i)
			{
				matrix[i].row++;
			}
			break;
		}
		case Direction::right:
		{
			for (int i = 0; i < matrixLength; ++i)
			{
				matrix[i].col++;
			}
			break;
		}
		case Direction::left:
		{
			for (int i = 0; i < matrixLength; ++i)
			{
				matrix[i].col--;
			}
			break;
		}
		default:
			break;
		}

		makeRealTetramino();
		recomputeRects();
	}




	// bothLength of first and second must be equal:
	void Tetramino::copyMatrixs(matrixPixel* first, matrixPixel* second,
		const int bothLength)
	{
		if (!first || !second)
		{
#ifdef LOGS
			std::cerr << "Cannot copy matrixs, data is absent.\n";
#endif
			return;
		}
		for (int i = 0; i < bothLength; ++i)
		{
			first[i] = second[i];
		}
	}

	static bool willHitWallOrTexture(const tetris::matrixPixel* matrix,
		const tetris::texturePixel* workSpace,
		const int atWall,
		const int atTexture)
	{
		if (!matrix || !workSpace)
		{
#ifdef LOGS
			std::cerr << "Cannot check wall hit, data is absent.\n";
#endif
			return true;
		}
		if ((matrix[atWall].col == ROOMWIDTH_PIXELS) ||
			(matrix[atWall].col < 0) ||
			workSpace[atTexture].pixelTexture) return true;
		return false;

	}

	void Tetramino::RotateClockWise(const texturePixel* workSpace)
	{
		if (!workSpace)
		{
#ifdef LOGS
			std::cerr << "Cannot rotate clockwise, workspace is absent\n";
#endif
			return;
		}
		copyMatrixs(rotatedMatrix, matrix, matrixLength);
		for (int row = 0; row < matrixSide; ++row)
		{
			for (int col = 0; col < matrixSide; ++col)
			{
				const auto rotatedAT = col * rotatedMatrixSide + 
					((rotatedMatrixSide - 1) - row);
				const auto matrixAT = row * matrixSide + col;
				const auto rotatedRow = rotatedMatrix[rotatedAT].row;
				const auto rotatedCol = rotatedMatrix[rotatedAT].col;
				const auto workRotatedAT = rotatedRow * ROOMWIDTH_PIXELS +
					rotatedCol;
				rotatedMatrix[rotatedAT].filled = 
					matrix[matrixAT].filled;

				if (willHitWallOrTexture(rotatedMatrix, workSpace, rotatedAT, 
					workRotatedAT))
					return;
				
			}
		} 


		copyMatrixs(matrix, rotatedMatrix, matrixLength);
		makeRealTetramino();
		recomputeRects();
	}

	void Tetramino::RotateCounterClockWise(const texturePixel* workSpace)
	{
		if (!workSpace)
		{
#ifdef LOGS
			std::cerr << "Cannot rotate counterclockwise, workspace is absent\n";
#endif
			return;
		}
		copyMatrixs(rotatedMatrix, matrix, matrixLength);
		for (int row = 0; row < matrixSide; ++row)
		{
			for (int col = 0; col < matrixSide; ++col)
			{
				const auto rotatedAT = ((rotatedMatrixSide-1) - col) * 
					rotatedMatrixSide + row;
				const auto matrixAT = row * matrixSide + col;
				const auto rotatedRow = rotatedMatrix[rotatedAT].row;
				const auto rotatedCol = rotatedMatrix[rotatedAT].col;
				const auto workRotatedAT = rotatedRow * ROOMWIDTH_PIXELS + 
					rotatedCol;

				rotatedMatrix[rotatedAT].filled =
					matrix[matrixAT].filled;
				if (willHitWallOrTexture(rotatedMatrix, workSpace, rotatedAT, 
					workRotatedAT))
					return;
			}
		}
		copyMatrixs(matrix, rotatedMatrix, matrixLength);
		makeRealTetramino();
		recomputeRects();
	}

	Tetramino::~Tetramino()
	{
		tetrisTexture = nullptr;
		delete[] matrix;
		matrix = nullptr;
		delete[] rotatedMatrix;
		rotatedMatrix = nullptr;
		delete[] realTetramino;
		realTetramino = nullptr;
		render = nullptr;
	}

#ifdef LOGS
	std::ostream& operator<<(std::ostream& os, const Tetramino& t)
	{
		os << "real tetramino: \n";
		for (int i = 0; i < REALTETRAMINO_LENGTH; ++i)
		{
			os << t.realTetramino[i] << ' ';
		}
		os << std::endl;
		os << "matrix: \n";
		int side = t.matrixSide;
		int index = 0;
		for (int i = 0; i < t.matrixLength; ++i)
		{
			os << t.matrix[i] << ' ';
			index++;
			if (index == t.matrixSide)
			{
				index = 0; os << std::endl;
			}
		}
		return os << std::endl;
	}
#endif



	WorkSpace::WorkSpace(SDL_Renderer* render)
	{
		if (!render)
		{
#ifdef LOGS
			std::cerr << "Cannot create worksapce, renderer is absent.\n";
#endif
			initOk = false; return;
		}
		this->render = render;
		this->workSpace = new texturePixel[ROOMPIXELSCOUNT]{};
		geomtryInitRoom(this->workSpace);
	}

	void WorkSpace::Show()
	{
		Showing(this->render, this->workSpace, ROOMPIXELSCOUNT);
	}


	void WorkSpace::WorkSpaceOperate(const texturePixel* realTetramino,
		WorkSpaceOperation op)
	{
		for (int i = 0; i < REALTETRAMINO_LENGTH; ++i)
		{
			const auto AT = realTetramino[i].row * ROOMWIDTH_PIXELS +
				realTetramino[i].col;

			switch (op)
			{
			case WorkSpaceOperation::deletion:
			{
				workSpace[AT].pixelTexture = nullptr;
				break;
			}
			case WorkSpaceOperation::projection:
			{
				workSpace[AT].pixelRect = realTetramino[i].pixelRect;
				workSpace[AT].sourceRect = realTetramino[i].sourceRect;
				workSpace[AT].pixelTexture = realTetramino[i].pixelTexture;
				break;
			}
			default:
				break;
			}
		}
	}


	WorkSpace::~WorkSpace()
	{
		delete[] workSpace;
		workSpace = nullptr;
	}




	



#ifdef LOGS
	std::ostream& operator<<(std::ostream& os, const TetrisRoom& tr)
	{
		return os << *tr.tetrisRoomPixelArray;
	}
#endif
}