#include "prog.h"

namespace tetris
{
	Prog::Prog(SDL_Renderer* render)
	{
		if (!render)
		{
#ifdef LOGS
			std::cerr << "Cannot create tetris prog, render is absent.\n";
#endif
			initOk = false; return;
		}

		tetrisRender = render;
		storage = new Storage(render);
		if (storage->InitOk() == false)
		{
#ifdef LOGS
			std::cerr << "Cannot create tetris prog, storage is damaged.\n";
#endif
			initOk = false; return;
		}
		tetrisRoom = new TetrisRoom(storage->operator[](TetrisTextureRects::room),
			storage->operator[](TetrisTextureRects::wall),
			storage->operator[](TexturesStorageNames::tetris),
			tetrisRender);
		if (tetrisRoom->InitOk() == false)
		{
#ifdef LOGS
			std::cerr << "Cannot create tetris prog, tetris room is damaged.\n";
#endif
			initOk = false; return;
		}

		workSpace = new WorkSpace(tetrisRender);
		if (!workSpace || workSpace->InitOK() == false)
		{
#ifdef LOGS
			std::cerr << "Cannot create tetris prog, making workspace is damaged.\n";
#endif
			initOk = false; return;
		}
		if (!makeTetramino())
		{
#ifdef LOGS
			std::cerr << "Cannot make tetramino, abort.\n";
#endif
			initOk = false; return;
		}

		runSession();
	}

	void Prog::runSession()
	{
		while (sessionQuit == false)
		{
			checkPressedKeys();
			renderScreen();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	void Prog::renderScreen()
	{
		SDL_SetRenderDrawColor(tetrisRender, 0u, 0u, 0u, 0xffu);
		SDL_RenderClear(tetrisRender);
		tetrisRoom->Show();
		currentTetramino->Show();
		workSpace->Show();
		SDL_RenderPresent(tetrisRender);
	}

	void Prog::checkPressedKeys()
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT) sessionQuit = true;
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_p:
				{
#ifdef LOGS	
					std::cout << *currentTetramino;
					
#endif
					break;
				}
				case SDLK_d:
				{
					if (currentTetramino->tetraminoKind == TetraminoKind::cube ||
						currentTetramino->Fixed())
						break;
					rotateClockWise();
					break;
				}
				case SDLK_a:
				{
					if (currentTetramino->tetraminoKind == TetraminoKind::cube ||
						currentTetramino->Fixed())
						break;
					rotateCounterClockWise();
					break;
				}
				case SDLK_RIGHT:
				{
					move(Direction::right);
					break;
				}
				case SDLK_LEFT:
				{
					move(Direction::left);
					break;
				}
				case SDLK_DOWN:
				{
					move(Direction::down);
					break;
				}
				default:
					break;
				}
			}
		}
	}



	bool Prog::makeTetramino()
	{
		int randomNumber = trandom.GetRandom();
		bool success{ true };
		//currentTetramino =
			//static_cast<TetraminoT*>(storage->operator[](TetraminoKind::t));
		switch (randomNumber)
		{
		case planeTetramino:
		{
			currentTetramino =
				static_cast<TetraminoPlane*>(storage->operator[](TetraminoKind::plane));
			break;
		}
		case cubeTetramino:
		{
			currentTetramino =
				static_cast<TetraminoCube*>(storage->operator[](TetraminoKind::cube));
			break;
		}
		case lTetramino:
		{	currentTetramino =
			static_cast<TetraminoL*>(storage->operator[](TetraminoKind::l));
			break;
		}
		case gTetramino:
		{
			currentTetramino =
				static_cast<TetraminoG*>(storage->operator[](TetraminoKind::g));
			break;
		}
		case rightTetramino:
		{
			currentTetramino =
				static_cast<TetraminoRight*>(storage->operator[](TetraminoKind::right));
			break;
		}
		case leftTetramino:
		{
			currentTetramino =
				static_cast<TetraminoLeft*>(storage->operator[](TetraminoKind::left));
			break;
		}
		case tTetramino:
		{
			currentTetramino =
				static_cast<TetraminoT*>(storage->operator[](TetraminoKind::t));
			break;
		}
		default:
			break;
		}
		if (!currentTetramino || currentTetramino->InitOk() == false)
		{
#ifdef LOGS
			std::cerr << "Fail by making tetramino, abort.\n";
#endif
			success = false;
		}
		return success;
	}

	void Prog::move(Direction dir)
	{
		currentTetramino->Move(dir, workSpace->GetWorkSpace());
	}



	void Prog::rotateClockWise()
	{
		currentTetramino->RotateClockWise(workSpace->GetWorkSpace());
	}

	void Prog::rotateCounterClockWise()
	{
		currentTetramino->RotateCounterClockWise(workSpace->GetWorkSpace());
	}

	void Prog::projectTetraminoToVirtual()
	{

	}


	Prog::~Prog()
	{
		delete storage;
		storage = nullptr;
		delete tetrisRoom;
		tetrisRoom = nullptr;
		tetrisRender = nullptr;
		delete currentTetramino;
		currentTetramino = nullptr;
	}
}