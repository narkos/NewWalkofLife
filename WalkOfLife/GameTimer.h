//Edited by Oskar Hellström	
//Edited 2015-01-20  23:00


#include <windows.h>
#include <string.h>
#include <d3d11.h>
#include <d3dcompiler.h>

class GameTimer
{
public:
	GameTimer();

	float GameTime()const; // in seconds
	float TotalTime()const;
	float DeltaTime()const;
	void Reset(); // Resets GameTimer
	void Tick(); // Counts time
	void setPausedTime(__int64 Time);
	void setCurrTime(float Time);
	void Start(float);
	void Stop();
	float getPauseTime(float pTime);

private:
	double mSecondsPerCount;
	//Time Variables must be int64 because its the format they are aquiered from
	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;
	bool mStopped;
	double mDeltaTime;


};
