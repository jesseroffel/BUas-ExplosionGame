#include "screenmanager.h"

Level* ScreenManager::oLevel = new Level();

ScreenManager::ScreenManager() {}

ScreenManager::ScreenManager(SDL_Renderer* rRend)
{
	//Init Level Object
	oLevel = new Level(rRend);
	currentState = gameState::sGame;
	closeGame = false;
}


ScreenManager::~ScreenManager()
{
	delete oLevel;
}

void ScreenManager::Input(SDL_Event* eEvent)
{
	oLevel->Input(eEvent);
}

void ScreenManager::Update()
{
	switch (currentState) 
	{
	case sGame:
		oLevel->Update();
		if (oLevel->getCloseGame()) { 
			closeGame = true;
		}
		break;
	}
}


void ScreenManager::Render(SDL_Renderer* rRend)
{
	switch (currentState)
	{
	case sGame:
		oLevel->Render(rRend);
		break;
	}
}

int ScreenManager::getState()
{
	return currentState;
}

void ScreenManager::setState(gameState newstate)
{
	currentState = newstate;
}

Level* ScreenManager::getLevel()
{
	return oLevel;
}