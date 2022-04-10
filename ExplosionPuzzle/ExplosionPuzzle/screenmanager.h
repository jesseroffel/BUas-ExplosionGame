#pragma once

#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include "level.h"

//Class that was supposed to be a menu manager, sadly in this version I haven't created a menu yet
//Each menu uses it's own class
//TODO Make a little menu

class ScreenManager
{
public:
	ScreenManager();
	ScreenManager(SDL_Renderer* rRend);
	~ScreenManager();

	enum gameState {
		sTitleScreen,
		sMainMenu,
		sAbout,
		sGame
	};

	gameState currentState;


	void Update();

	void Input(SDL_Event* eEvent);

	void Render(SDL_Renderer* rRend);

	int getState();

	void setState(gameState newstate);

	bool getCloseGame() { return closeGame; }

	//Get the Level object reference
	static Level* getLevel();
private:
	static Level* oLevel;
	bool closeGame = false;
};

#endif