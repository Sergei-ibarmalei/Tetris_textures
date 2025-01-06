#pragma once
#include "logs.h"
#include "pixel.h"
#include "names.h"
#include "tetrisroomconst.h"


constexpr int initsLength{ 5 };
constexpr SDL_Point planeInits[initsLength]{
	{0, 3}, // start point of plane matrix
	{1, 3}, {1, 4}, {1, 5}, {1, 6} // plane filled pixels
};


constexpr SDL_Point cubeInits[initsLength]{
	{0, 4},
	{0, 4}, {0, 5}, {1, 4}, {1, 5}
};

constexpr SDL_Point lInits[initsLength]{
	{0, 4},
	{0, 4}, {1, 4}, {2, 4}, {2, 5}
};

constexpr SDL_Point gInits[initsLength]{
	{0, 4},
	{0, 4}, {0, 5}, {1, 4}, {2, 4}
};

constexpr SDL_Point rightInits[initsLength]{
	{0, 3},
	{0, 4}, {0, 5}, {1, 3}, {1, 4}
};

constexpr SDL_Point leftInits[initsLength]{
	{0, 3},
	{0, 3}, {0, 4}, {1, 4}, {1, 5}
};

constexpr SDL_Point tInits[initsLength]{
	{0, 3},
	{0, 3}, {0, 4}, {0, 5}, {1, 4}
};











namespace tetris
{
	class TetrisRoom
	{
	private:
		int tetrisRoomPixelArrayLength{ 0 };
		int tetrisBorderRoomPixelArrayLength{ 0 };
		texturePixel* tetrisRoomPixelArray{ nullptr };
		texturePixel* tetrisBorderRoomPixelArray{ nullptr };

		SDL_Texture* tetrisTexture{ nullptr };
		SDL_Renderer* tetrisRender{ nullptr };

		bool initOk{ true };
		void init(const SDL_Rect& wallSourceRect, const SDL_Rect& roomSourceRect) const;
	public:
		TetrisRoom(const SDL_Rect sourceRectsForeground,
					const SDL_Rect sourceRectsWalls,
					SDL_Texture* texture,
					SDL_Renderer* render);
		TetrisRoom(const TetrisRoom&) = delete;
		TetrisRoom& operator=(const TetrisRoom&) = delete;
		TetrisRoom(TetrisRoom&&) = delete;
		~TetrisRoom();
		bool InitOk() const { return initOk; }
		void Show();

#ifdef LOGS
		friend std::ostream& operator<<(std::ostream& os, const TetrisRoom& tr);
#endif
	};



	class Tetramino
	{
	protected:
		matrixPixel* matrix{ nullptr };
		matrixPixel* rotatedMatrix{ nullptr };
		texturePixel* realTetramino{ nullptr };
		SDL_Texture* tetrisTexture{ nullptr };
		SDL_Renderer* render;
		SDL_Rect    tetraminoSourceRect{};


		int matrixSide{ 0 };
		int rotatedMatrixSide{ 0 };
		int matrixLength{ 0 };
		int rotatedMatrixLength{ 0 };
		bool initOk{ true };

		void initMatrix(const SDL_Point* initPoints, const int initPointLength);
		void recomputeRects();
		void makeRealTetramino();
		static void copyMatrixs(matrixPixel* first, matrixPixel* matrixPixelSecond, 
				const int bothLength);
		bool fixed{ false };

	public:
		TetraminoKind tetraminoKind;


		Tetramino(TetraminoKind tetraminoKind,
				SDL_Texture* texture,
				SDL_Renderer* render,
				const SDL_Rect sourceRect, const SDL_Point* initPoints,
				const int initPointsLength);
		~Tetramino();
		bool InitOk() const { return initOk; }
		bool Fixed()  const { return fixed; }
		void Unfix() { fixed = false; }
		void Show();
		void Move(Direction dir, const texturePixel* workSpace);
		void RotateClockWise(const texturePixel* workSpace);
		void RotateCounterClockWise(const texturePixel* workSpace);
		texturePixel* RealTetramino() const { return realTetramino; }

#ifdef LOGS
		friend std::ostream& operator<<(std::ostream& os, const Tetramino& t);
#endif
	};

	class TetraminoPlane final : public Tetramino
	{
	public:
		TetraminoPlane(SDL_Texture* texture,
			SDL_Renderer* render,
			const SDL_Rect sourceRects,
			const SDL_Point* initPoints,
			const int initPointsLength) :
			Tetramino(TetraminoKind::plane,
				texture,
				render,
				sourceRects,
				initPoints,
				initPointsLength) {}
	};

	class TetraminoCube final : public Tetramino
	{
	public:
		TetraminoCube(SDL_Texture* texture, 
			SDL_Renderer* render,
			const SDL_Rect sourceRects,
			const SDL_Point* initPoints,
			const int initPointsLength) :
			Tetramino(TetraminoKind::cube, 
				texture, 
				render,
				sourceRects,
				initPoints,
				initPointsLength) {}
	};

	class TetraminoL final : public Tetramino
	{
	public:
		TetraminoL(SDL_Texture* texture, 
			SDL_Renderer* render,
			const SDL_Rect sourceRects,
			const SDL_Point* initPoints,
			const int initPointsLength) :
			Tetramino(TetraminoKind::l, 
				texture, 
				render,
				sourceRects,
				initPoints,
				initPointsLength) {}
	};

	class TetraminoG final : public Tetramino
	{
	public:
		TetraminoG(SDL_Texture* texture, 
			SDL_Renderer* render,
			const SDL_Rect sourceRects,
			const SDL_Point* initPoints,
			const int initPointsLength) :
			Tetramino(TetraminoKind::g, 
				texture, 
				render,
				sourceRects,
				initPoints,
				initPointsLength) {}
	};

	class TetraminoRight final : public Tetramino
	{
	public:
		TetraminoRight(SDL_Texture* texture, 
			SDL_Renderer* render,
			const SDL_Rect sourceRects,
			const SDL_Point* initPoints,
			const int initPointsLength) :
			Tetramino(TetraminoKind::right, 
				texture, 
				render,
				sourceRects,
				initPoints,
				initPointsLength) {}
	};

	class TetraminoLeft final : public Tetramino
	{
	public:
		TetraminoLeft(SDL_Texture* texture,
			SDL_Renderer* render,
			const SDL_Rect sourceRects,
			const SDL_Point* initPoints,
			const int initPointsLength) :
			Tetramino(TetraminoKind::left, 
				texture, 
				render,
				sourceRects,
				initPoints,
				initPointsLength) {}
	};

	class TetraminoT final : public Tetramino
	{
	public:
		TetraminoT(SDL_Texture* texture, 
			SDL_Renderer* render,
			const SDL_Rect sourceRects,
			const SDL_Point* initPoints,
			const int initPointsLength) :
			Tetramino(TetraminoKind::t, 
				texture, 
				render,
				sourceRects,
				initPoints,
				initPointsLength) {}
	};

	//enum class WorkSpaceOperation {deletion, projection, all};

	class WorkSpace
	{
	private:
		texturePixel* workSpace{ nullptr };
		SDL_Renderer* render{ nullptr };
		int lowest{ 0 };
		int highest{ 0 };
		int lowvalue{ 0 };
		int valuesOfRows[ROOMHEIGHT_PIXELS]{};
		bool initOk{ true };
	public:
		WorkSpace(SDL_Renderer* render);
		WorkSpace(const WorkSpace&) = delete;
		WorkSpace& operator=(const WorkSpace&) = delete;
		WorkSpace(WorkSpace&&) = delete;
		~WorkSpace();
		bool InitOK() const { return initOk; }
		void WorkSpaceOperate(const texturePixel* realTetramino);
		texturePixel* GetWorkSpace() const { return workSpace; }
		void Show();
		bool CheckForCombo();
		void DoCombo();
		bool HasEnoughPlaceForNew(const texturePixel* realTetramino);
		void ClearValuesOfRows();

#ifdef LOGS
		friend std::ostream& operator<<(std::ostream& os, const WorkSpace& ws);
#endif

	};


}