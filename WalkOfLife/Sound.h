#pragma once
#include <string>
#include <windows.h>
#include <mmsystem.h>
#include <strmif.h>
#include <control.h>
#include <uuids.h>

#pragma comment(lib,"strmiids.lib")

using namespace std;

class Sound
{
public:
	void InitMp3();
	void CleanupMp3();
	void LoadMp3(char filename[]);
	void PlayMp3();
	void PauseMp3();
	void StopMp3();
	void setVolume(long);
	long getVolume();
	void daCapo();
	LONGLONG getDuration();

	IGraphBuilder* g_pGraphBuilder = NULL;
	IMediaControl* g_pPlaybackControl = NULL;
	IBasicAudio*   g_pVolumeControl = NULL;
	IMediaSeeking* g_pMediaSeeking = NULL;
	int			   g_bReady = 0;
	float soundTime = 0;
	LONGLONG duration;

	Sound();
	~Sound();
};

