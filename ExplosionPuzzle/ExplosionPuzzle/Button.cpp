#include "Button.h"

Button::Button() {}

Button::Button(std::string path, SDL_Renderer* rRend, TTF_Font* FONT, int posX, int posY, BTYPE ButtonType)
{
	mPosX = posX;
	mPosY = posY;
	mButtonIconPosX = mPosX;
	mButtonIconPosY = mPosY;
	mInteractable = true;
	interacted = false;
	curInteraction = false;
	renderOutlines = false;
	drawButton = true;
	tButtonTexture = new Texture("button_frame.png", rRend, mPosX, mPosY);
	mButtonWidth = tButtonTexture->getWidth();
	mButtonHeight = tButtonTexture->getHeight();
	tButtonIcon = new Texture(path, rRend, mButtonIconPosX, mButtonIconPosY);
	mButtonIconWidth = tButtonIcon->getWidth();
	mButtonIconHeight = tButtonIcon->getHeight();
	mButtonType = ButtonType;
	mEntityType = eNothing;
	tTextureAlpha = 255;
	
	SDL_Color blackColor = { 0x00, 0x00, 0x00, 0xFF };
	txtInfo = new Text("Init Text!", FONT, rRend, mPosX, (mPosY + mButtonWidth), blackColor);
	setText(rRend, FONT);
}

Button::Button(std::string path, SDL_Renderer* rRend, int posX, int posY, BTYPE buttonType, ETYPE entityType)
{
	mPosX = posX;
	mPosY = posY;
	mButtonIconPosX = mPosX;
	mButtonIconPosY = mPosY;
	mInteractable = true;
	interacted = false;
	curInteraction = false;
	renderOutlines = false;
	drawButton = false;
	//tButtonTexture = new Texture("button_frame.png", rRend, mPosX, mPosY);

	tButtonIcon = new Texture(path, rRend, mButtonIconPosX, mButtonIconPosY);
	mButtonWidth = tButtonIcon->getWidth();
	mButtonHeight = tButtonIcon->getHeight();
	mButtonIconWidth = tButtonIcon->getWidth();
	mButtonIconHeight = tButtonIcon->getHeight();
	mButtonType = buttonType;
	mEntityType = entityType;

	tTextureAlpha = 255;
	//tButtonTexture->setAlpha(tTextureAlpha);

	//SDL_Color blackColor = { 0x00, 0x00, 0x00, 0xFF };
	//txtInfo = new Text("Init Text!", FONT, rRend, mPosX, 788, blackColor);
	//setText(rRend, FONT);
}


/*
Button::Button(std::string path, SDL_Renderer* rRend, int posX, int posY, int bW, int bH)
{
	mPosX = posX;
	mPosY = posY;
	interacted = false;
	renderOutlines = true;
	tButtonTexture = new Texture(path, rRend, posX, posY);
	mButtonWidth = bW;
	mButtonHeight = bH;
}
*/

Button::~Button()
{
	Free();
}


//Set topleft position
void Button::setPosition(int x, int y)
{

}

//Handle mouse events
void Button::handleEvents(SDL_Event* eEvent, Music* oMusic)
{
	if (mButtonType != 0 && mInteractable)
	{
		mReturnButtonType = 0;
		mReturnEntityType = 0;
		SDL_GetMouseState(&mX, &mY);
		if (mX >= 0 && mX <= 800 && mY >= 0 && mY <= 800)
		{
			bool checkmousepos = checkMousePos();
			if (checkmousepos)
			{
				switch (eEvent->type)
				{
					case SDL_MOUSEMOTION:
					{
						if (playsfx) {
							playsfx = false;
							oMusic->playSFX(oMusic->sMouseHover);
						}
						break;
					}
					case SDL_MOUSEBUTTONUP:
					{
						//Mouse is within the button width/height
						bool checkmouse = checkMousePos();
						if (checkmouse)
						{
							mReturnButtonType = mButtonType;
							mReturnEntityType = mEntityType;
							switch (mReturnButtonType)
							{
							case 1:
								oMusic->playSFX(oMusic->sActionclap);
								break;
							case 3: case 4:
								oMusic->playSFX(oMusic->sMouseSelectButton);
								break;
							case 5:
								if (mReturnEntityType != 0) {
									oMusic->playSFX(oMusic->sMouseSelectInventory);
								}
								break;
							}
						}
					}
				}
			}
		}
		
	} else {
		if (interacted != curInteraction) {
			//Remove highlighting
			curInteraction = interacted;
		}
	}
}

bool Button::checkMousePos() {

	bool inside = true;
	renderText = true;
	//mouse is left of the button
	if (mX < mPosX) { inside = false; playsfx = true; }
	//mouse is right of the button
	else if (mX > mPosX + mButtonWidth) { inside = false; playsfx = true; }
	//mouse is above button
	else if (mY < mPosY) { inside = false; playsfx = true; }
	//mouse is below button
	else if (mY > mPosY + mButtonHeight) { inside = false; playsfx = true; }

	if (!inside) {
		renderText = false;
	}

	return inside;
}

//Render button on screen
void Button::Render(SDL_Renderer* rRend)
{
	if (drawButton) { tButtonTexture->Render(rRend, mPosX, mPosY); }
	tButtonIcon->Render(rRend, mButtonIconPosX, mButtonIconPosY);
	if (renderOutlines) {
		SDL_Rect renderQuad = { mPosX, mPosY, mButtonWidth, mButtonHeight };
		//Render (red) outline for debugging if true
		SDL_SetRenderDrawColor(rRend, 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(rRend, &renderQuad);
		SDL_SetRenderDrawColor(rRend, 0x00, 0x00, 0x00, 0xFF);
	}

	if (renderText && mInteractable && mButtonType == bGoToAction || mButtonType == bGoToEditor) {
		txtInfo->Render(rRend); 
	}
}

void Button::setInteractable(bool state)
{
	if (state) {
		mInteractable = true;
		tButtonIcon->setAlpha(255);
	} else {
		mInteractable = false;
		renderText = false;
		tButtonIcon->setAlpha(125);
	}

}

bool Button::getInteractable() { return mInteractable; }

void Button::setAlpha(Uint8 newalpha) {
	tTextureAlpha = newalpha;
	tButtonIcon->setAlpha(tTextureAlpha);
}

int Button::getEntity() { return mEntityType; }

int Button::getReturnType() {
	return mReturnButtonType;
}

int Button::getEntityType()
{
	return mReturnEntityType;
}

void Button::Free()
{
	delete tButtonIcon;
	delete tButtonTexture;
	delete tButtonEntityCount;
	delete txtInfo;
	tButtonIcon = nullptr;
	tButtonTexture = nullptr;

	mPosX = 0;
	mPosY = 0;
	mInteractable = false;
	interacted = false;
	curInteraction = false;
	renderOutlines = false;
	mButtonWidth = 0;
	mButtonHeight = 0;
	mButtonType = bNothing;
	mEntityType = eNothing;
}

void Button::setText(SDL_Renderer* rRend, TTF_Font* FONT)
{
	SDL_Color blackColor = { 0x00, 0x00, 0x00, 0xFF };
	switch (mButtonType)
	{
		case bGoToAction:
			txtInfo->RenderStringToTexture("Test this configuration!", rRend, FONT, blackColor);
			break;
		case bGoToEditor:
			txtInfo->RenderStringToTexture("Go to editor", rRend, FONT, blackColor);
			break;
	}
}

//int Button::getButtonType() { return mButtonType; }

//int Button::getEntityType() { return mEntityType; }