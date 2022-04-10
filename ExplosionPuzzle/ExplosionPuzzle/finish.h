#pragma once

#ifndef FINISH_H
#define FINISH_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <iostream>
#include "entity.h"
#include "texture.h"

class Finish : public Entity
{
public:
	//From what point does the player need to enter to finish
	enum FTYPE {
		fNothing,
		fAbove,
		fLeft,
		fRight,
		fBelow
	};

	Finish();
	Finish(SDL_Renderer* rRend, int posx, int posy, FTYPE finishtype);
	~Finish();

	void Render(SDL_Renderer* rRend, Texture* FinishTexture, SDL_Rect FinishClip);

	SDL_Rect getHitbox();
	SDL_Rect getFinishHitbox();

	int getPosX();
	int getPosY();

	FTYPE getFinishType();

	bool getFinishTrigger();
	void setFinishTrigger(bool state);
	bool getFinishActivated();
	void setFinishActivated(bool state);

private:
	//Texture* tFinishTexture;
	FTYPE mType;
	int mPosX;
	int mPosY;
	int mWidth;
	int mHeight;
	//int mType;

	bool finishTriggered;
	bool finishActivated;
	bool renderFinishHitbox;

	SDL_Rect mHitBox;
	SDL_Rect mFinishBox;
};

#endif