#include "music.h"

Music::Music()
{
	//Load all music
	vMusic.push_back(loadMusic("Editor.mp3"));
	vDurationPlaying.push_back(175);
	vSecondsPlayed.push_back(0);
	vMusic.push_back(loadMusic("Action.mp3"));
	vDurationPlaying.push_back(87);
	vSecondsPlayed.push_back(0);
	//Load all sound effects
	vSFX.push_back(loadSFX("Actionclap.aif"));
	vSFX.push_back(loadSFX("Bouncingball.aif"));
	vSFX.push_back(loadSFX("Error.aif"));
	vSFX.push_back(loadSFX("Explosion.aif"));
	vSFX.push_back(loadSFX("Looser.aif"));
	vSFX.push_back(loadSFX("Winner.aif"));
	vSFX.push_back(loadSFX("Hover.aif"));
	vSFX.push_back(loadSFX("Select1.aif"));
	vSFX.push_back(loadSFX("Select2.aif"));
	vSFX.push_back(loadSFX("Select3.aif"));
	setVolume(75);
	volumeSFX = 80;
	currentlyPlaying = eMusic::mEMPTY;

	Mix_VolumeMusic(volumeMusic);
}


Music::~Music()
{
	delete secondsPlaying;
	//Delete Music using iterator
	for (int i = 0; i < vMusic.size(); i++)
	{
		Mix_FreeMusic(vMusic[i]);
	}
	vMusic.clear();
	for (int i = 0; i < vSFX.size(); i++)
	{
		Mix_FreeChunk(vSFX[i]);
	}
	vSFX.clear();
	vDurationPlaying.clear();
}


void Music::playMusic(eMusic musicID)
{
	if (musicID == mEMPTY) {
		stopMusic();
	} else {
		getNewTimer();
		Mix_PlayMusic(vMusic[musicID - 1], -1);
		currentlyPlaying = musicID;
		secondsPlaying->SetTimer(vDurationPlaying[currentlyPlaying - 1]);
	}
}

void Music::pauseMusic()
{

}

void Music::stopMusic()
{
	Mix_HaltMusic();

}

void Music::playSFX(eSFX sfxID)
{
	Mix_VolumeChunk(vSFX[sfxID], volumeSFX);
	Mix_PlayChannel(-1, vSFX[sfxID], 0);
}

Mix_Music* Music::loadMusic(std::string file)
{
	std::string filepath = folder + file;
	return Mix_LoadMUS(filepath.c_str());
}

Mix_Chunk* Music::loadSFX(std::string file)
{
	std::string filepath = folder + file;
	return Mix_LoadWAV(filepath.c_str());
}

int Music::getVolume() { return volumeMusic; }

void Music::setVolume(int newVolume) { volumeMusic = newVolume; }

void Music::fadeToMusic(eMusic musicID)
{
	if (Mix_PlayingMusic()) {
		stopMusic();
	}
	getNewTimer();
	Mix_FadeInMusicPos(vMusic[musicID - 1], -1, 2000, vSecondsPlayed[musicID -1]);
	secondsPlaying->SetTimer(vDurationPlaying[currentlyPlaying - 1] - vSecondsPlayed[currentlyPlaying - 1]);
	currentlyPlaying = musicID;
}

void Music::Update() {
	if (Mix_PlayingMusic()) {
		if (secondsPlaying) { 
			secondsPlaying->UpdateTimer(); 
			if (secondsPlaying->getTimerCompleted()) {
				vSecondsPlayed[currentlyPlaying - 1] = 0;
				makenewtimer = true;
			}
		}
		if (makenewtimer) {
			makenewtimer = false;
			delete secondsPlaying;
			secondsPlaying = new Timer();
			secondsPlaying->SetTimer(vDurationPlaying[currentlyPlaying - 1]);
		}
	}
}

void Music::updateTimePlaying() {
	if (currentlyPlaying != eMusic::mEMPTY) {
		double timenow = (secondsPlaying->getTimerTicking()) / 1000;
		double timesaved = vSecondsPlayed[currentlyPlaying - 1];
		vSecondsPlayed[currentlyPlaying - 1] = timenow + timesaved;
	}
}

void Music::getNewTimer() {
	if (secondsPlaying != nullptr) {
		updateTimePlaying();
		delete secondsPlaying;
		secondsPlaying = nullptr;
	}
	secondsPlaying = new Timer();
}