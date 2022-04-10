#pragma once

#ifndef BOMB_H
#define BOMB_H

#include <SDL.h>
#include "entity.h"
#include "texture.h"
#include "music.h"

class Bomb: public Entity
{
public:
	Bomb();
	Bomb(SDL_Renderer* rRend, int posx, int posy, int bombtype);
	Bomb(std::string path, SDL_Renderer* rRend, int posx, int posy, int bombtype);
	~Bomb();

	void handleEvents(SDL_Event* eEvent, bool removing, Music* oMusic);

	void Update(Music* oMusic);
	void Render(SDL_Renderer* rRend, Texture* bombTexture);

	Circle& getCircleCollider();
	SDL_Rect getBoxCollider();

	float getDamageVelX();
	float getDamageVelY();

	void setHit();

	bool getRemove();

	int getBombType();
	int getPosX();
	int getPosY();

	void Free();

private:
	Circle mCircle;
	SDL_Rect mBox;
	//Texture* tBombTexture;

	int mPosX;
	int mPosY;
	int mBombWidth;
	int mBombHeight;
	int mBombType;
	int firstframescount;

	float mDamageVelX;
	float mDamageVelY;

	bool mHandleMouseEvents;
	bool mExploding;
	
	bool mInteractable;
	bool mFirstTimeUse;
	bool mRemoving;

	bool checkMousePos();
};

#endif
