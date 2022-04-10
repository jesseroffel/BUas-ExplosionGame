#pragma once

#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

//This class is for a big part written over from the lazyfoo.net tutorials based on timers.

class Timer
{
public:
	Timer();
	~Timer();


	void Start();
	void Paused();
	void Unpaused();
	void Stop();

	void SetTimer(float seconds);

	Uint32 getTicks();

	bool isStarted();
	bool isPaused();

	bool getActive();
	bool getTimerCompleted();

	void UpdateTimer();

	//how long was timer active
	double getTimerTicking();

private:
	//SDL Ticks stored when timer is started
	Uint32 mStartTicks;
	//Timer Ticks stored when timer is paused
	Uint32 mPausedTicks;

	//If a time is set this will store the time set
	Uint32 mTimerSetTicks;

	bool mStarted;
	bool mPaused;
	bool mActive;
	bool mTimeCompleted;
};

#endif