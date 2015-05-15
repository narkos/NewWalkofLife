//Edited by Oskar Hellström	
//Edited 2015-01-20  23:00


#include "GameTimer.h"

//GameTimer constructor
GameTimer::GameTimer()
	: mSecondsPerCount(0.0), mBaseTime(0),
	mPausedTime(0), mPrevTime(0), mCurrTime(0)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}


void GameTimer::Tick()
{

	// Get the time this frame.
	//WindowsFunction
	/*if (mStopped)
	{
	mDeltaTime = 0.0;
	return;
	}*/
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;
	// Time difference between this frame and the previous.

	// Prepare for next frame.
	mPrevTime = mCurrTime;


}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;

}




float GameTimer::TotalTime()const
{
	/*if (mStopped)
	{
	return (float)(((mStopTime - mPausedTime) -
	mBaseTime)*mSecondsPerCount);
	}
	else
	{*/

	return (float)(((mCurrTime - mPausedTime) -
		mBaseTime)*mSecondsPerCount);
	//}
}
void GameTimer::setPausedTime(__int64 Time)
{
	mPausedTime = mCurrTime - Time;
}

float GameTimer::getPauseTime(float pTime){
	return pTime;
}

void GameTimer::setCurrTime(float Time)
{
	mPrevTime = Time;
}

void GameTimer::Start(float ptime)
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	if (mStopped)
		mPausedTime += (startTime - mStopTime);
	mPrevTime = ptime;
	mStopTime = 0;
	mStopped = false;

}

void GameTimer::Stop()
{
	if (!mStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mStopTime = mCurrTime;
		mStopped = true;

	}
}

float GameTimer::DeltaTime()const
{
	return (float)mDeltaTime;
}