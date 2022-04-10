#pragma once

#ifndef MUSIC_H
#define MUSIC_H

#include <SDL.h>
#include <SDL_mixer.h>

#include <string>
#include <vector>

#include "timer.h"


class Music
{
public:

	enum eMusic {
		mEMPTY,
		mEditor,
		mPlaying
	};

	enum eSFX {
		sActionclap,
		sBounce,
		sError,
		sExplosion,
		sFinishLose,
		sFinishWin,
		sMouseHover,
		sMouseSelectButton,
		sMouseSelectInventory,
		sPlaceEntity,
	};

	Music();
	~Music();

	void playMusic(eMusic musicID);
	void pauseMusic();
	void stopMusic();

	void playSFX(eSFX sfxID);

	Mix_Music* loadMusic(std::string file);						// Load function for music
	Mix_Chunk* loadSFX(std::string file);						// Load function for soundfxs

	int getVolume();
	void setVolume(int newVolume);

	void fadeToMusic(eMusic musicID);

	void Update();

private:
	std::vector<Mix_Music*> vMusic;								// Stores all loaded mix_music object pointers that can be used
	std::vector<Mix_Chunk*> vSFX;								// Stores all loaded mix_chunk object pointers that can be used
	std::vector<double> vDurationPlaying;						// vector that stores the duration of each music file
	std::vector<double> vSecondsPlayed;							// vector that stores how long the current mmusic file was playing to resume at that point next time

	Timer* secondsPlaying;										// Timer used to capture seconds playing

	eMusic currentlyPlaying = eMusic::mEMPTY;
	int volumeMusic = 0;
	int volumeSFX = 0;
	bool makenewtimer = false;

	std::string folder = "files/sounds/";

	void updateTimePlaying();

	void getNewTimer();
};

#endif