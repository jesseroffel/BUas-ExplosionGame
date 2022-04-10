#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <iostream>
#include "texture.h"
#include "music.h"
#include "text.h"

class Button
{
public:
	enum BTYPE {
		bNothing,
		bGoToAction,
		bGoToEditor,
		bDelete,
		bPlace,
		bEntity,
	};

	enum ETYPE {
		eNothing,
		eBombHorizontal,
		eBombVertical
	};

	Button();
	//Constructor with a position as arguments
	Button(std::string path, SDL_Renderer* rRend, TTF_Font* FONT, int posX, int posY, BTYPE ButtonType);
	Button(std::string path, SDL_Renderer* rRend, int posX, int posY, BTYPE ButtonType, ETYPE EntityType);

	//Constructor with a position and button width/height arguments
	~Button();

	//Set topleft position
	void setPosition(int x, int y);

	//Handle mouse events
	void handleEvents(SDL_Event* eEvent, Music* oMusic);

	//Render button on screen
	void Render(SDL_Renderer* rRend);

	int getReturnType();
	int getEntityType();

	int getEntity();

	void setInteractable(bool state);
	bool getInteractable();
	void setAlpha(Uint8 newalpha);

	void Free();

private:
	Texture* tButtonIcon;
	Texture* tButtonTexture;
	Texture* tButtonEntityCount;
	Uint8 tTextureAlpha;

	BTYPE mButtonType;						
	ETYPE mEntityType;

	Text* txtInfo;

	int mX;
	int mY;

	int mPosX;
	int mPosY;
	int mButtonWidth;
	int mButtonHeight;

	int mButtonIconPosX;
	int mButtonIconPosY;
	int mButtonIconWidth;
	int mButtonIconHeight;

	//int mButtonType;
	//int mEntityType;

	bool drawBorder;
	bool drawButton;

	bool mInteractable;
	bool interacted;
	bool curInteraction;
	bool renderOutlines;

	bool playsfx = true;
	bool renderText = false;

	int mReturnButtonType;
	int mReturnEntityType;

	bool checkMousePos();

	void setText(SDL_Renderer* rRend, TTF_Font* FONT);

	//int getButtonType();
	//int getEntityType();
};

#endif