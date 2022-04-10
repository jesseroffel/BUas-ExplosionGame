#pragma once

#ifndef TEXT_H
#define TEXT_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include <string>

#include "texture.h"

class Text
{
public:
	Text();
	Text(std::string Text, TTF_Font* fFont, SDL_Renderer* rRend, int posx, int posy, SDL_Color cColor);
	~Text();

	void RenderStringToTexture(std::string Text, SDL_Renderer* rRend, TTF_Font* fFont, SDL_Color cColor);

	void Render(SDL_Renderer* rRend);

	void setFontID(int newID);
	void setPosX(int newposx);
	void setPosY(int newposy);
	void setText(std::string newText);
	void freeTexture();

	int getPosX();
	int getPosY();
	bool getHasText();
	void setHasText(bool state);

private:
	Texture* tTextTexture = nullptr;

	SDL_Color mColor = { 0x00, 0x00, 0x00, 0xFF };

	int mFontID = 0;
	int mPosX = 0;
	int mPosY = 0;
	int mTextWidth = 0;
	int mTextHeight = 0;

	bool renderOutlines = false;
	bool hasText = false;

	std::string mText = "";
};

#endif