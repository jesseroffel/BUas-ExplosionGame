#pragma once

#ifndef LEVEL_H
#define LEVEL_H

#include <SDL/SDL.h>
#include <fstream>
#include <vector>

#include "Button.h"
#include "tile.h"
#include "player.h"
#include "bomb.h"
#include "entityplacer.h"
#include "finish.h"
#include "timer.h"
#include "text.h"
#include "music.h"

//TILES CONSTANTS
const int TILE_SKY = 0;
const int TILE_SOLIDWALL = 1;
//GRASS
const int TILE_GRASSTOP = 2;
const int TILE_GRASSRIGHT = 3;
const int TILE_GRASSDOWN = 4;
const int TILE_GRASSLEFT = 5;
//CORNER
const int TILE_GRASSCORUPRIGHT = 6;
const int TILE_GRASSCORDOWNRIGHT= 7;
const int TILE_GRASSCORDOWNLEFT = 8;
const int TILE_GRASSCORUPLEFT = 9;

//FINISH CONSTANTS
const int FINISH_ABOVE = 1;
const int FINISH_LEFT = 2;
const int FINISH_RIGHT = 3;
const int FINISH_BELOW = 4;

const int MAX_LEVELS = 10;

enum PlayState {
	pEditor,
	pPlayTest,
};

class Level
{
public:
	PlayState currentPlayState;			// Holds the current enum value of PlayState, this is used for switching to/from edit/play mode
	PlayState checkPlayState;			// Holds the enum of PlayState, if this does not match with currentPlayState then the game will switch

	Level();
	Level(SDL_Renderer* rRend);
	~Level();

	void Input(SDL_Event* eEvent);

	void Update();

	void Render(SDL_Renderer* rRend);

	void SetPlayState(PlayState newplaystate);

	bool getCloseGame();

private:
	//General Game Objects
	Timer* tiLevelComplete;				// Timer Object that will hold the game from switching to the next levels for a few seconds.

	//Fonts
	TTF_Font* ttfFONT = nullptr;		// General font used

	//Text
	SDL_Color blackColor = { 0x00, 0x00, 0x00, 0xFF };
	SDL_Color WarningColor = { 0xFF, 0x00, 0x00, 0xFF };

	Text* teLevelText;					// Text object that is used for displaying the current version
	Text* teLevelHint1;					// Help text 1
	Text* teLevelHint2;					// Help text 2
	Text* teDeleteMode;					// Delete mode text
	Text* tePlaceMode;					// Place mode text
	Text* teLevelCleared;				// Level clear text
	Text* teResetLevelsWarning;			// Prompted when pressing r, asks if the player wants to go back to level 1
	Text* teResetLevelsWarningHigh;		// Highlighting 

	//Music
	Music* mMusicHandler;				// Music object that holds all the music and sound effects and handles the function calls for these sounds


	//********** TILE Objects
	std::vector<Tile*> vTileset;		// Actual tile objects inside a vector.
	std::string levelFolder;			// String used to get the right tileset map
	bool renderLevel;					// Render the tiles
	int vTilesetCount;					// Used for rendering

	Texture* tTileTexture;				// The whole tiles texture
	SDL_Rect rTileClips[10];			// Little pieces of the texture stored as clips to render each tile from the same texture
	Uint8 tTileTextureAlpha;			// Transparency of the texture used between edit and play mode

	//********** Level Objects
	EntityPlacer* oEntityPlacer;		// A object that holds information of the entity that is to be placed

	Player* oPlayer;					// Player object used in edit mode (can't move)
	Player* oTestPlayer;				// Player object used in play mode (can move)

	Finish* oEditFinish;				// Finish object used in edit mode
	Finish* oPlayFinish;				// Finish object used in play mode
	Texture* tFinishTexture;			// Actual finish texture
	SDL_Rect rFinishClips[5];			// And the matching clips instead of used seperate files

	SDL_Rect BoundryCheck;				// Rectangle used for checking if playing a new entity with the Entityplacer is not blocked by a tile

	//Make two lists, Editor with loaded and placed items and Playing will copy first list

	std::vector<Bomb*> vBombList;		// Vector that holds the bomb objects placed in edit mode
	std::vector<Bomb*> vBombListPlay;	// Vector that holds the bomb objects copied from edit mode
	Texture* tBombHorizontalTexture;	// Bomb X texture
	Texture* tBombVerticalTexture;		// Bomb Y texture

	int vBombListCount;					// Counter for vBombList
	int vBombListPlayCount;				// Counter for vBombListPlay

	//List of buttons with items that can be used in editor mode to place corresponding item

	std::vector<Button*> vInventoryButtons;				// Vector that holds all button objects that the player can use in a level
	std::vector<std::vector<int>> vInventoryUseAmount;  // A vector of int vectors that will be used like this: vInventoryUseAmount[number] and inside that: [0] = Buttontype [1] = amount to use
	int vInventoryButtonsCount;							// Counter for vInventoryButtons
	

	//********** HUD Objects EDITOR

	// HUD Bottom bar texture
	Texture* tHUD_BottomBarEdit;

	// HUD Paper background texture
	Texture* tHUD_EditBG;

	// HUD Edit lines texture
	Texture* tHUD_EditLines;

	// HUD GoToTesting Button
	Button* bHUD_GoToAction;

	//HUD Delete/Place Entities button
	Button* bHUD_DeleteEntities;
	Button* bHUD_PlaceEntities;

	//********** HUD Objects PLAY

	//HUD playing background texture
	Texture* tHUD_PlayBG;
	// HUD Boxes
	Texture* tHUD_LevelComplete;

	Texture* tHUD_BottomBarPlaying;

	// HUD GoToEditing Button
	Button* bHUD_GoToEditor;
	
	// string used for level loading
	std::string levelFilename;

	// Level
	int mLevelWidth = 800;
	int mLevelHeight = 704;
	Sint32 reachedLevel[1];

	//current level
	int currentLevel;
	int buttonAction;
	int buttonActionEntity;
	int newEntityType;
	int mousePosX;
	int mousePosY;

	bool switchingstage;
	bool loadinglevel;
	bool itemFollowMouse;
	bool createNewFollow;
	bool createNewEntity;
	bool resetquestion = false;
	bool hardreset = false;

	bool failedAttempt = false;

	bool finishlevel;
	bool lockSwitching = false;

	bool closeGameInstruction = false;

	//Enable/disable options
	bool EnableGrid = true;

	// DELETE = FALSE; PLACE = TRUE;
	bool checkPlaceDelete;

	//Load all game objects textures/data
	void loadGameObjects(SDL_Renderer* rRend);

	//Init tile objects
	void initTiles(SDL_Renderer* rRend);

	//Init used LevelObjects
	void initLevelObjects(SDL_Renderer* rRend);

	//Load Level
	bool createLevel(SDL_Renderer* rRend);

	//Check Input from Buttons
	void checkButtonValue();

	//Clear all current editor and playing objects and switches to a new level
	void SwapLevel(SDL_Renderer* rRend);

	//Clear playing objects
	void clearPlayingObjects();

	//Clear all loaded vectors and objects
	void clearLevel();

	//Copy Edit Objects and apply to Playing game
	void applyEditChanges(SDL_Renderer* rRend);

	//switch between playstates
	void switchStates(SDL_Renderer* rRend);

	//Handle Input Editor Objects
	void InputEditor(SDL_Event* eEvent);

	//Handle Input Playing/Testing Objects
	void InputPlayTest(SDL_Event* eEvent);

	//Update Editor Objects
	void UpdateEditor();

	//Update Playing/Testing Objects
	void UpdatePlayTest();

	//Render Editor Objects
	void RenderEditor(SDL_Renderer* rRend);
	
	//Render Playing/Testing Objects
	void RenderPlayTest(SDL_Renderer* rRend);

	//Get level from file
	void LoadLevelNumber();

	//Save last played level
	void SaveLevel();


	//Level loading functions
	void loadLevel01(SDL_Renderer* rRend);
	void loadLevel02(SDL_Renderer* rRend);
	void loadLevel03(SDL_Renderer* rRend);
	void loadLevel04(SDL_Renderer* rRend);
	void loadLevel05(SDL_Renderer* rRend);
	void loadLevel06(SDL_Renderer* rRend);
	void loadLevel07(SDL_Renderer* rRend);
	void loadLevel08(SDL_Renderer* rRend);
	void loadLevel09(SDL_Renderer* rRend);
	void loadLevel10(SDL_Renderer* rRend);
	void loadLevel11(SDL_Renderer* rRend);
};

#endif