#include "texture.h"

Texture::Texture() {}

Texture::Texture(std::string path, SDL_Renderer* rRend)
{
	tTexture = nullptr;
	mPosX = 0;
	mPosY = 0;
	mWidth = 0;
	mHeight = 0;
	renderOutlines = false;
	renderExtra = false;
	mAngle = 0.0;
	mCenter.x = 0;
	mCenter.y = 0;
	mFlip = SDL_FLIP_NONE;
	setTexture(path, rRend);
}

Texture::Texture(std::string path, SDL_Renderer* rRend, int posX, int posY)
{
	//Init variables
	tTexture = nullptr;
	mPosX = posX;
	mPosY = posY;
	mWidth = 0;
	mHeight = 0;
	renderOutlines = false;
	renderExtra = false;
	mAngle = 0.0;
	mCenter.x = 0;
	mCenter.y = 0;
	mFlip = SDL_FLIP_NONE;
	setTexture(path, rRend);
}


Texture::~Texture()
{
	Free();
}


void Texture::Render(SDL_Renderer* rRend, int posX, int posY)
{
	//Make rect for texture to render whole texture
	SDL_Rect renderQuad = { posX, posY, mWidth, mHeight };

	if (!renderExtra) {
		SDL_RenderCopy(rRend, tTexture, nullptr, &renderQuad);
	}
	else {
		SDL_RenderCopyEx(rRend, tTexture, nullptr, &renderQuad, mAngle, &mCenter, mFlip);
	}

	if (renderOutlines) {
		//Render (green) outline for debugging if true
		SDL_SetRenderDrawColor(rRend, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawRect(rRend, &renderQuad);
		SDL_SetRenderDrawColor(rRend, 0x00, 0x00, 0x00, 0xFF);
	}
}

void Texture::Render(SDL_Renderer* rRend, int posX, int posY, SDL_Rect* rClip)
{
	//Make rect for texture to render whole or part of texture
	SDL_Rect renderQuad = { posX, posY, mWidth, mHeight };

	if (rClip != nullptr) { renderQuad.w = rClip->w; renderQuad.h = rClip->h; }

	if (!renderExtra) {
		SDL_RenderCopy(rRend, tTexture, rClip, &renderQuad);
	}
	else {
		SDL_RenderCopyEx(rRend, tTexture, rClip, &renderQuad, mAngle, &mCenter, mFlip);
	}

	if (renderOutlines) {
		//Render (green) outline for debugging if true
		SDL_SetRenderDrawColor(rRend, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawRect(rRend, &renderQuad);
		SDL_SetRenderDrawColor(rRend, 0x00, 0x00, 0x00, 0xFF);
	}
}

void Texture::Render(SDL_Renderer* rRend, int posX, int posY, Texture* TileTexture, SDL_Rect* rClip)
{
	SDL_Rect renderQuad = { posX, posY, mWidth, mHeight };

	if (rClip != nullptr) { renderQuad.w = rClip->w; renderQuad.h = rClip->h; }
	SDL_RenderCopy(rRend, TileTexture->getTexture(), rClip, &renderQuad);

	if (!renderExtra) {
		SDL_RenderCopy(rRend, TileTexture->getTexture(), rClip, &renderQuad);
	}
	else {
		SDL_RenderCopyEx(rRend, TileTexture->getTexture(), rClip, &renderQuad, mAngle, &mCenter, mFlip);
	}

	if (renderOutlines) {
		//Render (green) outline for debugging if true
		SDL_SetRenderDrawColor(rRend, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawRect(rRend, &renderQuad);
		SDL_SetRenderDrawColor(rRend, 0x00, 0x00, 0x00, 0xFF);
	}
}


//SDL_Texture* Texture::getTexture()
//{

//}

void Texture::setTexture(std::string path, SDL_Renderer* rRend)
{
	std::string texturepath = "files/images/" + path;
	//Temp texture
	SDL_Texture* tempTexture = nullptr;

	//Load image at path
	SDL_Surface* loadedSurface = IMG_Load(texturepath.c_str());
	if (loadedSurface == nullptr) { std::cout << "Could not load image at : " << texturepath << "." << std::endl << "SDL_image error: " << IMG_GetError() << std::endl; }
	else {
		//colorkey

		//Create texture from surface pixels
		tempTexture = SDL_CreateTextureFromSurface(rRend, loadedSurface);
		if (tempTexture == nullptr) { std::cout << "Could not create texture!" << std::endl << "SDL Error: " << SDL_GetError() << std::endl; }
		else {
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		//Free surface
		SDL_FreeSurface(loadedSurface);
	}
	tTexture = tempTexture;
}

void Texture::checkTextureSettings() {
	bool check = false;
	if (mAngle != 0.0) { check = true; }
	if (mCenter.x != 0 || mCenter.y != 0) { check = true; }
	if (mFlip != SDL_FLIP_NONE) { check = true; }
	if (check) { renderExtra = true; }
	else { renderExtra = false; }
}

SDL_Texture* Texture::getTexture()
{
	return tTexture;
}

void Texture::clearTexture() {
	if (tTexture != nullptr) {
		SDL_DestroyTexture(tTexture);
		tTexture = nullptr;
		mWidth = 0;
		mHeight = 0;
	}
}

int Texture::getPosX() { return mPosX; }

int Texture::getPosY() { return mPosY; }

int Texture::getWidth() { return mWidth; }

int Texture::getHeight() { return mHeight; }

SDL_RendererFlip Texture::getFlip() { return mFlip; }

void Texture::setAlpha(Uint8 alpha) {
	if (tTexture != nullptr) { SDL_SetTextureAlphaMod(tTexture, alpha); checkTextureSettings(); }
}

void Texture::setFlip(SDL_RendererFlip newFlip) { mFlip = newFlip; checkTextureSettings(); }

void Texture::RenderStringToTexture(std::string Text, SDL_Renderer* rRend, TTF_Font* fFont, SDL_Color cColor)
{
	//Clear existing texture
	clearTexture();

	SDL_Surface* textSurface = TTF_RenderText_Solid(fFont, Text.c_str(), cColor);
	if (textSurface == nullptr) { std::cout << "Failed to create surface for rendering text!" << std::endl; std::cout << TTF_GetError() << std::endl; }
	else {
		//create texture from surface
		tTexture = SDL_CreateTextureFromSurface(rRend, textSurface);
		if (tTexture == nullptr) { std::cout << "Could not create texture!" << std::endl << "SDL Error: " << SDL_GetError() << std::endl; }
		else {
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Free created surface
		SDL_FreeSurface(textSurface);
	}
}

void Texture::Free() {
	SDL_DestroyTexture(tTexture);
	tTexture = nullptr;
	mPosX = 0;
	mPosY = 0;
	mWidth = 0;
	mHeight = 0;
	renderOutlines = false;
	renderExtra = false;
	mAngle = 0.0;
	mCenter.x = 0;
	mCenter.y = 0;
	mFlip = SDL_FLIP_NONE;
}