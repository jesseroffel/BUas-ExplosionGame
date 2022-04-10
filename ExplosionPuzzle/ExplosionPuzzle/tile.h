#pragma once

#ifndef TILE_H
#define TILE_H

#include <SDL/SDL.h>
#include "texture.h"

const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;

class Tile
{
public:
	Tile();
	Tile(SDL_Renderer* rRend, int posX, int posY, int tileType);
	~Tile();

	void Render(SDL_Renderer* rRend, Texture* TileTexture);
	void Render(SDL_Renderer* rRend, Texture* TileTexture, SDL_Rect TileClip);

	SDL_Rect getColBox();
	int getType();
	int getTileWidth();
	int getTileHeight();
	bool getActive();
	
private:
	//Position and width/height box
	SDL_Rect mBox;
	//Render clip
	//SDL_Rect rRenderbox;
	int mTileWidth;
	int mTileHeight;
	int mType;
	bool mActive;

	//Debug lines
	bool renderOutlines;
	
};
#endif

