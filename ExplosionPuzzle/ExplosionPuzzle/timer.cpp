#include "timer.h"



Timer::Timer()
{
	mStartTicks = 0;
	mPausedTicks = 0;
	mTimerSetTicks = 0;

	mStarted = false;
	mPaused = false;
	mActive = false;
	mTimeCompleted = false;
}


Timer::~Timer()
{
}


void Timer::Start()
{
	mStarted = true;
	mPaused = false;
	mActive = true;

	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void Timer::Paused()
{
	if (mStarted && !mPaused) 
	{
		mPaused = true;

		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void Timer::Unpaused()
{
	if (mStarted && mPaused) 
	{
		mPaused = false;
		mStartTicks = SDL_GetTicks() - mPausedTicks;
	}
}

void Timer::Stop()
{
	mStarted = false;
	mPaused = false;
	mActive = false;

	mStartTicks = 0;
	mPausedTicks = 0;
}

Uint32 Timer::getTicks()
{
	Uint32 ticks = 0;
	
	if (mStarted)
	{
		if (mPaused) { ticks = mPausedTicks; } 
		else { ticks = SDL_GetTicks() - mStartTicks; }
	}

	return ticks;
}

void Timer::SetTimer(float seconds)
{
	mStarted = true;
	mActive = true;
	mPaused = false;
	mTimeCompleted = false;

	mStartTicks = SDL_GetTicks();
	mTimerSetTicks = mStartTicks + (1000 * seconds);
	mPausedTicks = 0;
}

void Timer::UpdateTimer()
{
	if (mStarted && mTimeCompleted == false) {
		if (mTimerSetTicks != 0)
		{
			Uint32 checktimer = SDL_GetTicks();
			if (checktimer > mTimerSetTicks) {
				mTimeCompleted = true;
				mStartTicks = 0;
				mPausedTicks = 0;
				mTimerSetTicks = 0;
			}
		}
	}
}

bool Timer::isStarted() { return mStarted; }

bool Timer::isPaused() { return mPaused; }

bool Timer::getActive() { return mActive; }

bool Timer::getTimerCompleted() { return mTimeCompleted; }

double Timer::getTimerTicking() { 
	return SDL_GetTicks() - mStartTicks;
}