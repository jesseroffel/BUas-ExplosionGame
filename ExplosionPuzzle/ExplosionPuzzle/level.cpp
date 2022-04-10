#include "level.h"

Level::Level() {}

Level::Level(SDL_Renderer* rRend)
{
	currentLevel = 1;
	mousePosX = 0;
	mousePosY = 0;
	renderLevel = true;
	currentPlayState = PlayState::pEditor;
	switchingstage = false;
	loadinglevel = true;
	checkPlaceDelete = true;
	createNewFollow = false;
	createNewEntity = false;
	failedAttempt = true;
	finishlevel = false;
	levelFolder = "files/levels/";
	tTileTextureAlpha = 125;
	tiLevelComplete = nullptr;
	LoadLevelNumber();

	//Init Texts
	ttfFONT = TTF_OpenFont("files/font/Pixellari.ttf", 16);
	teLevelText = new Text("Init Text!", ttfFONT, rRend, 10, 10, blackColor);
	teLevelHint1 = new Text("Init Text!", ttfFONT, rRend, 10, 686, blackColor);
	teLevelHint2 = new Text("Init Text!", ttfFONT, rRend, 10, 670, blackColor);
	teDeleteMode = new Text("Delete mode", ttfFONT, rRend, 709, 686, blackColor);
	tePlaceMode = new Text("Edit mode", ttfFONT, rRend, 726, 686, blackColor);
	teLevelCleared = new Text("LEVEL COMPLETED", ttfFONT, rRend, 340, 25, WarningColor);
	teResetLevelsWarning = new Text("RESET TO LEVEL 1? PRESS Y TO CONFIRM AND N TO CANCEL", ttfFONT, rRend, 175, 380, WarningColor);
	teResetLevelsWarningHigh = new Text("RESET TO LEVEL 1? PRESS Y TO CONFIRM AND N TO CANCEL", ttfFONT, rRend, 176, 381, blackColor);
	
	//Init Music Object
	mMusicHandler = new Music();

	//Init Level
	loadGameObjects(rRend);
	//lEntities[0].push_back(new Player("player.png", rRend, 50, 50));
}


Level::~Level()
{
	//Delete Textures HUD
	delete tHUD_BottomBarEdit;
	delete tHUD_BottomBarPlaying;
	delete tHUD_PlayBG;
	delete tHUD_EditBG;
	delete tHUD_EditLines;
	delete tHUD_LevelComplete;
	//Delete Textures Objects
	delete tTileTexture;
	delete tFinishTexture;
	delete tBombHorizontalTexture;
	delete tBombVerticalTexture;

	//Delete Buttons
	delete bHUD_GoToAction;
	delete bHUD_GoToEditor;
	delete bHUD_DeleteEntities;
	delete bHUD_PlaceEntities;

	//Delete Inventory Buttons using iterator
	for (std::vector<Button*>::iterator i = vInventoryButtons.begin(); i != vInventoryButtons.end(); i++)
	{
		delete(*i);
	}

	//Delete Objects
	delete oEntityPlacer;
	delete oPlayer;
	delete oTestPlayer;
	delete oEditFinish;
	delete oPlayFinish;

	//Delete Tiles using iterator
	for (std::vector<Tile*>::iterator i = vTileset.begin(); i != vTileset.end(); i++)
	{
		delete(*i);
	}

	//Delete Bombs using iterator
	for (std::vector<Bomb*>::iterator i = vBombList.begin(); i != vBombList.end(); i++)
	{
		delete(*i);
	}
	for (std::vector<Bomb*>::iterator i = vBombListPlay.begin(); i != vBombListPlay.end(); i++)
	{
		delete(*i);
	}

	//delete Timers
	delete tiLevelComplete;

	//Delete Music
	delete mMusicHandler;

	//Delete Fonts
	delete teLevelText;
	delete teLevelHint1;
	delete teLevelHint2;
	delete teDeleteMode;
	delete tePlaceMode;
	delete teLevelCleared;
	delete teResetLevelsWarning;
	delete teResetLevelsWarningHigh;
	//Fix access error for closing font..
	//TTF_Close();
	ttfFONT = nullptr;

	closeGameInstruction = true;
}


void Level::loadGameObjects(SDL_Renderer* rRend)
{
	//HUD Editor
	tHUD_BottomBarEdit = new Texture("hud_bottom.png", rRend, 0, 704);
	tHUD_EditBG = new Texture("editbgpaper.png", rRend, 0, 0);
	tHUD_EditLines = new Texture("editbglines.png", rRend, 0, 0);

	bHUD_GoToAction = new Button("action.png", rRend, ttfFONT, 16, 720, Button::bGoToAction);

	bHUD_DeleteEntities = new Button("remove.png", rRend, ttfFONT, 720, 720, Button::bDelete);
	bHUD_PlaceEntities = new Button("place.png", rRend, ttfFONT, 720, 720, Button::bPlace);

	//HUD Playing
	tHUD_PlayBG = new Texture("playbg.png", rRend, 0, 0);
	tHUD_BottomBarPlaying = new Texture("hud_bottom.png", rRend, 0, 704);

	tHUD_LevelComplete = new Texture("goalpanel.png", rRend, (mLevelWidth / 2) - (150), 100);

	bHUD_GoToEditor = new Button("edit.png", rRend, ttfFONT, 16, 720, Button::bGoToEditor);

	//TILES
	tTileTexture = new Texture("tiles.png", rRend);
	//Finishes
	tFinishTexture = new Texture("baskets.png", rRend);
	//Bombs
	tBombHorizontalTexture = new Texture("bombx.png", rRend);
	tBombVerticalTexture = new Texture("bomby.png", rRend);

	//Apply settings
	bHUD_GoToAction->setInteractable(false);
	bHUD_DeleteEntities->setInteractable(false);

	//Set tile properties
	initTiles(rRend);
	//Init selected level
	initLevelObjects(rRend);

	//Play music
	if (mMusicHandler != nullptr) { mMusicHandler->playMusic(mMusicHandler->mEditor); }
}

void Level::initTiles(SDL_Renderer* rRend) {
	//Get current tile width/height
	int tW = 32;
	int tH = 32;

	if (tTileTexture != nullptr) {

		//TILE 0
		rTileClips[TILE_SKY].x = 0;
		rTileClips[TILE_SKY].y = 0;
		rTileClips[TILE_SKY].w = tW;
		rTileClips[TILE_SKY].h = tH;
		//TILE 1
		rTileClips[TILE_SOLIDWALL].x = 32;
		rTileClips[TILE_SOLIDWALL].y = 0;
		rTileClips[TILE_SOLIDWALL].w = tW;
		rTileClips[TILE_SOLIDWALL].h = tH;

		//TILE 2
		rTileClips[TILE_GRASSTOP].x = 64;
		rTileClips[TILE_GRASSTOP].y = 0;
		rTileClips[TILE_GRASSTOP].w = tW;
		rTileClips[TILE_GRASSTOP].h = tH;
		//TILE 3
		rTileClips[TILE_GRASSRIGHT].x = 96;
		rTileClips[TILE_GRASSRIGHT].y = 0;
		rTileClips[TILE_GRASSRIGHT].w = tW;
		rTileClips[TILE_GRASSRIGHT].h = tH;
		//TILE 4
		rTileClips[TILE_GRASSDOWN].x = 128;
		rTileClips[TILE_GRASSDOWN].y = 0;
		rTileClips[TILE_GRASSDOWN].w = tW;
		rTileClips[TILE_GRASSDOWN].h = tH;
		//TILE 5
		rTileClips[TILE_GRASSLEFT].x = 160;
		rTileClips[TILE_GRASSLEFT].y = 0;
		rTileClips[TILE_GRASSLEFT].w = tW;
		rTileClips[TILE_GRASSLEFT].h = tH;

		//TILE 6
		rTileClips[TILE_GRASSCORUPRIGHT].x = 192;
		rTileClips[TILE_GRASSCORUPRIGHT].y = 0;
		rTileClips[TILE_GRASSCORUPRIGHT].w = tW;
		rTileClips[TILE_GRASSCORUPRIGHT].h = tH;
		//TILE 7
		rTileClips[TILE_GRASSCORDOWNRIGHT].x = 224;
		rTileClips[TILE_GRASSCORDOWNRIGHT].y = 0;
		rTileClips[TILE_GRASSCORDOWNRIGHT].w = tW;
		rTileClips[TILE_GRASSCORDOWNRIGHT].h = tH;
		//TILE 8
		rTileClips[TILE_GRASSCORDOWNLEFT].x = 256;
		rTileClips[TILE_GRASSCORDOWNLEFT].y = 0;
		rTileClips[TILE_GRASSCORDOWNLEFT].w = tW;
		rTileClips[TILE_GRASSCORDOWNLEFT].h = tH;
		//TILE 9
		rTileClips[TILE_GRASSCORUPLEFT].x = 288;
		rTileClips[TILE_GRASSCORUPLEFT].y = 0;
		rTileClips[TILE_GRASSCORUPLEFT].w = tW;
		rTileClips[TILE_GRASSCORUPLEFT].h = tH;

		tTileTexture->setAlpha(tTileTextureAlpha);
	}

	//FINISH
	rFinishClips[0].x = 0;
	rFinishClips[0].y = 0;
	rFinishClips[0].w = 0;
	rFinishClips[0].h = 0;

	rFinishClips[FINISH_ABOVE].x = 0;
	rFinishClips[FINISH_ABOVE].y = 0;
	rFinishClips[FINISH_ABOVE].w = 32;
	rFinishClips[FINISH_ABOVE].h = 32;

	rFinishClips[FINISH_LEFT].x = 32;
	rFinishClips[FINISH_LEFT].y = 0;
	rFinishClips[FINISH_LEFT].w = 32;
	rFinishClips[FINISH_LEFT].h = 32;

	rFinishClips[FINISH_RIGHT].x = 64;
	rFinishClips[FINISH_RIGHT].y = 0;
	rFinishClips[FINISH_RIGHT].w = 32;
	rFinishClips[FINISH_RIGHT].h = 32;

	rFinishClips[FINISH_BELOW].x = 96;
	rFinishClips[FINISH_BELOW].y = 0;
	rFinishClips[FINISH_BELOW].w = 32;
	rFinishClips[FINISH_BELOW].h = 32;
}

void Level::Input(SDL_Event* eEvent)
{
	if (!switchingstage)
	{
		switch (currentPlayState)
		{
		case pEditor:
			InputEditor(eEvent);
			break;
		case pPlayTest:
			InputPlayTest(eEvent);
			break;
		}
	}
}

void Level::Update()
{
	if (!switchingstage)
	{
		switch (currentPlayState)
		{
		case pEditor:
			UpdateEditor();
			break;
		case pPlayTest:
			UpdatePlayTest();
			break;
		}

		mMusicHandler->Update();
	}
	lockSwitching = false;
	if (buttonAction != 0) { 
		checkButtonValue(); 
		buttonAction = 0;
		buttonActionEntity = 0;
	}
}

void Level::Render(SDL_Renderer* rRend)
{
	if (loadinglevel && finishlevel) {
		SwapLevel(rRend);
	}


	if (currentPlayState != checkPlayState) {
		switchingstage = true;
		switchStates(rRend);
	}
	if (!switchingstage)
	{
		switch (currentPlayState)
		{
		case pEditor:
			RenderEditor(rRend);
			break;
		case pPlayTest:
			RenderPlayTest(rRend);
			break;
		}
	}
}

void Level::checkButtonValue()
{
	switch (buttonAction) {
		case 1:
			checkPlayState = PlayState::pPlayTest;
			break;
		case 2:
			checkPlayState = PlayState::pEditor;
			break;
		case 3:
			checkPlaceDelete = true;
			break;
		case 4:
			checkPlaceDelete = false;
			break;
	}

	//Check if it is allowed to create a new entity, if entity button, check vInventoryUseAmount for type match
	if (buttonAction == 5) {
		if (buttonActionEntity != 0) {
			if (checkPlaceDelete) {
				if (buttonActionEntity >= 0 && buttonActionEntity <= 3) {
					bool disablecheck = true;
					for (auto &i : vInventoryUseAmount) {
						if (i[0] == buttonActionEntity) {
							if (i[1] > 0) {
								if (oEntityPlacer == nullptr) { 
									createNewFollow = true;
									disablecheck = false;
									newEntityType = buttonActionEntity;
								}
							}
						}
					}
					if (disablecheck) {
						buttonAction = 0;
						buttonActionEntity = 0;
					}
				}
			}
		}
	}
}

void Level::SetPlayState(PlayState newplaystate)
{
	currentPlayState = newplaystate;
}

bool Level::createLevel(SDL_Renderer* rRend)
{
	//Tile data
	int tileAmount = 550;
	int tileSprites = 10;
	int tileWidth = 32;
	int tileHeight = 32;

	int levelWidth = 800;
	int levelHeight = 704;

	//Tile reading offsets
	int rX = 0;
	int rY = 0;

	bool levelLoaded = true;
	std::ifstream map(levelFolder + levelFilename);

	if (!map.is_open()) { std::cout << "Could not load tilemap at: " << levelFolder + levelFilename << "." << std::endl; }
	else {
		for (int i = 0; i < tileAmount; ++i)
		{
			//Make tiletype check
			int tileType = -1;
			//Read current tile from file and set als tiletype
			map >> tileType;
			tileType--;
			if (map.fail()) { 
				std::cout << "Unexpected end of file at: " << levelFolder + levelFilename << "." << std::endl;
				levelLoaded = false;
				break;
			}
			//Check if number is a tilenumber
			if ((tileType >= 0) && (tileType < tileSprites))
			{
				//SDL_Rect rRenderbox = rTileClips[tileType];
				vTileset.push_back(new Tile(rRend, rX, rY, tileType));//, &rRenderbox);
			} 
			else {
				std::cout << "Unvalid tileType: " << tileType << " at: " << rX << " " << rY << std::endl;
				levelLoaded = false;
			}
			//Go to next tile position and move to next line if at end of level
			rX += tileWidth;
			if (rX >= levelWidth) { 
				rX = 0; 
				rY += tileHeight; 
			}
		}
	}
	//Close file reading object
	map.close();
	vTilesetCount = vTileset.size();
	return levelLoaded;
}

void Level::SwapLevel(SDL_Renderer* rRend)
{
	if (currentLevel > MAX_LEVELS)
	{
		closeGameInstruction = true;
	} else {
		renderLevel = false;
		clearLevel();
		teLevelHint1->setHasText(false);
		teLevelHint2->setHasText(false);
		currentLevel++;
		SaveLevel();
		initLevelObjects(rRend);
		checkPlaceDelete = true;
		renderLevel = true;
		loadinglevel = false;
		switchingstage = false;
		createNewFollow = false;
		createNewEntity = false;
		finishlevel = false;
		currentPlayState = PlayState::pEditor;
		checkPlayState = PlayState::pEditor;
		mMusicHandler->fadeToMusic(mMusicHandler->mEditor);
		mousePosX = 0;
		mousePosY = 0;
		bHUD_GoToEditor->setInteractable(true);
		bHUD_GoToAction->setInteractable(false);
		vBombListCount = 0;
		vBombListPlayCount = 0;
		tTileTextureAlpha = 125;
	}
}

void Level::clearLevel()
{
	
	//Clear Tiles
	for (std::vector<Tile*>::iterator it = vTileset.begin(); it != vTileset.end(); it++) {
		delete (*it);
		(*it) = nullptr;
	}
	vTileset.clear();
	vTilesetCount = 0;

	//Clear Inventory buttons
	for (std::vector<Button*>::iterator it = vInventoryButtons.begin(); it != vInventoryButtons.end(); it++) {
		delete (*it);
		(*it) = nullptr;
	}
	vInventoryButtons.clear();
	vInventoryUseAmount.clear();
	vInventoryButtonsCount = 0;

	//Clear vBomblist
	for (std::vector<Bomb*>::iterator it = vBombList.begin(); it != vBombList.end(); it++) {
		delete (*it);
		(*it) = nullptr;
	}
	vBombList.clear();
	vBombListCount = 0;
	
	//Clear Editor Objects
	delete oPlayer;
	oPlayer = nullptr;

	delete oEditFinish;
	oEditFinish = nullptr;

	delete tiLevelComplete;
	tiLevelComplete = nullptr;

	clearPlayingObjects();

	//Load new level
}

void Level::clearPlayingObjects()
{
	tTileTextureAlpha = 125;
	tTileTexture->setAlpha(tTileTextureAlpha);

	//Delete all objects in vBombListPlay that had been used in play mode
	for (std::vector<Bomb*>::iterator it = vBombListPlay.begin(); it != vBombListPlay.end(); it++) {
		// delete object, set to nullptr, erase from list and reset iterator
		delete (*it);
		(*it) = nullptr;
	}
	vBombListPlay.clear();
	vBombListPlayCount = vBombListPlay.size();

	//Delete other objects used in play mode
	delete oTestPlayer;
	oTestPlayer = nullptr;
	delete oPlayFinish;
	oPlayFinish = nullptr;

	//Enable/disable place/delete buttons on return to editor
	bool allempty = true;
	for (auto &i : vInventoryUseAmount)
	{
		if (i[1] != 0) { allempty = false; }
	}
	if (allempty) {
		bHUD_PlaceEntities->setInteractable(false);
		bHUD_DeleteEntities->setInteractable(false);
	} else {
		bHUD_PlaceEntities->setInteractable(true);
		bHUD_DeleteEntities->setInteractable(true);
	}
	checkPlaceDelete = false;
}

void Level::applyEditChanges(SDL_Renderer* rRend)
{
	//Copy objects placed in editor
	for (unsigned int i = 0; i < vBombList.size(); i++)
	{
		vBombListPlay.push_back(new Bomb(rRend, vBombList[i]->getPosX(), vBombList[i]->getPosY(), vBombList[i]->getBombType()));
	}
	vBombListPlayCount = vBombListPlay.size();

	//Make a copy of the player and finish
	oTestPlayer = new Player("player.png", rRend, (int)oPlayer->getFPosX(), (int)oPlayer->getFPosY(), oPlayer->getFacing());
	oPlayFinish = new Finish(rRend, oEditFinish->getPosX(), oEditFinish->getPosY(), oEditFinish->getFinishType());

	tTileTextureAlpha = 255;
	tTileTexture->setAlpha(tTileTextureAlpha);

	//Set information attempts/items used etc
}


void Level::switchStates(SDL_Renderer* rRend)
{
	if (!lockSwitching)
	{
		lockSwitching = true;
		switch (checkPlayState)
		{
		case pEditor:
			clearPlayingObjects();
			if (mMusicHandler != nullptr) { mMusicHandler->fadeToMusic(mMusicHandler->mEditor); }
			if (failedAttempt) { mMusicHandler->playSFX(mMusicHandler->sFinishLose); }
			break;
		case pPlayTest:
			applyEditChanges(rRend);
			if (mMusicHandler != nullptr) { mMusicHandler->fadeToMusic(mMusicHandler->mPlaying); }
			break;
		}
		//Changes done
		currentPlayState = checkPlayState;
		switchingstage = false;
	}

}

void Level::InputEditor(SDL_Event* eEvent)
{
	//Check mouse
	if (eEvent->type == SDL_KEYDOWN && eEvent->key.repeat == 0) {
		switch (eEvent->key.keysym.sym)
		{
			case SDLK_g:
				EnableGrid = !EnableGrid;
				break;
			case SDLK_r:
				if (currentLevel > 1) { resetquestion = true; }
				break;
			case SDLK_y:
				if (resetquestion) {
					resetquestion = false;
					hardreset = true;
				}
				break;
			case SDLK_n:
				if (resetquestion) { resetquestion = false; }
				break;
			/*case SDLK_c:
				//Delete Bombs using iterator
				for (std::vector<Bomb*>::iterator i = vBombList.begin(); i != vBombList.end(); i++)
				{
					delete(*i);
					(*i) = nullptr;
				}
				vBombList.clear();
				vBombListCount = vBombList.size();
				mMusicHandler->playSFX(mMusicHandler->sExplosion);
				break;*/
		}
	}
	if (eEvent->type == SDL_MOUSEMOTION)
	{
		mousePosX = eEvent->motion.x;
		mousePosY = eEvent->motion.y;
	}
	if (eEvent->type == SDL_MOUSEBUTTONDOWN)
	{
		if (eEvent->button.button == SDL_BUTTON_LEFT)
		{
			if (itemFollowMouse && oEntityPlacer != nullptr) {
				BoundryCheck;
				int comp = 32;
				//TODO REALTIME INFO PLZ
				BoundryCheck.x = 0;
				BoundryCheck.y = 0;
				BoundryCheck.w = 800;
				BoundryCheck.h = 704;
				bool check = true;
				if (mousePosX < BoundryCheck.x + comp) { check = false; }
				if (mousePosX > BoundryCheck.w - comp) { check = false; }
				if (mousePosY < BoundryCheck.y + comp) { check = false; }
				if (mousePosY > BoundryCheck.h - comp) { check = false; }

				//TODO CHECK IF TILE/OBJECT is in the way
				BoundryCheck.x = mousePosX;
				BoundryCheck.y = mousePosY;
				BoundryCheck.w = 16;
				BoundryCheck.h = 16;

				bool tilecheck = oEntityPlacer->checkIfCollisionTiles(BoundryCheck, vTileset);
				if (tilecheck) { check = false; }
				if (check) { 
					createNewEntity = true; 
					if (vBombListCount > 0) { bHUD_GoToAction->setInteractable(true);}
					bHUD_DeleteEntities->setInteractable(true);
					bHUD_PlaceEntities->setInteractable(true);
				}
				else {
					mMusicHandler->playSFX(mMusicHandler->sError);
				}
			}
		}
		if (eEvent->button.button == SDL_BUTTON_RIGHT)
		{
			if (checkPlaceDelete)
			{
				if (itemFollowMouse) {
					if (oEntityPlacer != nullptr) {
						delete oEntityPlacer;
						oEntityPlacer = nullptr;
						itemFollowMouse = false;
					}
					if (vBombListCount > 0) { bHUD_GoToAction->setInteractable(true);}
					bHUD_DeleteEntities->setInteractable(true);
					bHUD_PlaceEntities->setInteractable(true);
				}
			}
		}
	}


	//HUD Play Button
	if (vBombListCount > 0) {
		bHUD_GoToAction->handleEvents(eEvent, mMusicHandler);
		if (bHUD_GoToAction->getReturnType() != 0 && buttonAction == 0) {
			buttonAction = bHUD_GoToAction->getReturnType(); 
		}
		
		//HUD place/delete button
		if (checkPlaceDelete) {
			if (bHUD_PlaceEntities->getInteractable()) {
				bHUD_PlaceEntities->handleEvents(eEvent, mMusicHandler);
				if (bHUD_PlaceEntities->getReturnType() != 0 && buttonAction == 0) {
					buttonAction = bHUD_PlaceEntities->getReturnType();
				}
			}
		}
		else {
			if (bHUD_DeleteEntities->getInteractable()) {
				bHUD_DeleteEntities->handleEvents(eEvent, mMusicHandler);
				if (bHUD_DeleteEntities->getReturnType() != 0 && buttonAction == 0) {
					buttonAction = bHUD_DeleteEntities->getReturnType();
				}
			}
		}
	}
	oPlayer->handleEvents(eEvent);

	//Handle bombs
	if (vBombListCount > 0)
	{
		for (std::vector<Bomb*>::iterator it = vBombList.begin(); it != vBombList.end(); it++) {
			if ((*it)->getRemove())
			{
				int type = (*it)->getBombType();
				// If object is marked as removable, delete object, set to nullptr, erase from list and reset iterator
				delete (*it);
				(*it) = nullptr;
				it = vBombList.erase(it);
				it = vBombList.begin();

				//vInventoryUseAmount[type]++;
				for (auto &i : vInventoryUseAmount) {
					if (i[0] == type) {
						i[1]++;
						for (int j = 0; j < vInventoryButtonsCount; ++j) {
							if (vInventoryButtons[j]->getEntity() == type) {
								vInventoryButtons[j]->setInteractable(true);
							}
						}
					}
				}

				//Check if there are objects left, if not break.
				vBombListCount = vBombList.size();
				if (vBombListCount == 0) { 
					bHUD_GoToAction->setInteractable(false);
					bHUD_DeleteEntities->setInteractable(false);
					checkPlaceDelete = true;
					break; 
				}
			}
			else {
				(*it)->handleEvents(eEvent, !checkPlaceDelete, mMusicHandler);
			}
		}
	}


	//Handle Inventory
	for (int i = 0; i < vInventoryButtonsCount; ++i)
	{
		vInventoryButtons[i]->handleEvents(eEvent, mMusicHandler);
		if (vInventoryButtons[i]->getReturnType() != 0 && buttonAction == 0) {
			buttonAction = vInventoryButtons[i]->getReturnType(); 
		} 
		if (vInventoryButtons[i]->getEntityType() != 0 && buttonActionEntity == 0) {
			buttonActionEntity = vInventoryButtons[i]->getEntityType();
			if (vInventoryButtons[i]->getInteractable()) {
				if (!checkPlaceDelete) { checkPlaceDelete = true; }
			}
		}
	}
	
}


void Level::InputPlayTest(SDL_Event* eEvent)
{
	//HUD Play Button
	if (!finishlevel) {
		bHUD_GoToEditor->handleEvents(eEvent, mMusicHandler);
		if (bHUD_GoToEditor->getReturnType() != 0) { buttonAction = bHUD_GoToEditor->getReturnType(); }
		//oTestPlayer->handleEvents(eEvent);
	}
}

void Level::UpdateEditor()
{
	//Update player position based on tiles
	//oPlayer->Update(vTileset, vBombList, oEditFinish, mMusicHandler);

	if (itemFollowMouse) { if(oEntityPlacer) {  oEntityPlacer->Update(mousePosX, mousePosY); } }

	for (int i = 0; i < vBombListCount; ++i)
	{
		vBombList[i]->Update(mMusicHandler);
	}
}

void Level::UpdatePlayTest()
{
	if (tiLevelComplete != nullptr) { 
		if (tiLevelComplete->isStarted())
		{
			tiLevelComplete->UpdateTimer();
		}
	}
	oTestPlayer->Update(vTileset, vBombListPlay, oPlayFinish, mMusicHandler);
	//If player came to a standstill or failed to complete te level in a certain way, switch back to Editor.
	bool failedAttemptPlayer = oTestPlayer->getFail();
	if (failedAttemptPlayer) { currentPlayState = PlayState::pEditor; }

	for (int i = 0; i < vBombListPlayCount; ++i)
	{
		vBombListPlay[i]->Update(mMusicHandler);
	}

	if (oPlayFinish->getFinishTrigger()) {
		finishlevel = true;
		failedAttempt = false;
		oPlayFinish->setFinishTrigger(false);
		oPlayFinish->setFinishActivated(false);
		oTestPlayer->setControllable(false);
		oTestPlayer->setPosX(oPlayFinish->getPosX());
		oTestPlayer->setPosY(oPlayFinish->getPosY());
		if (currentLevel == MAX_LEVELS) { 
			tiLevelComplete->SetTimer(30); 
			currentLevel = 1;	//reset level counter
		} else { tiLevelComplete->SetTimer(3); }
		bHUD_GoToEditor->setInteractable(false);
		mMusicHandler->playSFX(mMusicHandler->sFinishWin);
	}

	if (finishlevel) { 
		if (tiLevelComplete->getTimerCompleted()) {
			loadinglevel = true;
		}
	}
}


void Level::RenderEditor(SDL_Renderer* rRend)
{
	//Reset to level 1
	if (hardreset) {
		hardreset = false;
		currentLevel = 0;
		SwapLevel(rRend);
	}

	//HUD SPRITES
	tHUD_EditBG->Render(rRend, tHUD_EditBG->getPosX(), tHUD_EditBG->getPosY());
	tHUD_BottomBarEdit->Render(rRend, tHUD_BottomBarEdit->getPosX(), tHUD_BottomBarEdit->getPosY());

	//HUD BUTTONS
	bHUD_GoToAction->Render(rRend);
	if (checkPlaceDelete) { bHUD_DeleteEntities->Render(rRend); } else {  bHUD_PlaceEntities->Render(rRend); }

	//TILESET
	if (renderLevel) {
		for (int i = 0; i < vTilesetCount; ++i)
		{
			vTileset[i]->Render(rRend, tTileTexture, rTileClips[vTileset[i]->getType()]);
		}
	}

	//Render blue 'edit' lines as overlay
	if (EnableGrid) { tHUD_EditLines->Render(rRend, tHUD_EditBG->getPosX(), tHUD_EditBG->getPosY()); }

	//Handle Inventory
	for (int i = 0; i < vInventoryButtonsCount; ++i)
	{
		vInventoryButtons[i]->Render(rRend);
	}

	//Objects
	if (createNewFollow) {
		createNewFollow = false;
		if (oEntityPlacer == nullptr) { 
			oEntityPlacer = new EntityPlacer(rRend, mousePosX, mousePosY, newEntityType);
		}
		else {
			delete oEntityPlacer;
			oEntityPlacer = new EntityPlacer(rRend, mousePosX, mousePosY, newEntityType);
		}
		newEntityType = 0;
		itemFollowMouse = true;
		if (vBombListCount > 0) { bHUD_GoToAction->setInteractable(false);}
		bHUD_DeleteEntities->setInteractable(false);
		bHUD_PlaceEntities->setInteractable(false);
		mMusicHandler->playSFX(mMusicHandler->sMouseSelectInventory);
	}

	if (oEntityPlacer != nullptr && oEntityPlacer->getActive()) { oEntityPlacer->Render(rRend); }

	if (oPlayer != nullptr) { oPlayer->Render(rRend); }

	if (oEditFinish != nullptr) { 
		oEditFinish->Render(rRend, tFinishTexture, rFinishClips[oEditFinish->getFinishType()]); 
	}

	//Bomblist
	if (createNewEntity) {
		createNewEntity = false;
		//
		if (oEntityPlacer != nullptr) {
			int type = oEntityPlacer->getEntityType();
			vBombList.push_back(new Bomb(rRend, oEntityPlacer->getPosX(), oEntityPlacer->getPosY(), oEntityPlacer->getEntityType()));
			vBombListCount = vBombList.size();	
			//Check remove one use from vInventoryUseAmount, if 0 set button interactable to falce
			for (auto &i : vInventoryUseAmount) {
				if (i[0] == type) {
					i[1]--;
					if (i[1] == 0) {
						for (int j = 0; j < vInventoryButtonsCount; ++j) {
							if (vInventoryButtons[j]->getEntity() == type) {
								vInventoryButtons[j]->setInteractable(false);
							}
						}
					}
				}
			}
			//Check if all the entities are used, if so, set buttons to delete modus and disable placing.
			bool allempty = true;
			for (auto &i : vInventoryUseAmount)
			{
				if (i[1] != 0) { 
					allempty = false; 
				}
			}
			if (allempty) { 
				checkPlaceDelete = false; 
				bHUD_PlaceEntities->setInteractable(false);
			}
			//Remove the placement entity oEntityplacer
			delete oEntityPlacer;
			oEntityPlacer = nullptr;
			//Set actions buttons interaction to true
			if (vBombListCount > 0) { 
				bHUD_GoToAction->setInteractable(true);
				bHUD_DeleteEntities->setInteractable(true);
			}
			itemFollowMouse = false;
			buttonAction = 0;
			buttonActionEntity = 0;
			mMusicHandler->playSFX(mMusicHandler->sPlaceEntity);
		}

	}
	for (int i = 0; i < vBombListCount; ++i)
	{
		if (vBombList[i]->getBombType() == 1) {
			vBombList[i]->Render(rRend, tBombHorizontalTexture);
		}
		else {
			vBombList[i]->Render(rRend, tBombVerticalTexture);
		}
	}

	//Text
	teLevelText->Render(rRend);
	if (teLevelHint1->getHasText()) { teLevelHint1->Render(rRend); }
	if (teLevelHint2->getHasText()) { teLevelHint2->Render(rRend); }
	if (checkPlaceDelete) { tePlaceMode->Render(rRend); } else {  teDeleteMode->Render(rRend); }
	if (resetquestion) { 
		teResetLevelsWarningHigh->Render(rRend);
		teResetLevelsWarning->Render(rRend); 
	}
}


void Level::RenderPlayTest(SDL_Renderer* rRend)
{
	//HUD TEXTURES
	tHUD_PlayBG->Render(rRend, tHUD_PlayBG->getPosX(), tHUD_PlayBG->getPosY());
	tHUD_BottomBarPlaying->Render(rRend, tHUD_BottomBarPlaying->getPosX(), tHUD_BottomBarPlaying->getPosY());

	//HUD BUTTONS
	bHUD_GoToEditor->Render(rRend);


	//TILESET
	if (renderLevel) {
		for (int i = 0; i < vTilesetCount; ++i)
		{
			vTileset[i]->Render(rRend, tTileTexture, rTileClips[vTileset[i]->getType()]);
		}
	}

	//OBJECTS
	if (oTestPlayer != nullptr) { oTestPlayer->Render(rRend); }
	for (int i = 0; i < vBombListPlayCount; ++i)
	{
		if (vBombListPlay[i]->getBombType() == 1) {
			vBombListPlay[i]->Render(rRend, tBombHorizontalTexture);
		} else {
			vBombListPlay[i]->Render(rRend, tBombVerticalTexture);
		}

	}
	if (oPlayFinish != nullptr) { oPlayFinish->Render(rRend, tFinishTexture, rFinishClips[oPlayFinish->getFinishType()]); }

	if (finishlevel)
	{
		//tHUD_LevelComplete->Render(rRend, tHUD_LevelComplete->getPosX(), tHUD_LevelComplete->getPosY());
	}

	//Text
	teLevelText->Render(rRend);
	if (finishlevel) {
		teLevelCleared->Render(rRend);
	}
}

bool Level::getCloseGame() { return closeGameInstruction; }

void Level::LoadLevelNumber()
{
	SDL_RWops* file = SDL_RWFromFile("savadata.bin", "r+b");
	reachedLevel[0] = 1;
	if (file != nullptr) {
		SDL_RWread(file, &reachedLevel[0], sizeof(Sint32), 1);
		SDL_RWclose(file);
	}
	if (reachedLevel[0] > 1) { currentLevel = reachedLevel[0]; }
}

void Level::SaveLevel()
{
	//Get current level and save it
	reachedLevel[0] = currentLevel;
	SDL_RWops* file = SDL_RWFromFile("savadata.bin", "w+b");
	if (file != nullptr) {
		SDL_RWwrite(file, &reachedLevel[0], sizeof(Sint32), 1);
		//close handler
		SDL_RWclose(file);
	}
}

void Level::initLevelObjects(SDL_Renderer* rRend)
{
	bool levelLoaded = false;
	levelFilename = "";
	switch (currentLevel) {
		case 1:
			levelFilename = "level01.map";
			loadLevel01(rRend);
			levelLoaded = createLevel(rRend);
			break;
		case 2:
			levelFilename = "level02.map";
			loadLevel02(rRend);
			levelLoaded = createLevel(rRend);
			break;
		case 3:
			levelFilename = "level03.map";
			loadLevel03(rRend);
			levelLoaded = createLevel(rRend);
			break;
		case 4:
			levelFilename = "level04.map";
			loadLevel04(rRend);
			levelLoaded = createLevel(rRend);
			break;
		case 5:
			levelFilename = "level05.map";
			loadLevel05(rRend);
			levelLoaded = createLevel(rRend);
			break;
		case 6:
			levelFilename = "level06.map";
			loadLevel06(rRend);
			levelLoaded = createLevel(rRend);
			break;
		case 7:
			levelFilename = "level07.map";
			loadLevel07(rRend);
			levelLoaded = createLevel(rRend);
			break;
		case 8:
			levelFilename = "level08.map";
			loadLevel08(rRend);
			levelLoaded = createLevel(rRend);
			break;
		case 9:
			levelFilename = "level09.map";
			loadLevel09(rRend);
			levelLoaded = createLevel(rRend);
			break;
		case 10:
			levelFilename = "level10.map";
			loadLevel10(rRend);
			levelLoaded = createLevel(rRend);
			break;
		case 11:
			levelFilename = "level11.map";
			loadLevel11(rRend);
			levelLoaded = createLevel(rRend);
			break;
	}
	if (!renderLevel) { std::cout << "Failed creation level: " << currentLevel << std::endl; }
	else { std::cout << "Loaded level: " << currentLevel << std::endl; }
	loadinglevel = false;
}

void Level::loadLevel01(SDL_Renderer* rRend)
{
	//INVENTORY BUTTONS
	vInventoryButtons.push_back(new Button("bombx.png", rRend, 500, 738, Button::bEntity, Button::eBombHorizontal));		// NEW BUTTON
	vInventoryUseAmount.push_back(std::vector<int>(0));									// NEW vector 
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// BOMB TYPE 
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// AMOUNT USE
	vInventoryButtonsCount = vInventoryButtons.size();

	//Text
	teLevelText->RenderStringToTexture("Level 1: A booming start", rRend, ttfFONT, blackColor);
	teLevelHint1->RenderStringToTexture("Place the bomb somewhere below the player, it will launch you forward!", rRend, ttfFONT, blackColor);
	teLevelHint2->RenderStringToTexture("Use your mouse to navigate this game!", rRend, ttfFONT, blackColor);

	//OBJECTS
	if (oPlayer == nullptr) { oPlayer = new Player("player.png", rRend, 175, 300, Player::Facing::RIGHT); }
	if (oEditFinish == nullptr) { oEditFinish = new Finish(rRend, 642, 384, Finish::FTYPE::fRight); }
	if (tiLevelComplete == nullptr) { tiLevelComplete = new Timer(); }
}


void Level::loadLevel02(SDL_Renderer* rRend)
{
	vInventoryButtons.push_back(new Button("bombx.png", rRend, 500, 738, Button::bEntity, Button::eBombHorizontal));
	vInventoryUseAmount.push_back(std::vector<int>(0));
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// TYPE
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// USE
	vInventoryButtonsCount = vInventoryButtons.size();

	//Text
	teLevelText->RenderStringToTexture("Level 2: One bomb won't be enough!", rRend, ttfFONT, blackColor);
	teLevelHint1->RenderStringToTexture("You may use a limited amount of bombs in each level", rRend, ttfFONT, blackColor);
	teLevelHint2->RenderStringToTexture("It may takes a few tries before you get the right solution!", rRend, ttfFONT, blackColor);

	//OBJECTS
	if (oPlayer == nullptr) { oPlayer = new Player("player.png", rRend, 600, 136, Player::Facing::LEFT); }
	if (oEditFinish == nullptr) { oEditFinish = new Finish(rRend, 608, 512, Finish::FTYPE::fRight); }
	if (tiLevelComplete == nullptr) { tiLevelComplete = new Timer(); }
}

void Level::loadLevel03(SDL_Renderer* rRend)
{
	//INVENTORY BUTTONS
	vInventoryButtons.push_back(new Button("bombx.png", rRend, 464, 738, Button::bEntity, Button::eBombHorizontal));		// NEW BUTTON
	vInventoryUseAmount.push_back(std::vector<int>(0));									// NEW vector 
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// BOMB TYPE 
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// AMOUNT USE
	vInventoryButtonsCount = vInventoryButtons.size();

	//Text
	teLevelText->RenderStringToTexture("Level 3: Gravity exists", rRend, ttfFONT, blackColor);
	teLevelHint1->RenderStringToTexture("It matters when you hit the bomb in the air!", rRend, ttfFONT, blackColor);
	teLevelHint2->RenderStringToTexture(" ", rRend, ttfFONT, blackColor);

	//OBJECTS
	if (oPlayer == nullptr) { oPlayer = new Player("player.png", rRend, 480, 164, Player::Facing::LEFT); }
	if (oEditFinish == nullptr) { oEditFinish = new Finish(rRend, 224, 472, Finish::FTYPE::fLeft); }
	if (tiLevelComplete == nullptr) { tiLevelComplete = new Timer(); }
}

void Level::loadLevel04(SDL_Renderer* rRend)
{
	//INVENTORY BUTTONS
	vInventoryButtons.push_back(new Button("bombx.png", rRend, 500, 738, Button::bEntity, Button::eBombHorizontal));		// NEW BUTTON
	vInventoryUseAmount.push_back(std::vector<int>(0));									// NEW vector 
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// BOMB TYPE 
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// AMOUNT USE
	vInventoryButtonsCount = vInventoryButtons.size();

	//Text
	teLevelText->RenderStringToTexture("Level 4: Simple U turn", rRend, ttfFONT, blackColor);
	teLevelHint1->RenderStringToTexture("Sometimes it's better to place a bomb at a point where you have less speed!", rRend, ttfFONT, blackColor);
	teLevelHint2->RenderStringToTexture(" ", rRend, ttfFONT, blackColor);

	//OBJECTS
	if (oPlayer == nullptr) { oPlayer = new Player("player.png", rRend, 175, 210, Player::Facing::RIGHT); }
	if (oEditFinish == nullptr) { oEditFinish = new Finish(rRend, 158, 466, Finish::FTYPE::fLeft); }
	if (tiLevelComplete == nullptr) { tiLevelComplete = new Timer(); }
}

void Level::loadLevel05(SDL_Renderer* rRend)
{
	//INVENTORY BUTTONS
	vInventoryButtons.push_back(new Button("bomby.png", rRend, 384, 738, Button::bEntity, Button::eBombVertical));		// NEW BUTTON
	vInventoryUseAmount.push_back(std::vector<int>(0));									// NEW vector 
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// BOMB TYPE 
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// AMOUNT USE
	vInventoryButtonsCount = vInventoryButtons.size();

	//Text
	teLevelText->RenderStringToTexture("Level 5: Rocket Science", rRend, ttfFONT, blackColor);
	teLevelHint1->RenderStringToTexture("Y bombs makes you go up, this can be used to reach places above you.", rRend, ttfFONT, blackColor);
	teLevelHint2->RenderStringToTexture("Low falling speed means high launching speed!", rRend, ttfFONT, blackColor);

	//OBJECTS
	if (oPlayer == nullptr) { oPlayer = new Player("player.png", rRend, 384, 352, Player::Facing::RIGHT); }
	if (oEditFinish == nullptr) { oEditFinish = new Finish(rRend, 384, 200, Finish::FTYPE::fBelow); }
	if (tiLevelComplete == nullptr) { tiLevelComplete = new Timer(); }
}

void Level::loadLevel06(SDL_Renderer* rRend)
{
	vInventoryButtons.push_back(new Button("bombx.png", rRend, 500, 738, Button::bEntity, Button::eBombHorizontal));
	vInventoryUseAmount.push_back(std::vector<int>(0));
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// TYPE
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// USE

	vInventoryButtons.push_back(new Button("bomby.png", rRend, 600, 738, Button::bEntity, Button::eBombVertical));
	vInventoryUseAmount.push_back(std::vector<int>(0));
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// TYPE
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// USE
	vInventoryButtonsCount = vInventoryButtons.size();

	//Text
	teLevelText->RenderStringToTexture("Level 6: Combined forces", rRend, ttfFONT, blackColor);
	teLevelHint1->RenderStringToTexture("Use both bombs to reach a higher place!", rRend, ttfFONT, blackColor);
	teLevelHint2->RenderStringToTexture(" ", rRend, ttfFONT, blackColor);

	//OBJECTS
	if (oPlayer == nullptr) { oPlayer = new Player("player.png", rRend, 240, 430, Player::Facing::RIGHT); }
	if (oEditFinish == nullptr) { oEditFinish = new Finish(rRend, 480, 160, Finish::FTYPE::fBelow); }
	if (tiLevelComplete == nullptr) { tiLevelComplete = new Timer(); }
}

void Level::loadLevel07(SDL_Renderer* rRend)
{
	//INVENTORY BUTTONS
	vInventoryButtons.push_back(new Button("bombx.png", rRend, 464, 738, Button::bEntity, Button::eBombHorizontal));		// NEW BUTTON
	vInventoryUseAmount.push_back(std::vector<int>(0));									// NEW vector 
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// BOMB TYPE 
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// AMOUNT USE

	vInventoryButtons.push_back(new Button("bomby.png", rRend, 600, 738, Button::bEntity, Button::eBombVertical));
	vInventoryUseAmount.push_back(std::vector<int>(0));
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// TYPE
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// USE
	vInventoryButtonsCount = vInventoryButtons.size();

	//Text
	teLevelText->RenderStringToTexture("Level 7: Bunny hop", rRend, ttfFONT, blackColor);
	teLevelHint1->RenderStringToTexture("Sometimes you need both forces to move over an obstacle", rRend, ttfFONT, blackColor);
	teLevelHint2->RenderStringToTexture(" ", rRend, ttfFONT, blackColor);

	//OBJECTS
	if (oPlayer == nullptr) { oPlayer = new Player("player.png", rRend, 175, 350, Player::Facing::RIGHT); }
	if (oEditFinish == nullptr) { oEditFinish = new Finish(rRend, 608, 452, Finish::FTYPE::fAbove); }
	if (tiLevelComplete == nullptr) { tiLevelComplete = new Timer(); }
}

void Level::loadLevel08(SDL_Renderer* rRend)
{
	vInventoryButtons.push_back(new Button("bombx.png", rRend, 500, 738, Button::bEntity, Button::eBombHorizontal));
	vInventoryUseAmount.push_back(std::vector<int>(0));
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// TYPE
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(3);					// USE

	vInventoryButtons.push_back(new Button("bomby.png", rRend, 600, 738, Button::bEntity, Button::eBombVertical));
	vInventoryUseAmount.push_back(std::vector<int>(0));
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// TYPE
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// USE
	vInventoryButtonsCount = vInventoryButtons.size();

	//Text
	teLevelText->RenderStringToTexture("Level 8: Fire in the hole", rRend, ttfFONT, blackColor);
	teLevelHint1->RenderStringToTexture("Don't use too much force or you may overshoot", rRend, ttfFONT, blackColor);
	teLevelHint2->RenderStringToTexture(" ", rRend, ttfFONT, blackColor);

	//OBJECTS
	if (oPlayer == nullptr) { oPlayer = new Player("player.png", rRend, 144, 136, Player::Facing::RIGHT); }
	if (oEditFinish == nullptr) { oEditFinish = new Finish(rRend, 160, 512, Finish::FTYPE::fAbove); }
	if (tiLevelComplete == nullptr) { tiLevelComplete = new Timer(); }
}

void Level::loadLevel09(SDL_Renderer* rRend)
{
	vInventoryButtons.push_back(new Button("bombx.png", rRend, 500, 738, Button::bEntity, Button::eBombHorizontal));
	vInventoryUseAmount.push_back(std::vector<int>(0));
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// TYPE
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(3);					// USE

	vInventoryButtons.push_back(new Button("bomby.png", rRend, 600, 738, Button::bEntity, Button::eBombVertical));
	vInventoryUseAmount.push_back(std::vector<int>(0));
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// TYPE
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// USE
	vInventoryButtonsCount = vInventoryButtons.size();

	//Text
	teLevelText->RenderStringToTexture("Level 9: U are doing great", rRend, ttfFONT, blackColor);
	teLevelHint1->RenderStringToTexture(" ", rRend, ttfFONT, blackColor);
	teLevelHint2->RenderStringToTexture(" ", rRend, ttfFONT, blackColor);

	//OBJECTS
	if (oPlayer == nullptr) { oPlayer = new Player("player.png", rRend, 96, 100, Player::Facing::RIGHT); }
	if (oEditFinish == nullptr) { oEditFinish = new Finish(rRend, 700, 160, Finish::FTYPE::fRight); }
	if (tiLevelComplete == nullptr) { tiLevelComplete = new Timer(); }
}

void Level::loadLevel10(SDL_Renderer* rRend)
{
	vInventoryButtons.push_back(new Button("bombx.png", rRend, 500, 738, Button::bEntity, Button::eBombHorizontal));
	vInventoryUseAmount.push_back(std::vector<int>(0));
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// TYPE
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// USE

	vInventoryButtons.push_back(new Button("bomby.png", rRend, 600, 738, Button::bEntity, Button::eBombVertical));
	vInventoryUseAmount.push_back(std::vector<int>(0));
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// TYPE
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// USE
	vInventoryButtonsCount = vInventoryButtons.size();

	//Text
	teLevelText->RenderStringToTexture("Level 10: Reaching higher places", rRend, ttfFONT, blackColor);
	teLevelHint1->RenderStringToTexture(" ", rRend, ttfFONT, blackColor);
	teLevelHint2->RenderStringToTexture(" ", rRend, ttfFONT, blackColor);

																						//OBJECTS
	if (oPlayer == nullptr) { oPlayer = new Player("player.png", rRend, 608, 400, Player::Facing::LEFT); }
	if (oEditFinish == nullptr) { oEditFinish = new Finish(rRend, 160, 432, Finish::FTYPE::fAbove); }
	if (tiLevelComplete == nullptr) { tiLevelComplete = new Timer(); }
}

void Level::loadLevel11(SDL_Renderer* rRend)
{
	vInventoryButtons.push_back(new Button("bombx.png", rRend, 500, 738, Button::bEntity, Button::eBombHorizontal));
	vInventoryUseAmount.push_back(std::vector<int>(0));
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(1);					// TYPE
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(3);					// USE

	vInventoryButtons.push_back(new Button("bomby.png", rRend, 600, 738, Button::bEntity, Button::eBombVertical));
	vInventoryUseAmount.push_back(std::vector<int>(0));
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(2);					// TYPE
	vInventoryUseAmount[vInventoryUseAmount.size() - 1].push_back(3);					// USE
	vInventoryButtonsCount = vInventoryButtons.size();

	//Text
	teLevelText->RenderStringToTexture("Level 11: At a snails pace", rRend, ttfFONT, blackColor);
	teLevelHint1->RenderStringToTexture("Using Y bombs can be used to go down faster", rRend, ttfFONT, blackColor);
	teLevelHint2->RenderStringToTexture(" ", rRend, ttfFONT, blackColor);

	//OBJECTS
	if (oPlayer == nullptr) { oPlayer = new Player("player.png", rRend, 594, 130, Player::Facing::LEFT); }
	if (oEditFinish == nullptr) { oEditFinish = new Finish(rRend, 482, 416, Finish::FTYPE::fRight); }
	if (tiLevelComplete == nullptr) { tiLevelComplete = new Timer(); }
}

