#pragma once

#ifndef ENTITYPLACER_H
#define ENTITYPLACER_H

#include <SDL/SDL.h>
#include <string>
#include <vector>
#include "entity.h"
#include "texture.h"
#include "tile.h"

class EntityPlacer: public Entity
{
public:
	EntityPlacer();
	~EntityPlacer();

	EntityPlacer(SDL_Renderer* rRend, int posx, int posy, int entityType);

	void Update(int posx, int posy);

	void Render(SDL_Renderer* rRend);
	void Render(SDL_Renderer* rRend, Texture* TileTexture, SDL_Rect TileClip);

	bool checkIfCollisionTiles(SDL_Rect entityRect, std::vector<Tile*> pVTiles);

	int getEntityType();
	int getPosX();
	int getPosY();
private:

	Texture* tEntityTexture;

	int mPosX;
	int mPosY;
	int mMiddleMouseCorrection;
	int mEntityWidth;
	int mEntityHeight;
	int mEntityType;

	bool renderOutlines;

	Uint8 tTextureAlpha;
};

#endif