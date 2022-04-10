#include "text.h"

Text::Text()
{
}

Text::Text(std::string Text, TTF_Font* fFont, SDL_Renderer* rRend, int posx, int posy, SDL_Color cColor)
{
	mPosX = posx;
	mPosY = posy;
	mFontID = 0;
	if (Text == "") {
		tTextTexture = new Texture();
		mTextWidth = 0;
		mTextHeight = 0;
		hasText = false;
	} else {
		tTextTexture = new Texture();
		tTextTexture->RenderStringToTexture(Text, rRend, fFont, cColor);
		mTextWidth = tTextTexture->getWidth();
		mTextHeight = tTextTexture->getHeight();
		hasText = true;
	}
	renderOutlines = false;
}

Text::~Text()
{
	delete tTextTexture;
}


void Text::RenderStringToTexture(std::string Text, SDL_Renderer* rRend, TTF_Font* fFont, SDL_Color cColor)
{
	tTextTexture->RenderStringToTexture(Text, rRend, fFont, cColor);
	mTextWidth = tTextTexture->getWidth();
	mTextHeight = tTextTexture->getHeight();
	if (!hasText) { hasText = true; }
}

void Text::Render(SDL_Renderer* rRend)
{
	if (hasText)
	{
		tTextTexture->Render(rRend, mPosX, mPosY);
		if (renderOutlines) {
			SDL_Rect renderQuad = { mPosX, mPosY, mTextWidth, mTextHeight };
			//Render (green) outline for debugging if true
			SDL_SetRenderDrawColor(rRend, 0x00, 0xFF, 0x00, 0xFF);
			SDL_RenderDrawRect(rRend, &renderQuad);
			SDL_SetRenderDrawColor(rRend, 0x00, 0x00, 0x00, 0xFF);
		}
	}
}

void Text::freeTexture() {
	if (tTextTexture != nullptr) {
		tTextTexture->clearTexture();
		tTextTexture = nullptr;
	}
}

void Text::setFontID(int newID) { mFontID = newID; }
void Text::setPosX(int newposx) { mPosX = newposx; }
void Text::setPosY(int newposy) { mPosY = newposy; }
void Text::setText(std::string newText) { mText = newText; }

int Text::getPosX() { return mPosX; }
int Text::getPosY() { return mPosY; }

bool Text::getHasText() { return hasText; }

void Text::setHasText(bool state)
{
	if (state) { hasText = true; }
	else { hasText = false; }
}