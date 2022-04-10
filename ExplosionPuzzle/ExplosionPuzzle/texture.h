#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

class Texture
{
public:
	Texture();
	Texture(std::string path, SDL_Renderer* rRend);
	Texture(std::string path, SDL_Renderer* rRend, int posX, int posY);
	~Texture();

	void Render(SDL_Renderer* rRend, int posX, int posY);
	void Render(SDL_Renderer* rRend, int posX, int posY, SDL_Rect* rClip);
	void Render(SDL_Renderer* rRend, int posX, int posY, Texture* TileTexture, SDL_Rect* rClip);

	void Texture::setTexture(std::string path, SDL_Renderer* rRend);
	SDL_Texture* getTexture();
	
	int getPosX();
	int getPosY();
	int getWidth();
	int getHeight();

	//set alpha modulation
	void setAlpha(Uint8 alpha);
	void setFlip(SDL_RendererFlip newflips);
	SDL_RendererFlip getFlip();
	void checkTextureSettings();

	void clearTexture();

	void RenderStringToTexture(std::string Text, SDL_Renderer* rRend, TTF_Font* fFont, SDL_Color cColor);

	void Free();

private:
	//Texture object
	SDL_Texture* tTexture;
	//Rect for x/y position
	int mPosX;
	int mPosY;
	int mWidth;
	int mHeight;
	bool renderOutlines;
	//check for to check SDL_RenderCopy(Ex);
	bool renderExtra;

	double mAngle;
	SDL_Point mCenter;
	SDL_RendererFlip mFlip;
};

#endif