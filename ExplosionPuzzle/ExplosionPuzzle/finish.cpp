#include "finish.h"



Finish::Finish()
{
}

Finish::Finish(SDL_Renderer* rRend, int posx, int posy, FTYPE finishtype)
{
	mPosX = posx;
	mPosY = posy;
	mType = finishtype;

	mActive = true;
	finishTriggered = false;
	//TODO Finish activated after collectable
	finishActivated = true;

	renderFinishHitbox = false;

	//Finish type 1, small hitbox on top
	//std::string filename = "";
	switch (finishtype)
	{
		case 1:
			mHitBox.x = mPosX;
			mHitBox.y = mPosY + 14;
			mHitBox.w = 32;
			mHitBox.h = 16;

			mFinishBox.w = 28;
			mFinishBox.h = 4;
			mFinishBox.x = mHitBox.x + 2;
			mFinishBox.y = mHitBox.y - mFinishBox.h;
			//filename = "finish.png";
			break;
		case 2:
			mHitBox.x = mPosX + 2;
			mHitBox.y = mPosY;
			mHitBox.w = 16;
			mHitBox.h = 32;

			mFinishBox.w = 4;
			mFinishBox.h = 28;
			mFinishBox.x = mHitBox.x + 14;
			mFinishBox.y = mHitBox.y + 2;
			//filename = "finish_left.png";
			break;
		case 3:
			mHitBox.x = mPosX + 14;
			mHitBox.y = mPosY;
			mHitBox.w = 16;
			mHitBox.h = 32;

			mFinishBox.w = 4;
			mFinishBox.h = 28;
			mFinishBox.x = mHitBox.x - 2;
			mFinishBox.y = mHitBox.y + 2;
			//filename = "finish_right.png";
			break;
		case 4:
			mHitBox.x = mPosX;
			mHitBox.y = mPosY + 2;
			mHitBox.w = 32;
			mHitBox.h = 16;

			mFinishBox.w = 28;
			mFinishBox.h = 4;
			mFinishBox.x = mHitBox.x + 2;
			mFinishBox.y = mHitBox.y + 14;
			//filename = "finish_up.png";
			break;
	}

	mWidth = mHitBox.w;
	mHeight = mHitBox.h;

	//tFinishTexture = new Texture(filename, rRend, mPosX, mPosY);
}


Finish::~Finish()
{
	//delete tFinishTexture;
}


void Finish::Render(SDL_Renderer* rRend, Texture* FinishTexture, SDL_Rect FinishClip)
{
	if (mActive) { FinishTexture->Render(rRend, mPosX, mPosY, &FinishClip); }
	if (renderFinishHitbox)
	{
		//Render (cyan) outline for debugging if true
		SDL_SetRenderDrawColor(rRend, 0xFF, 0x00, 0xFF, 0xFF);
		SDL_RenderDrawRect(rRend, &mFinishBox);
		SDL_SetRenderDrawColor(rRend, 0x00, 0x00, 0x00, 0xFF);

		SDL_SetRenderDrawColor(rRend, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(rRend, &mHitBox);
		SDL_SetRenderDrawColor(rRend, 0x00, 0x00, 0x00, 0xFF);
	}
}


SDL_Rect Finish::getHitbox() { return mHitBox; }
SDL_Rect Finish::getFinishHitbox() { return mFinishBox; }

bool Finish::getFinishTrigger() { return finishTriggered; }

void Finish::setFinishTrigger(bool state) { if (state) { finishTriggered = true; } else { finishTriggered = false; } }

bool Finish::getFinishActivated() { return finishActivated; }

void Finish::setFinishActivated(bool state) { if (state) { finishActivated = true; } else { finishActivated = false; } }

int Finish::getPosX() { return mPosX; }
int Finish::getPosY() { return mPosY; }

Finish::FTYPE Finish::getFinishType() { return mType; }