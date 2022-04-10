#include "player.h"



Player::Player()
{
	mPosX = 0;
	mPosY = 0;

	mVelX = 0;
	mVelY = 0;

	mCurVelX = 0;
	mCurVelY = 0;

	mWidth = 30;
	mHeight = 30;
	mActive = false;
}

Player::Player(std::string path, SDL_Renderer* rRend, int posx, int posy, Facing startdirection)
{
	mPosX = posx;
	mPosY = posy;

	mVelX = 0;
	mVelY = 0;

	mCurVelX = 0;
	mCurVelY = 0;

	mWidth = 30;
	mHeight = 30;

	tileHitX = 0;
	tileHitY = 0;

	checkX = true;

	mCircle.r = mWidth / 2;

	checkCircleCol = false;
	mActive = true;
	mControllable = true;
	mFailed = false;


	mDirectionUpDown = true;

	onGround = false;

	debugMovement = false;

	mDirCurLR = mDirectionLeftRight;
	mDirCurUD = mDirectionUpDown;
	mDirectionHorizontal = startdirection;
	if (mDirectionHorizontal == Facing::LEFT) {
		mFlipMode = SDL_FLIP_HORIZONTAL;
		mDirectionLeftRight = false;
	}
	else {
		mFlipMode = SDL_FLIP_NONE;
		mDirectionLeftRight = true;
	}

	//Init Texture

	tPlayerTexture = new Texture(path, rRend, (int)mPosX, (int)mPosY);
	if (mFlipMode != SDL_FLIP_NONE) { tPlayerTexture->setFlip(mFlipMode); }
}

Player::~Player()
{
	Free();
}

void Player::handleEvents(SDL_Event* eEvent)
{
	if (mActive && mControllable)
	{
		// if a key was pressed
		if (eEvent->type == SDL_KEYDOWN && eEvent->key.repeat == 0)
		{
			// adjusts velocity
			switch (eEvent->key.keysym.sym)
			{
			case SDLK_UP: mVelY -= mCurVelY; break;
			case SDLK_DOWN: mVelY += mCurVelY; break;
			case SDLK_LEFT: mVelX -= mCurVelX; break;
			case SDLK_RIGHT: mVelX += mCurVelX; break;
			}
		} // if a key was released
		else if (eEvent->type == SDL_KEYUP && eEvent->key.repeat == 0)
		{
			//adjust velocity
			switch (eEvent->key.keysym.sym)
			{
			case SDLK_UP: mVelY += mCurVelY; break;
			case SDLK_DOWN: mVelY -= mCurVelY; break;
			case SDLK_LEFT: mVelX += mCurVelX; break;
			case SDLK_RIGHT: mVelX -= mCurVelX; break;
			}
		}
	}
}

void Player::Update(std::vector<Tile*> pVTiles, std::vector<Bomb*> pVBombs, Finish* oFinish, Music* oMusic)
{
	if (mActive)
	{
		if (!debugMovement)
		{
			//Check if movement is currect
			if (mDirCurLR != mDirectionLeftRight) {
				mDirCurLR = mDirectionLeftRight;
				tPlayerTexture->setFlip(mFlipMode);
			}

			if (mDirectionLeftRight){ mVelX += mCurVelX; }
			else { mVelX -= mCurVelX; }

			if (!onGround) { mCurVelY += 0.05f; }
			if (onGround && mCurVelY > 0) { onGround = false; }
			mVelY += mCurVelY; 

			if (mCurVelX > 0) { mCurVelX *= 0.985f; }
			if (mCurVelX > 0 && mCurVelX < 0.01f) { mCurVelX = 0; }
		}

		if (mControllable)
		{
			if (checkCircleCol)
			{
				checkPlayerCircle(pVTiles);
			}
			else
			{
				checkPlayerRect(pVTiles, pVBombs, oFinish, oMusic);
			}
		}
	}

	//TODO If player object doesn't move anymore, set fail true

	mVelX = 0;
	mVelY = 0;
}

void Player::checkPlayerCircle(std::vector<Tile*> pVTiles)
{
	//Update this with currentlevel
	int levelWidth = 800;
	int levelHeight = 704;

	mPosX += mVelX;

	mCircle.x = mPosX;
	mCircle.y = mPosY;

	if (!debugMovement)
	{
		//Check if player circle collided with tile or went offscreen
		if ((mPosX - mCircle.r < 0) || (mPosX + mCircle.r > levelWidth) || checkTileCollision(mCircle, pVTiles)) {
			mPosX -= mVelX;
			mCircle.x = mPosX;
			mCircle.y = mPosY;
		}
	}

	mPosY += mVelY;

	mCircle.x = mPosX;
	mCircle.y = mPosY;

	if (!debugMovement)
	{
		if ((mPosY - mCircle.r < 0) || (mPosY + mCircle.r > levelHeight) || checkTileCollision(mCircle, pVTiles)) {
			mPosY -= mVelY;
			mCircle.x = mPosX;
			mCircle.y = mPosY;
		}
	}

}

void Player::checkPlayerRect(std::vector<Tile*> pVTiles, std::vector<Bomb*> pVBombs, Finish* oFinish, Music* oMusic)
{
	//Update this with currentlevel
	int levelWidth = 800;
	int levelHeight = 704;

	 mPosX += mVelX;

	SDL_Rect playerinfo;
	playerinfo.x = mPosX;
	playerinfo.y = mPosY;
	playerinfo.w = mWidth;
	playerinfo.h = mHeight;

	checkX = false;
	checkY = false;

	if (!debugMovement)
	{	//Check if player collided with objects or went offscreen
		if ((mPosX < 0) || (mPosX + mWidth > levelWidth) || checkTileCollision(playerinfo, pVTiles, oMusic) || checkBombCollision(playerinfo, pVBombs) || checkFinishCollision(playerinfo, oFinish)) { 
			mPosX -= mVelX;
			if (checkX) { 
				mDirectionLeftRight = !mDirectionLeftRight;
				if (!mDirectionLeftRight) { mFlipMode = SDL_FLIP_HORIZONTAL; }
				else { mFlipMode = SDL_FLIP_NONE; }
				oMusic->playSFX(oMusic->sBounce);
			}
		}
	}
	mPosY += mVelY;

	playerinfo.y = mPosY;
	if (!debugMovement)
	{   //Check if player collided with objects or went offscreen
		if ((mPosY < 0) || (mPosY + mHeight > levelHeight) || checkTileCollision(playerinfo, pVTiles, oMusic) || checkBombCollision(playerinfo, pVBombs) || checkFinishCollision(playerinfo, oFinish)) { 
			mPosY -= mVelY; 
			if (checkY) {
				if (tileHitY == 2 || tileHitY == 4)
				{
					mCurVelY *= 0.65f;
					if (mCurVelY > 0 && mCurVelY < 0.15f) {
						mCurVelY = 0;
						onGround = true;
					}
					else { mCurVelY = -mCurVelY; }
				    mDirectionUpDown = !mDirectionUpDown; 
					oMusic->playSFX(oMusic->sBounce);
				}
				//if (tileHitY == 0 && onGround) { onGround = false; }
			}
		}
	}

	//if (tileHitX != 0) { mDirectionLeftRight = !mDirectionLeftRight; tileHitX = 0;}
	//if (tileHitY != 0) { mDirectionUpDown = !mDirectionUpDown; }
	
	//tileHitY = 0;
}

void Player::Render(SDL_Renderer* rRend)
{
	if (mActive)
	{
		tPlayerTexture->Render(rRend, mPosX, mPosY);
	}
		
}


bool Player::checkTileCollision(SDL_Rect playerRect, std::vector<Tile*> pVTiles, Music* oMusic)
{
	//Loop through tileset and check collision with player, only if not a transparent sky tile
	for (int i = 0; i < pVTiles.size(); ++i)
	{
		if (pVTiles[i]->getActive()) {
			//If tile is a wall
			if ((pVTiles[i]->getType() >= 1) && (pVTiles[i]->getType() <= 9))
			{
				//check collision based on rects with eachother
				if (Entity::checkCollision(playerRect, pVTiles[i]->getColBox())) { 
					checkX = true;
					checkY = true;
					tileHitY = pVTiles[i]->getType();
					return true; 
				}
			}
		}
	}
	return false;
}

bool Player::checkBombCollision(SDL_Rect playerRect, std::vector<Bomb*> pVBombs)
{
	//Loop through tileset and check collision with player, only if not a transparent sky tile
	for (int i = 0; i < pVBombs.size(); ++i)
	{
		if (pVBombs[i]->getActive()) {
			//check collision based on rects with eachother
			if (Entity::checkCollision(playerRect, pVBombs[i]->getBoxCollider())) { 
				float xV = pVBombs[i]->getDamageVelX();
				float yV = pVBombs[i]->getDamageVelY();
				pVBombs[i]->setHit();
				addVelocity(xV, yV);
				return true; 
			}
		}
	}
	return false;
}

bool Player::checkFinishCollision(SDL_Rect playerRect, Finish* oFinish)
{
	//check finishhitbox
	//if false
	//check normal hitbox
	if (oFinish->getFinishActivated()) {
		if (Entity::checkCollision(playerRect, oFinish->getFinishHitbox())) {
			oFinish->setFinishTrigger(true);
		}
		else {
			if (Entity::checkCollision(playerRect, oFinish->getHitbox())) {
				return true;
			}
		}
	}
	return false;
}

bool Player::checkTileCollision(Circle playerCircle, std::vector<Tile*> pVTiles)
{
	//Loop through tileset and check collision with player, only if not a transparent sky tile
	for (int i = 0; i < pVTiles.size(); ++i)
	{
		if (pVTiles[i]->getActive()) {
			//If tile is a wall
			if ((pVTiles[i]->getType() >= 1) && (pVTiles[i]->getType() <= 9))
			{
				//check collision based on rects with eachother
				if (Entity::checkCollision(playerCircle, pVTiles[i]->getColBox())) { return true; }
			}
		}
	}
	return false;
}

void Player::addVelocity(float addingVelX, float addingVelY)
{
	mCurVelX += addingVelX;
	mCurVelY += addingVelY;
}

void Player::setPosX(int newposx)
{
	mPosX = newposx;
}

void Player::setPosY(int newposy)
{
	mPosY = newposy;
}

void Player::setFlip(SDL_RendererFlip newFlip) { tPlayerTexture->setFlip(newFlip); }

float Player::getFPosX() { return mPosX; }
float Player::getFPosY() { return mPosY; }

SDL_RendererFlip Player::getFlip() { return tPlayerTexture->getFlip(); }

void Player::setControllable(bool state) { if (state) { mControllable = true; } else { mControllable = false; } }


bool Player::getFail() { return mFailed; }

void Player::setDebugMode(bool state) 
{ 
	if (state) { 
		debugMovement = true;	
		mCurVelX = 1;
		mCurVelY = 1;
	} else { 
		debugMovement = false; 
		mCurVelX = 0;
		mCurVelY = 0;
	} 
}

void Player::Free()
{
	delete tPlayerTexture;
	tPlayerTexture = nullptr;
	mPosX = 0;
	mPosY = 0;
	mVelX = 0;
	mVelY = 0;
	mCurVelX = 0;
	mCurVelY = 0;
	mWidth = 0;
	mHeight = 0;
	tileHitX = 0;
	tileHitY = 0;
	mCircle.r = 0;
	mCircle.x = 0;
	mCircle.y = 0;
	checkCircleCol = false;
	checkX = false;
	checkY = false;
	mActive = false;
	mControllable = false;

	//Init Texture
}



Player::Facing Player::getFacing()
{
	return mDirectionHorizontal;
}