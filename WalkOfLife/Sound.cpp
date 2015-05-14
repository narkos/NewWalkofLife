#include "Sound.h"


Sound::Sound()
{
}


Sound::~Sound()
{
}

void Sound::InitMp3()
{
	CoInitialize(NULL);
}

void Sound::LoadMp3(char filename[])
{
	// DirectShow requires unicode (16bit) characters rather than 8bit ascii
	WCHAR wFile[MAX_PATH];

	// convert ascii to unicode
	MultiByteToWideChar(CP_ACP, 0, filename, -1, wFile, MAX_PATH);

	// create a graph build to allow us to make the audio signal path
	if (SUCCEEDED(CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder,
		(void**)&g_pGraphBuilder)))
	{
		// create a simple controller for the sound file
		g_pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&g_pPlaybackControl);

		// create a simple interface for volume and balance
		g_pGraphBuilder->QueryInterface(IID_IBasicAudio, (void **)&g_pVolumeControl);

		// load and start playback of the specified sound file
		if (SUCCEEDED(g_pGraphBuilder->RenderFile(wFile, NULL)))
		{
			g_bReady = 1;
		}
	}
}

void Sound::CleanupMp3()
{
	// stop playback of file if playing
	if (g_pPlaybackControl)
	{
		g_pPlaybackControl->Stop();
	}

	// release the volume controller
	if (g_pVolumeControl)
	{
		g_pVolumeControl->Release();
		g_pVolumeControl = NULL;
	}

	// release the playback controller
	if (g_pPlaybackControl)
	{
		g_pPlaybackControl->Release();
		g_pPlaybackControl = NULL;
	}

	// release the graph builder 
	if (g_pGraphBuilder)
	{
		g_pGraphBuilder->Release();
		g_pGraphBuilder = NULL;
	}

	// Unload DirectShow
	CoUninitialize();
}

void Sound::PlayMp3()
{
	if (g_bReady)
	{
		g_pPlaybackControl->Run();
	}
}

void Sound::PauseMp3()
{
	if (g_bReady)
	{
		g_pPlaybackControl->Pause();
	}
}

void Sound::StopMp3()
{
	if (g_bReady)
	{
		g_pPlaybackControl->Stop();
	}
}

void Sound::setVolume(long vol)
{
	if (g_bReady)
	{
		g_pVolumeControl->put_Volume(vol);
	}
}

long Sound::getVolume()
{
	long vol = 0;
	if (g_bReady)
	{
		g_pVolumeControl->get_Volume(&vol);
	}
	return vol;
}