#include "prog.h"

static int moveDuration{ 1000 };
static int freezeDeleting{ 500 };


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
			std::cerr << "Cannot create tetris prog, " << 
				"making workspace is damaged.\n";
#endif
			initOk = false; return;
		}

		currentTetramino = makeCurrentTetramino();
		if (!currentTetramino)
		{
			initOk = false; return;
		}

		runSession();
	}

	void Prog::runSession()
	{
#ifdef TIMER
		sessionTimer.start(moveDuration);
#endif

		while (sessionQuit == false)
		{
#ifdef TIMER
			if (sessionTimer.hasElapsed())
			{
				moveDown();
				sessionTimer.reset();
				sessionTimer.start(moveDuration);
			}
#endif

			checkPressedKeys();

			if (currentTetramino->Fixed())
			{

				projectTetraminoToVirtual();
				if (workSpace->CheckForCombo())
				{
//#ifdef TIMER
					pauseBeforeDeletionTimer.start(freezeDeleting);
					while(!pauseBeforeDeletionTimer.hasElapsed())
					{
						renderScreen();
					}
					pauseBeforeDeletionTimer.reset();
//#endif
					workSpace->DoCombo();
				}
				delete currentTetramino;
				currentTetramino = nullptr;
				currentTetramino = makeCurrentTetramino();
				if (!currentTetramino)
				{
					initOk = false; return;
				}
				if (!workSpace->HasEnoughPlaceForNew(currentTetramino->RealTetramino()))
				{
					sessionQuit = true;
					goto EXIT_WHILE;
				}

			}

			renderScreen();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	EXIT_WHILE:
		return;

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

	void Prog::moveDown()
	{
		if (!currentTetramino->Fixed())
			move(Direction::down);
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
					//std::cout << *currentTetramino;
					std::cout << *workSpace << std::endl;
					
#endif
					break;
				}
				case SDLK_SPACE:
				{
					move(Direction::drop);
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

	Tetramino* Prog::makeCurrentTetramino()
	{
//#ifdef LOGS
//		std::cout << "makeCurrentTetramino\n";
//#endif
#define int_ static_cast<int>
		int randomNumber = trandom.GetRandom();
		Tetramino* currentTetramino{ nullptr };
		switch (randomNumber)
		{
		case planeTetramino:
		{
			currentTetramino = new TetraminoPlane(storage->AllTextures()[int_(TexturesStorageNames::tetris)], tetrisRender,
				storage->RectStorage()[int_(TetrisTextureRects::plane)], planeInits, initsLength);
			if (currentTetramino->InitOk() == false) break;
			else return currentTetramino;
			break;
		}
		case cubeTetramino:
		{
			currentTetramino = new TetraminoCube(storage->AllTextures()[int_(TexturesStorageNames::tetris)], tetrisRender,
				storage->RectStorage()[int_(TetrisTextureRects::cube)], cubeInits, initsLength);
			if (currentTetramino->InitOk() == false) break;
			else return currentTetramino;
			break;
		}
		case lTetramino:
		{
			currentTetramino = new TetraminoL(storage->AllTextures()[int_(TexturesStorageNames::tetris)], tetrisRender,
				storage->RectStorage()[int_(TetrisTextureRects::l)], lInits, initsLength);
			if (currentTetramino->InitOk() == false) break;
			else return currentTetramino;
			break;
		}
		case gTetramino:
		{
			currentTetramino = new TetraminoG(storage->AllTextures()[int_(TexturesStorageNames::tetris)], tetrisRender,
				storage->RectStorage()[int_(TetrisTextureRects::g)], gInits, initsLength);
			if (currentTetramino->InitOk() == false) break;
			else return currentTetramino;
			break;
		}
		case rightTetramino:
		{
			currentTetramino = new TetraminoRight(storage->AllTextures()[int_(TexturesStorageNames::tetris)], tetrisRender,
				storage->RectStorage()[int_(TetrisTextureRects::right)], rightInits, initsLength);
			if (currentTetramino->InitOk() == false) break;
			else return currentTetramino;
			break;
		}
		case leftTetramino:
		{
			currentTetramino = new TetraminoLeft(storage->AllTextures()[int_(TexturesStorageNames::tetris)], tetrisRender,
				storage->RectStorage()[int_(TetrisTextureRects::left)], leftInits, initsLength);
			if (currentTetramino->InitOk() == false) break;
			else return currentTetramino;
			break;
		}
		case tTetramino:
		{
			currentTetramino = new TetraminoT(storage->AllTextures()[int_(TexturesStorageNames::tetris)], tetrisRender,
				storage->RectStorage()[int_(TetrisTextureRects::t)], tInits, initsLength);
			if (currentTetramino->InitOk() == false) break;
			else return currentTetramino;
			break;
		}
		}
		
#ifdef LOGS
		std::cerr << "Cannot make tetramino in progress, abort.\n";
#endif
		return nullptr;
		

#undef int_
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
		workSpace->WorkSpaceOperate(currentTetramino->RealTetramino());
			//WorkSpaceOperation::projection);

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