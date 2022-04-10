#include "bomb.h"



Bomb::Bomb()
{
	mHandleMouseEvents = false;
}

Bomb::Bomb(SDL_Renderer* rRend, int posx, int posy, int bombtype)
{
	//Init Texture
	mBombType = bombtype;
	mPosX = posx;
	mPosY = posy;
	mBombWidth = 32;
	mBombHeight = 32;
	mBox.x = mPosX;
	mBox.y = mPosY;
	mBox.w = mBombWidth;
	mBox.h = mBombHeight;
	mCircle.x = mPosX;
	mCircle.y = mPosY;
	mCircle.r = mPosX / 2;
	mActive = true;
	mExploding = false;
	mHandleMouseEvents = true;
	mFirstTimeUse = true;
	std::string filename = "";

	switch (mBombType) {
	case 1:
		mDamageVelX = 10.0f;
		mDamageVelY = 0.0f;
		//filename = "bombx.png";
		break;
	case 2:
		mDamageVelX = 0.0f;
		mDamageVelY = -5.0f;
		//filename = "bomby.png";
		break;
	default:
		mDamageVelX = 10.0f;
		mDamageVelY = -5.0f;
		//filename = "bbomb.png";
		break;
	}
	//tBombTexture = new Texture(filename, rRend, posx, posx);
}


Bomb::Bomb(std::string path, SDL_Renderer* rRend, int posx, int posy, int bombtype)
{
	//Init Texture
	mBombType = bombtype;
	mPosX = posx;
	mPosY = posy;
	mBombWidth = 32;
	mBombHeight = 32;
	mBox.x = mPosX;
	mBox.y = mPosY;
	mBox.w = mBombWidth;
	mBox.h = mBombHeight;
	mCircle.x = mPosX;
	mCircle.y = mPosY;
	mCircle.r = mPosX / 2;
	mActive = true;
	mExploding = false;
	mHandleMouseEvents = true;
	mFirstTimeUse = true;
	switch (mBombType) {
	case 1:
		mDamageVelX = 2.5f;
		mDamageVelY = 0.0f;
		break;
	case 2:
		mDamageVelX = 0.0f;
		mDamageVelY = 2.5f;
		break;
	default:
		mDamageVelX = 0.0f;
		mDamageVelY = 0.0f;
		break;
	}
	//tBombTexture = new Texture(path, rRend, posx, posx);
}

Bomb::~Bomb()
{
	Free();
}

void Bomb::handleEvents(SDL_Event* eEvent, bool removing, Music* oMusic)
{
	if (mActive)
	{
		if (mHandleMouseEvents && mInteractable) {
			//Check if outside/inside for highlighting
			switch (eEvent->type)
			{
				case SDL_MOUSEBUTTONUP:
					bool checkpos = checkMousePos();
					if (checkpos) {
						if (removing) {
							mRemoving = true;
							oMusic->playSFX(oMusic->sExplosion);
							break;
						}
					}
			}
		}
	}
}

bool Bomb::checkMousePos()
{
	int mX = 0;
	int mY = 0;
	SDL_GetMouseState(&mX, &mY);

	bool inside = true;

	//mouse is left of the button
	if (mX < mPosX) { inside = false; }
	//mouse is right of the button
	else if (mX > mPosX + mBombWidth) { inside = false; }
	//mouse is above button
	else if (mY < mPosY) { inside = false; }
	//mouse is below button
	else if (mY > mPosY + mBombHeight) { inside = false; }
	
	return inside;
}

void Bomb::Update(Music* oMusic)
{
	if (mExploding) {
		std::cout << "Bomb Exploded!" << std::endl;
		mExploding = false;
		mActive = false;
		oMusic->playSFX(oMusic->sExplosion);
	}
	if (mFirstTimeUse) { 
		if (firstframescount > 9) { mInteractable = true; firstframescount = 0; }
		else { firstframescount++; }

	}
}

void Bomb::Render(SDL_Renderer* rRend, Texture* bombTexture)
{
	if (mActive) { bombTexture->Render(rRend, mPosX, mPosY); }
}

void Bomb::setHit() {
	mExploding = true;
}

int Bomb::getBombType() { return mBombType; }

int Bomb::getPosX() { return mPosX;  }

int Bomb::getPosY() { return mPosY; }

bool Bomb::getRemove() { return mRemoving; }

Circle& Bomb::getCircleCollider() { return mCircle; }

SDL_Rect Bomb::getBoxCollider() { return mBox; }

float Bomb::getDamageVelX() { return mDamageVelX; }

float Bomb::getDamageVelY() { return mDamageVelY; }

void Bomb::Free()
{
	//delete tBombTexture;
	//tBombTexture = nullptr;
	mBombType = 0;
	mPosX = 0;
	mPosY = 0;
	mBombWidth = 0;
	mBombHeight = 0;
	mBox.x = 0;
	mBox.y = 0;
	mBox.w = 0;
	mBox.h = 0;
	mCircle.x = 0;
	mCircle.y = 0;
	mCircle.r = 0;
	mActive = false;
	mExploding = false;
	mHandleMouseEvents = false;
	std::string filename = "";
}