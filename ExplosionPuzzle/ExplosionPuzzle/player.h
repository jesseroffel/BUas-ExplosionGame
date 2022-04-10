#pragma once

#ifndef PLAYER_H
#define PLAYER_H

#include <SDL/SDL.h>
#include <vector>
#include "entity.h"

#include "tile.h"
#include "bomb.h"
#include "finish.h"



class Player: public Entity
{
public:

	enum Facing {
		LEFT,
		RIGHT
	};

	Player();
	Player(std::string path, SDL_Renderer* rRend, int posx, int posy, Facing startdirection);
	~Player();

	//For debugging, freely move the player
	void handleEvents(SDL_Event* eEvent);

	void Update(std::vector<Tile*> pVTiles, std::vector<Bomb*> pVBombs, Finish* oFinish, Music* oMusic);
	void Render(SDL_Renderer* rRend);

	//void move(SDL_Rect* square, Circle* circle);

	//Circle& getCollider();
	//int getPosX();
	//int getPosY();
	//int getVelX();
	//int getVelY();

	void setPosX(int newposx);
	void setPosY(int newposy);
	float getFPosX();
	float getFPosY();
	void setControllable(bool state);
	void setFlip(SDL_RendererFlip newFlip);
	SDL_RendererFlip getFlip();
	bool getFail();
	Facing getFacing();
	void setDebugMode(bool state);

	void Free();

private:
	Facing mDirectionHorizontal;
	//Final position
	float mPosX;
	float mPosY;

	//Final velocity
	float mVelX;
	float mVelY;

	//Adding velocity
	float mCurVelX;
	float mCurVelY;

	int mWidth;
	int mHeight;

	int tileHitX;
	int tileHitY;
	bool checkX;							// X axis collision return value
	bool checkY;							// Y axis collision return value 
	bool onGround;							// Check if player is on ground

	bool checkCircleCol;					// Use circle collision check
	bool mControllable;

	bool mFailed;							// Player failed to complete the game

	//LEFT = FALSE; RIGHT = TRUE;
	//UP   = FALSE; DOWN  = TRUE;    
	bool mDirectionLeftRight;
	bool mDirCurLR;
	bool mDirectionUpDown;
	bool mDirCurUD;

	bool debugMovement;

	SDL_Rect mBox;
	Circle mCircle;
	Texture* tPlayerTexture;
	SDL_RendererFlip mFlipMode;

	//Update collision check for movement
	void checkPlayerCircle(std::vector<Tile*> pVTiles);
	void checkPlayerRect(std::vector<Tile*> pVTiles, std::vector<Bomb*> pVBombs, Finish* oFinish, Music* oMusic);
	//Collision check player/tiles
	bool checkTileCollision(SDL_Rect playerRect, std::vector<Tile*> pVTiles, Music* oMusic);
	bool checkBombCollision(SDL_Rect playerRect, std::vector<Bomb*> pVBombs);
	bool checkFinishCollision(SDL_Rect playerRect, Finish* oFinish);
	bool checkTileCollision(Circle playerCircle, std::vector<Tile*> pVTiles);

	void addVelocity(float addingVelX, float addingVelY);

};

#endif
