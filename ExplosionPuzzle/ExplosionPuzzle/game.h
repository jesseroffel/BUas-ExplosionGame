#pragma once

#ifndef GAME_H
#define GAME_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

//#include <vld.h> 

#include "screenmanager.h"

class Game
{
public:
	Game();
	~Game();

	unsigned long lFPSTime;

	void MainLoop();

	void Update();

	void Render(SDL_Renderer* rRend);

	void Close();

private:
	SDL_Window* wWindow;
	SDL_Renderer* rRenderer;

	static ScreenManager* oScreenManager;
	
	const int GAME_WIDTH = 800;
	const int GAME_HEIGHT = 800;

	bool closeGameCommand;

	void Input();
	void InputMenu();

};


#endif