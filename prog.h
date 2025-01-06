#pragma once
#include <thread>
#include "storages.h"
#include "trandom.h"
#include "timer.h"


namespace tetris
{
	class Prog
	{
	private:
		Storage* storage{ nullptr };
		TetrisRoom* tetrisRoom{ nullptr };
		Tetramino* currentTetramino{ nullptr };

		WorkSpace* workSpace{ nullptr };

		SDL_Renderer* tetrisRender{ nullptr };
		SDL_Event e;
		TRandom trandom;
		Timer sessionTimer;
		Timer pauseBeforeDeletionTimer;

		bool initOk{ true };
		bool flowQuit{ false };
		bool sessionQuit{ false };

		
		void projectTetraminoToVirtual();
		void runSession();
		void checkPressedKeys();
		void renderScreen();
		void move(Direction dir);
		void rotateClockWise();
		void rotateCounterClockWise();
		void moveDown();
		Tetramino* makeCurrentTetramino();


	public:
		explicit Prog(SDL_Renderer* render);
		Prog(const Prog&) = delete;
		Prog& operator=(const Prog&) = delete;
		Prog(Prog&&) = delete;
		~Prog();
		bool InitOk() const { return initOk; }



	};
}