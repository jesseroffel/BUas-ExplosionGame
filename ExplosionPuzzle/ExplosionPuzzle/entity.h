#pragma once

#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <string>
#include "texture.h"

struct Circle
{
	int x, y;
	int r;
};


class Entity
{
public:
	Entity();
	Entity(std::string path, SDL_Renderer* rRend);
	~Entity();
	virtual void Update();
	virtual void Render(SDL_Renderer* rRend);

	bool checkCollision(SDL_Rect a, SDL_Rect b);
	bool checkCollision(Circle& a, SDL_Rect b);
	bool checkCollision(Circle& a, Circle& b);

	double distanceSquared(int x1, int y1, int x2, int y2);

	virtual bool getActive();

	virtual int getPosX();
	virtual int getPosY();

private:
	float xPos, yPos;
	int mPosX;
	int mPosY;
	Texture tEntityTexture;
protected:
	bool mActive;
};

#endif
