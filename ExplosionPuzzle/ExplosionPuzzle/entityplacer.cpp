#include "entityplacer.h"



EntityPlacer::EntityPlacer()
{
}

EntityPlacer::EntityPlacer(SDL_Renderer* rRend, int posx, int posy, int entityType)
{
	mMiddleMouseCorrection = 16;
	mPosX = posx - mMiddleMouseCorrection;
	mPosY = posy - mMiddleMouseCorrection;
	mEntityWidth = 32;
	mEntityHeight = 32;
	mEntityType = entityType;

	renderOutlines = true;
	mActive = true;

	std::string filename = "";
	switch (mEntityType) {
		case 1:
			filename = "bombx.png";
			break;
		case 2:
			filename = "bomby.png";
			break;
		case 3:
			filename = "bbomb.png";
			break;
	}
	if (filename != "") { tEntityTexture = new Texture(filename, rRend, mPosX, mPosY); }
	tTextureAlpha = 155;
	tEntityTexture->setAlpha(tTextureAlpha);
}


EntityPlacer::~EntityPlacer()
{
	delete tEntityTexture;
	//mActive = false;
}

void EntityPlacer::Update(int posx, int posy)
{
	mPosX = posx - mMiddleMouseCorrection;
	mPosY = posy - mMiddleMouseCorrection;
}

void EntityPlacer::Render(SDL_Renderer* rRend)
{
	tEntityTexture->Render(rRend, mPosX, mPosY);
	if (renderOutlines) {
		SDL_Rect renderQuad = { mPosX, mPosY, mEntityWidth, mEntityHeight };
		//Render (cyan) outline for debugging if true
		SDL_SetRenderDrawColor(rRend, 0xFF, 0x00, 0xFF, 0xFF);
		SDL_RenderDrawRect(rRend, &renderQuad);
		SDL_SetRenderDrawColor(rRend, 0x00, 0x00, 0x00, 0xFF);
	}
	
}

void EntityPlacer::Render(SDL_Renderer* rRend, Texture* TileTexture, SDL_Rect TileClip)
{

}

int EntityPlacer::getEntityType() { return mEntityType; }

int EntityPlacer::getPosX() { return mPosX; }

int EntityPlacer::getPosY() { return mPosY; }

bool EntityPlacer::checkIfCollisionTiles(SDL_Rect entityRect, std::vector<Tile*> pVTiles)
{
	for (int i = 0; i < pVTiles.size(); ++i)
	{
		if (pVTiles[i]->getActive() && (pVTiles[i]->getType() != 0)) {
			//If tile is a wall
			if ((pVTiles[i]->getType() >= 1) && (pVTiles[i]->getType() <= 9))
			{
				//check collision based on rects with eachother
				if (Entity::checkCollision(entityRect, pVTiles[i]->getColBox())) {
					return true;
				}
			}
		}
	}
	return false;
}