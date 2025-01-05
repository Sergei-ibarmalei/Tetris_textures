#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "logs.h"

#ifdef LOGS
#include <iostream>
#endif


namespace tetris
{
	class Sdl
	{
	private:
		SDL_Window* window{ nullptr };
		SDL_Renderer* render{ nullptr };
		//SDL_Event e;
		//SDL_Texture* tetrisTexture{ nullptr };

		void makeTexture();
		bool initOk{ true };
	public:
		Sdl();
		~Sdl();
		Sdl(const Sdl&) = delete;
		Sdl& operator=(const Sdl&) = delete;
		Sdl(Sdl&&) = delete;

		SDL_Renderer* Render() { return render; }
		//SDL_Event*	  Event() { return &e; }
		bool InitOk() const { return initOk; }
		//SDL_Texture* TetrisTexture() const { return tetrisTexture; }

	};


}