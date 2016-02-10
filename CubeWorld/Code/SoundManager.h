#ifndef _AUDIO_MANAGER__H
#define _AUDIO_MANAGER__H

#include <d3dx10.h>

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>

#include "Wavefile.h"

#include <vector>
using std::vector;

//-----------------------------------------------------------------------------
// Global defines
//-----------------------------------------------------------------------------
#define INPUTCHANNELS 2
#define OUTPUTCHANNELS 8

#define NUM_PRESETS 30

// Constants to define our world space
const int WORLD_XMIN = -10;
const int WORLD_XMAX = 10;
const int WORLD_ZMIN = -10;
const int WORLD_ZMAX = 10;

#define SOUNDMANAGER SoundManager::GetInstance()

// Must match order of g_PRESET_NAMES
static XAUDIO2FX_REVERB_I3DL2_PARAMETERS g_PRESET_PARAMS[ NUM_PRESETS ] =
{
    XAUDIO2FX_I3DL2_PRESET_DEFAULT,
    XAUDIO2FX_I3DL2_PRESET_GENERIC,
    XAUDIO2FX_I3DL2_PRESET_PADDEDCELL,
    XAUDIO2FX_I3DL2_PRESET_ROOM,
    XAUDIO2FX_I3DL2_PRESET_BATHROOM,
    XAUDIO2FX_I3DL2_PRESET_LIVINGROOM,
    XAUDIO2FX_I3DL2_PRESET_STONEROOM,
    XAUDIO2FX_I3DL2_PRESET_AUDITORIUM,
    XAUDIO2FX_I3DL2_PRESET_CONCERTHALL,
    XAUDIO2FX_I3DL2_PRESET_CAVE,
    XAUDIO2FX_I3DL2_PRESET_ARENA,
    XAUDIO2FX_I3DL2_PRESET_HANGAR,
    XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY,
    XAUDIO2FX_I3DL2_PRESET_HALLWAY,
    XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR,
    XAUDIO2FX_I3DL2_PRESET_ALLEY,
    XAUDIO2FX_I3DL2_PRESET_FOREST,
    XAUDIO2FX_I3DL2_PRESET_CITY,
    XAUDIO2FX_I3DL2_PRESET_MOUNTAINS,
    XAUDIO2FX_I3DL2_PRESET_QUARRY,
    XAUDIO2FX_I3DL2_PRESET_PLAIN,
    XAUDIO2FX_I3DL2_PRESET_PARKINGLOT,
    XAUDIO2FX_I3DL2_PRESET_SEWERPIPE,
    XAUDIO2FX_I3DL2_PRESET_UNDERWATER,
    XAUDIO2FX_I3DL2_PRESET_SMALLROOM,
    XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM,
    XAUDIO2FX_I3DL2_PRESET_LARGEROOM,
    XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL,
    XAUDIO2FX_I3DL2_PRESET_LARGEHALL,
    XAUDIO2FX_I3DL2_PRESET_PLATE,
};

struct SOUND_DATA
{
	CWaveFile*				m_Wave;
	char					m_waveName[MAX_PATH];
	BYTE*                   m_pbSampleData;

	WAVEFORMATEX*			m_WaveFormat;
	DWORD					m_cbWaveSize;

	SOUND_DATA()
	{
		strcpy_s(m_waveName, sizeof(m_waveName), "NOT_SET");
		m_pbSampleData = NULL;
	}

	~SOUND_DATA()
	{
		if(m_pbSampleData)
		{
			delete [] m_pbSampleData;
			m_pbSampleData = NULL;
		}

		if(m_Wave)
		{
			delete m_Wave;
			m_Wave = NULL;
		}
	}
};

struct SOUND
{
private:
	friend class SoundManager;
	SOUND_DATA*				m_pSoundData;
	IXAudio2SourceVoice*    m_pSourceVoice;
	X3DAUDIO_EMITTER        m_emitter;
    X3DAUDIO_CONE           m_emitterCone;
	D3DXVECTOR3             mPosition;
	bool					mStarted;

	FLOAT32					m_emitterAzimuths[INPUTCHANNELS];

public:
	SOUND()
	{
		mStarted = false;
		m_pSoundData = NULL;
		m_pSourceVoice = NULL;

		ZeroMemory(&m_emitter, sizeof(X3DAUDIO_EMITTER));
		ZeroMemory(&m_emitterCone, sizeof(X3DAUDIO_CONE));

		mPosition = D3DXVECTOR3(0,0,0);
		ZeroMemory(&m_emitterAzimuths, sizeof(m_emitterAzimuths));
	};

	~SOUND()
	{
		if(m_pSourceVoice)
			m_pSourceVoice->DestroyVoice();
	}

	D3DXVECTOR3 GetPosition() { return mPosition; }
	void SetPosition(D3DXVECTOR3 pos) { mPosition = pos; }

	void SetVolume(float _volume)
	{
		m_pSourceVoice->SetVolume(_volume);
	}

	float GetVolume()
	{
		float vol;
		m_pSourceVoice->GetVolume(&vol);

		return vol;
	}
};

#define SOUND_DATA_CACHE_LIST vector<SOUND_DATA*>
#define SOUND_LIST vector<SOUND*>

class SoundManager
{
	SOUND*					m_BackgroundSound;
	char					m_NewBackgroundSound[MAX_PATH];

	bool					m_AllowPlaying;
    bool m_bInitialized;

	SOUND_LIST				m_Sounds;
	SOUND_DATA_CACHE_LIST	m_SoundCache;
    // XAudio2
    IXAudio2*               m_pXAudio2;
    IXAudio2MasteringVoice* m_pMasteringVoice;
    IXAudio2SubmixVoice*    m_pSubmixVoice;
    IUnknown*               m_pReverbEffect;

    // 3D
    X3DAUDIO_HANDLE         m_x3DInstance;
    int                     m_nFrameToApply3DAudio;

    DWORD                   m_dwChannelMask;
    UINT32                  m_nChannels;

	X3DAUDIO_DSP_SETTINGS   m_dspSettings;
    X3DAUDIO_LISTENER       m_listener;

    D3DXVECTOR3             mListenerPos;
	D3DXVECTOR3				mListenerUp;
	D3DXVECTOR3				mListenerLook;

	FLOAT32					m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];

	SOUND_DATA*				GetSoundData(char* wavename);

	SoundManager();

	static SoundManager*	instance;

	HRESULT					GetFilenamesInDirectory(char* folderPath, char* fileType,
								std::vector<std::string>& listToFill);

public:
	static SoundManager* GetInstance();
	~SoundManager();

	static void Release();

	HRESULT Init();
	SOUND* PlaySound( char* wavename, UINT32 _channelCount = 2, D3DXVECTOR3 position = D3DXVECTOR3(0,0,0),
		D3DXVECTOR3 up = D3DXVECTOR3(0,1,0), D3DXVECTOR3 look = D3DXVECTOR3(0,0,1), UINT32 loopCount = 0 /* 255u == infinite */ );
	HRESULT CacheAudio( char* wavename );
	HRESULT Update( double fElapsedTime );
	HRESULT SetReverb( int nReverb );
	void PauseAudio( bool resume );
	void CleanupAudio();

	D3DXVECTOR3 GetListenerPos();
	D3DXVECTOR3	GetListenerUp();
	D3DXVECTOR3 GetListenerLook();

	void SetListenerPos(D3DXVECTOR3 pos);
	void SetListenerUp(D3DXVECTOR3 up);
	void SetListenerLook(D3DXVECTOR3 look);

	int NumPlayingSounds();

	void DestroyAllSounds(bool _ignore_bg_sound = true);

	void SetAllowPlaying(bool _allow);
	bool GetAllowPlaying();

	void SetNewBackgroundSound(char* _fileName);
	
	void SetBackgroundSoundVolume(float _volume);
	float GetBackgroundSoundVolume();

	void SetGlobalVolume(float _volume);
	float GetGlobalVolume();

	void StopAllSounds();

	HRESULT CacheFolder(char* folderPath);
};
#endif
