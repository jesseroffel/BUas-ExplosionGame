#include "game.h"

ScreenManager* Game::oScreenManager = new ScreenManager();

Game::Game()
{
	//set variables
	closeGameCommand = false;
	//Init SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO);
	//Init SDL Window
	wWindow = SDL_CreateWindow("Explosion Puzzle - Submission version 2 - Jesse Roffel", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_SHOWN);
	if (wWindow == nullptr) { closeGameCommand = true; }
	//Init SDL Renderer
	rRenderer = SDL_CreateRenderer(wWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//Make SDL Event Object
	//Init SDL Image PNG
	int imgFlag = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlag) & imgFlag)) { closeGameCommand = true; }
	//Init TTF
	if (TTF_Init() == -1) { closeGameCommand = true; }
	//Init Audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) { closeGameCommand = true; }

	//Make ScreenManager object
	oScreenManager = new ScreenManager(rRenderer);
}


Game::~Game()
{
	Close();
	delete oScreenManager;
}


void Game::MainLoop()
{
	while (!closeGameCommand) {
		SDL_RenderClear(rRenderer);

		Input();
		Update();
		Render(rRenderer);

		SDL_RenderPresent(rRenderer);
	}			
}

void Game::Input()
{
	SDL_Event eEvent;
	while (SDL_PollEvent(&eEvent) != 0) {
		oScreenManager->Input(&eEvent);
		switch (eEvent.type)
		{
		case SDL_QUIT:
			closeGameCommand = true; 
			break;
		}
	}
}

void Game::InputMenu()
{
	//TODO Menu icon 'mover'
}

void Game::Update()
{
	oScreenManager->Update();
	if (oScreenManager->getCloseGame()) { closeGameCommand = true; }
}

void Game::Render(SDL_Renderer* rRend)
{
	oScreenManager->Render(rRend);
}


void Game::Close()
{
	//Free image objects

	//Free sound objects

	//Destroy (SDL) objects used
	SDL_DestroyRenderer(rRenderer);
	SDL_DestroyWindow(wWindow);

	//Stop SDL Systems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}