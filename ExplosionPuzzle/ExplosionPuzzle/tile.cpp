#include "tile.h"


Tile::Tile()
{
}

Tile::Tile(SDL_Renderer* rRend, int posX, int posY, int tileType)
{
	//Set tile position
	mBox.x = posX;
	mBox.y = posY;

	//Set collisionbox
	mBox.w = TILE_WIDTH;
	mBox.h = TILE_HEIGHT;

	//Set tile width
	mTileWidth = 32;
	mTileHeight = 32;

	//Set tile type
	mType = tileType;

	renderOutlines = false;
	mActive = true;
	//If tile is the sky tile, set active false
	if (mType == 0) { mActive = false; }
}

Tile::~Tile()
{
}


void Tile::Render(SDL_Renderer* rRend, Texture* TileTexture)
{
	//TileTexture->Render(rRend, mBox.x, mBox.y, rRenderbox);
}

void Tile::Render(SDL_Renderer* rRend, Texture* TileTexture, SDL_Rect TileClip)
{
	if (mActive)
	{
		TileTexture->Render(rRend, mBox.x, mBox.y, TileTexture, &TileClip);
		if (renderOutlines)
		{
			SDL_Rect renderQuad = { mBox.x, mBox.y, TILE_WIDTH, TILE_WIDTH };
			//Render (blue) outline for debugging if true
			SDL_SetRenderDrawColor(rRend, 0x00, 0x00, 0xFF, 0xFF);
			SDL_RenderDrawRect(rRend, &renderQuad);
			SDL_SetRenderDrawColor(rRend, 0x00, 0x00, 0x00, 0xFF);
		}
	}
}

SDL_Rect Tile::getColBox()
{
	return mBox;
}

int Tile::getType()
{
	return mType;
}

int Tile::getTileWidth()
{
	return mTileWidth;
}

int Tile::getTileHeight()
{
	return mTileHeight;
}

bool Tile::getActive()
{
	return mActive;
}