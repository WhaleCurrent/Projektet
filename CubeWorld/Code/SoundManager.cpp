#include "SoundManager.h"

SoundManager* SoundManager::instance = NULL;
SoundManager* SoundManager::GetInstance()
{
	if(!instance)
	{
		instance = myNew SoundManager();
	}

	return instance;
}

void SoundManager::Release()
{
	SAFE_DELETE(instance);
}

SoundManager::SoundManager()
{
    m_bInitialized = false;

    // XAudio2
    m_pXAudio2 = NULL;
    m_pMasteringVoice = NULL;
    m_pSubmixVoice = NULL;
    m_pReverbEffect = NULL;

	strcpy_s(m_NewBackgroundSound, sizeof(m_NewBackgroundSound), "NULL");
	m_BackgroundSound = NULL;

    // 3D
    ZeroMemory(m_x3DInstance, sizeof(X3DAUDIO_HANDLE));
    m_nFrameToApply3DAudio = 0;

    m_dwChannelMask = 0;
    m_nChannels = 0;

    ZeroMemory(&m_dspSettings, sizeof(X3DAUDIO_DSP_SETTINGS));
    ZeroMemory(&m_listener, sizeof(X3DAUDIO_LISTENER));

    SetListenerPos(D3DXVECTOR3(0,0,0));
	SetListenerUp(D3DXVECTOR3(0,1,0));
	SetListenerLook(D3DXVECTOR3(0,0,1));

	ZeroMemory(&m_matrixCoefficients, sizeof(m_matrixCoefficients));

	m_Sounds.clear();
	m_SoundCache.clear();

	SetAllowPlaying(true);
}

SoundManager::~SoundManager()
{

	if(m_BackgroundSound)
	{
		m_BackgroundSound->m_pSourceVoice->Stop();

		strcpy_s(m_NewBackgroundSound, sizeof(m_NewBackgroundSound), "NULL");
	}

	m_BackgroundSound = NULL;

	SOUND_DATA_CACHE_LIST::iterator sc = m_SoundCache.begin();
	while(sc != m_SoundCache.end())
	{
		SAFE_DELETE(*sc);
		sc++;
	}
	m_SoundCache.clear();

	SOUND_LIST::iterator s = m_Sounds.begin();
	while(s != m_Sounds.end())
	{
		SAFE_DELETE(*s);
		s++;
	}
	m_Sounds.clear();

	CleanupAudio();
}

void SoundManager::StopAllSounds()
{
	SOUND_LIST::iterator s = m_Sounds.begin();
	while(s != m_Sounds.end())
	{
		if((*s) != m_BackgroundSound)
			SAFE_DELETE(*s);
		
		s++;
	}
	m_Sounds.clear();

	if(m_BackgroundSound)
		m_Sounds.push_back(m_BackgroundSound);
}

HRESULT SoundManager::Init()
{
   //
    // Initialize XAudio2
    //
    CoInitializeEx( NULL, COINIT_MULTITHREADED );

    UINT32 flags = 0;
#ifdef _DEBUG
    flags |= XAUDIO2_DEBUG_ENGINE;
#endif

    HRESULT hr;

    if ( FAILED(hr = XAudio2Create( &m_pXAudio2, flags ) ) )
        return hr;

    //
    // Create a mastering voice
    //
    if ( FAILED(hr = m_pXAudio2->CreateMasteringVoice( &m_pMasteringVoice ) ) )
    {
        SAFE_RELEASE( m_pXAudio2 );
        return hr;
    }

    // Check device details to make sure it's within our sample supported parameters
    XAUDIO2_DEVICE_DETAILS details;
    if ( FAILED(hr = m_pXAudio2->GetDeviceDetails( 0, &details ) ) )
    {
        SAFE_RELEASE( m_pXAudio2 );
        return hr;
    }

    if ( details.OutputFormat.Format.nChannels > OUTPUTCHANNELS )
    {
        SAFE_RELEASE( m_pXAudio2 );
        return E_FAIL;
    }

    m_dwChannelMask = details.OutputFormat.dwChannelMask;
    m_nChannels = details.OutputFormat.Format.nChannels;

    //
    // Create reverb effect
    //
    flags = 0;
#ifdef _DEBUG
    flags |= XAUDIO2FX_DEBUG;
#endif

    if ( FAILED(hr = XAudio2CreateReverb( &m_pReverbEffect, flags ) ) )
    {
        SAFE_RELEASE( m_pXAudio2 );
        return hr;
    }

    //
    // Create a submix voice
    //

    // Reverb effect in XAudio2 currently only supports mono channel count
    XAUDIO2_EFFECT_DESCRIPTOR effects[] = { { m_pReverbEffect, TRUE, 1 } };
    const XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };
    
    if( FAILED(hr = m_pXAudio2->CreateSubmixVoice( &m_pSubmixVoice, 1,
                          details.OutputFormat.Format.nSamplesPerSec, 0, 0, NULL, &effectChain ) ) )
    {
        SAFE_RELEASE( m_pXAudio2 );
        SAFE_RELEASE( m_pReverbEffect );
        return hr;
    }

    // Set default FX params
    XAUDIO2FX_REVERB_PARAMETERS native;
    ReverbConvertI3DL2ToNative( &g_PRESET_PARAMS[0], &native );
    m_pSubmixVoice->SetEffectParameters( 0, &native, sizeof(native) );

    //
    // Initialize X3DAudio
    //  Speaker geometry configuration on the final mix, specifies assignment of channels
    //  to speaker positions, defined as per WAVEFORMATEXTENSIBLE.dwChannelMask
    //
    //  SpeedOfSound - speed of sound in user-defined world units/second, used
    //  only for doppler calculations, it must be >= FLT_MIN
    //
    const float SPEEDOFSOUND = X3DAUDIO_SPEED_OF_SOUND;

    X3DAudioInitialize( details.OutputFormat.dwChannelMask, SPEEDOFSOUND, m_x3DInstance );

    mListenerPos = D3DXVECTOR3(0,0, float(WORLD_ZMIN));

    //
    // Setup 3D audio structs
    //
    m_listener.Position              = mListenerPos;
    m_listener.OrientFront           = mListenerLook;
    m_listener.OrientTop             = mListenerUp;

    m_dspSettings.SrcChannelCount    = INPUTCHANNELS;
    m_dspSettings.DstChannelCount    = m_nChannels;
    m_dspSettings.pMatrixCoefficients = m_matrixCoefficients;

    //
    // Done
    //
    m_bInitialized = true;

    return S_OK;
}

int SoundManager::NumPlayingSounds()
{
	return (int)m_Sounds.size();
}

HRESULT SoundManager::CacheAudio( char* wavename )
{
	if(GetSoundData( wavename ) != 0)
		return S_OK;

	return E_FAIL;
}

SOUND* SoundManager::PlaySound( char* wavename, UINT32 _channelCount, D3DXVECTOR3 position,
								D3DXVECTOR3 up, D3DXVECTOR3 look, UINT32 loopCount )
{
	//return NULL;
/*
#ifdef _DEBUG
	return NULL;
#endif
*/

    if( !m_bInitialized || !m_AllowPlaying )
        return NULL;

	SOUND* s = myNew SOUND();

	s->SetPosition(position);
    s->m_emitter.Position               = s->GetPosition();
    s->m_emitter.OrientFront            = look;
    s->m_emitter.OrientTop              = up;
    s->m_emitter.ChannelCount           = _channelCount; //INPUTCHANNELS;
	s->m_emitter.ChannelRadius          = _channelCount == 1 ? 1.0f : 0.0f;
    s->m_emitter.CurveDistanceScaler    = 14.0f;
    s->m_emitter.DopplerScaler          = 1.0f;
	s->m_emitter.pCone                  = &s->m_emitterCone;
    
	s->m_emitter.pCone->InnerAngle      = 0.0f;
											// Setting the inner cone angles to X3DAUDIO_2PI and
											// outer cone other than 0 causes
											// the emitter to act like a point emitter using the
											// INNER cone settings only.
	s->m_emitter.pCone->OuterAngle      = 0.0f;
											// Setting the outer cone angles to zero causes
											// the emitter to act like a point emitter using the
											// OUTER cone settings only.
	s->m_emitter.pCone->InnerVolume     = 1.0f;
	s->m_emitter.pCone->OuterVolume     = 1.0f;
	s->m_emitter.pCone->InnerLPF        = 0.0f;
	s->m_emitter.pCone->OuterLPF        = 1.0f;
	s->m_emitter.pCone->InnerReverb     = 0.0f;
	s->m_emitter.pCone->OuterReverb     = 1.0f;

    s->m_emitter.pChannelAzimuths       = s->m_emitterAzimuths;

    //
    // Search for media
    //

    //char strFilePath[ MAX_PATH ];
    //char wavFilePath[ MAX_PATH ];

	//strcpy( wavFilePath, "D:/XAudio2/Wavs/Electro_1.wav");
    //StringCchCopy( wavFilePath, MAX_PATH, "Media\\Wavs\\" );
    //StringCchCat( wavFilePath, MAX_PATH, wavname );

    //HRESULT hr;

    //V_RETURN( DXUTFindDXSDKMediaFileCch( strFilePath, MAX_PATH, wavFilePath ) );
	SOUND_DATA* sd = GetSoundData(wavename);
	s->m_pSoundData = sd;

    //
    // Play the wave using a XAudio2SourceVoice
    //
    //const IXAudio2Voice* voices[] = { m_pMasteringVoice, m_pSubmixVoice };

	XAUDIO2_SEND_DESCRIPTOR sendDesc[2];
	sendDesc[0].Flags = 0;
	sendDesc[0].pOutputVoice = m_pMasteringVoice;
	sendDesc[1].Flags = 0;
	sendDesc[1].pOutputVoice = m_pSubmixVoice;

    const XAUDIO2_VOICE_SENDS sendList = { 2, sendDesc };
    // Create the source voice
    if(FAILED(m_pXAudio2->CreateSourceVoice( &s->m_pSourceVoice, sd->m_WaveFormat, 0,
                          XAUDIO2_DEFAULT_FREQ_RATIO, NULL, &sendList ) ))
	{
		SAFE_DELETE(s);
		return NULL;
	}

    // Submit the wave sample data using an XAUDIO2_BUFFER structure
    XAUDIO2_BUFFER buffer = {0};
    buffer.pAudioData = sd->m_pbSampleData;
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.AudioBytes = sd->m_cbWaveSize;
    buffer.LoopCount = loopCount; //XAUDIO2_LOOP_INFINITE;

    if(FAILED( s->m_pSourceVoice->SubmitSourceBuffer( &buffer ) ))
	{
		SAFE_DELETE(s);
		return NULL;
	}

    //m_nFrameToApply3DAudio = 0;

	m_Sounds.push_back(s);

    return s;
}

HRESULT SoundManager::Update( double fElapsedTime )
{
	if(strcmp(m_NewBackgroundSound, "NULL") != 0)
	{
		//crossfade
		if(m_BackgroundSound && m_BackgroundSound->GetVolume() > 0.0f)
		{
			m_BackgroundSound->SetVolume(m_BackgroundSound->GetVolume() - (float)fElapsedTime);
		}
		else
		{
			if(m_BackgroundSound)
				m_BackgroundSound->m_pSourceVoice->Stop();


			//free memory for current bg sound
			SOUND_LIST::iterator s = m_Sounds.begin();
			while(s != m_Sounds.end() && m_BackgroundSound)
			{
				if((*s) == m_BackgroundSound)
				{
					SAFE_DELETE(*s);
					s = m_Sounds.erase(s);
					break;
				}
				
				s++;
			}


			m_BackgroundSound = PlaySound( m_NewBackgroundSound, 2, D3DXVECTOR3(0,0,0),
				D3DXVECTOR3(0,1,0), D3DXVECTOR3(0,0,1), 255u );

			if(m_BackgroundSound) m_BackgroundSound->SetVolume(1.25f);

			strcpy_s(m_NewBackgroundSound, sizeof(m_NewBackgroundSound), "NULL");
		}
	}

	if(m_BackgroundSound)
	{
		m_BackgroundSound->SetPosition(mListenerPos);
	}

    if( !m_bInitialized )
        return S_FALSE;

    if( m_nFrameToApply3DAudio == 0 )
    {
		SOUND_LIST::iterator sound = m_Sounds.begin();
		while(sound != m_Sounds.end())
		{
			XAUDIO2_VOICE_STATE vs;
			(*sound)->m_pSourceVoice->GetState(&vs);

			//sound finished playing?
			if(vs.BuffersQueued == 0)
			{
				SAFE_DELETE(*sound);
				sound = m_Sounds.erase(sound);
			}
			else
			{
				//sound started?
				if((*sound)->mStarted == false)
				{
					if(FAILED((*sound)->m_pSourceVoice->Start( 0 ) ))
					{

					}

					(*sound)->mStarted = true;
				}
				
				if ( fElapsedTime > 0 )
				{
					D3DXVECTOR3 lVelocity = ( mListenerPos - m_listener.Position ) / (float)fElapsedTime;
					m_listener.Position = mListenerPos;
					m_listener.OrientFront  = mListenerLook;
					m_listener.OrientTop    = mListenerUp;
					m_listener.Velocity = lVelocity;

					D3DXVECTOR3 eVelocity = ( (*sound)->mPosition - (*sound)->m_emitter.Position ) / (float)fElapsedTime;
					(*sound)->m_emitter.Position = (*sound)->mPosition;
					(*sound)->m_emitter.Velocity = eVelocity;
				}

				const DWORD dwCalcFlags = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER 
										  | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_LPF_REVERB 
										  | X3DAUDIO_CALCULATE_REVERB;

				X3DAudioCalculate( m_x3DInstance, &m_listener, &(*sound)->m_emitter, dwCalcFlags,
								   &m_dspSettings );


				//IXAudio2SourceVoice* voice = (*sound)->m_pSourceVoice;
				//if ( voice )
				//{
				//	voice->SetFrequencyRatio( m_dspSettings.DopplerFactor );
				//	voice->SetOutputMatrix( m_pMasteringVoice, (*sound)->m_emitter.ChannelCount /*INPUTCHANNELS*/, m_nChannels, 
				//							m_matrixCoefficients );
				//}

				sound++;
			}
		}
    }

    m_nFrameToApply3DAudio++;
    m_nFrameToApply3DAudio %= 3;

    return S_OK;
}

void SoundManager::SetNewBackgroundSound(char* _fileName)
{
	strcpy_s(m_NewBackgroundSound, sizeof(m_NewBackgroundSound), _fileName);
}

void SoundManager::SetBackgroundSoundVolume(float _volume)
{
	if(m_BackgroundSound)
	{
		m_BackgroundSound->SetVolume(_volume);
	}
}

float SoundManager::GetBackgroundSoundVolume()
{
	if(m_BackgroundSound)
	{
		return m_BackgroundSound->GetVolume();
	}

	return -1;
}

void SoundManager::SetGlobalVolume(float _volume)
{
	if(m_pMasteringVoice)
		m_pMasteringVoice->SetVolume(_volume);
}

float SoundManager::GetGlobalVolume()
{
	float vol = -1;
	if(m_pMasteringVoice)
		m_pMasteringVoice->GetVolume(&vol);

	return vol;
}

HRESULT SoundManager::SetReverb( int nReverb )
{
    if( !m_bInitialized )
        return S_FALSE;

    if ( nReverb < 0 || nReverb >= NUM_PRESETS )
        return E_FAIL;

    if ( m_pSubmixVoice )
    {
        XAUDIO2FX_REVERB_PARAMETERS native;
        ReverbConvertI3DL2ToNative( &g_PRESET_PARAMS[ nReverb ], &native );
        m_pSubmixVoice->SetEffectParameters( 0, &native, sizeof(native) );
    }

    return S_OK;
}

void SoundManager::PauseAudio( bool resume )
{
	if( !m_bInitialized )
        return;

    if (resume)
        m_pXAudio2->StartEngine();
    else
        m_pXAudio2->StopEngine();
}

void SoundManager::DestroyAllSounds(bool _ignore_bg_sound)
{
	//return;
    if( !m_bInitialized )
        return;

	StopAllSounds();

	/*
	SOUND_LIST::iterator sound = m_Sounds.begin();
	while(sound != m_Sounds.end())
	{
		if(_ignore_bg_sound && (*sound) == m_BackgroundSound)
		{
			sound++;
			continue;
		}
		else if((*sound)->m_pSourceVoice)
		{
			//(*sound)->m_pSourceVoice->DestroyVoice();
			SAFE_DELETE(*sound);

			sound = m_Sounds.erase(sound);
		}
	}
	*/
	//m_Sounds.clear();

	SOUND_DATA_CACHE_LIST::iterator sound_data = m_SoundCache.begin();
	while(sound_data != m_SoundCache.end())
	{
		if(_ignore_bg_sound && m_BackgroundSound && (*sound_data) == m_BackgroundSound->m_pSoundData)
		{
			sound_data++;
			continue;
		}
		else
		{
			SAFE_DELETE(*sound_data);
			sound_data = m_SoundCache.erase(sound_data);
		}
	}
	//m_SoundCache.clear();
}

void SoundManager::CleanupAudio()
{
    if( !m_bInitialized )
        return;

	DestroyAllSounds(false);

    if (m_pSubmixVoice)
    {
        m_pSubmixVoice->DestroyVoice();
        m_pSubmixVoice=NULL;
    }

    if (m_pMasteringVoice)
    {
        m_pMasteringVoice->DestroyVoice();
        m_pMasteringVoice=NULL;
    }

    m_pXAudio2->StopEngine();
    SAFE_RELEASE( m_pXAudio2 );
    SAFE_RELEASE( m_pReverbEffect );

	/*
    SAFE_DELETE_ARRAY( m_pbSampleData );
	*/

    CoUninitialize();

    m_bInitialized = false;
}

SOUND_DATA* SoundManager::GetSoundData(char* wavename)
{
	SOUND_DATA_CACHE_LIST::iterator sd = m_SoundCache.begin();
	while(sd != m_SoundCache.end())
	{
		if(strcmp((*sd)->m_waveName, wavename) == 0)
			return *sd;

		sd++;
	}

	SOUND_DATA* sound_data = myNew SOUND_DATA();
	sound_data->m_Wave = myNew CWaveFile;
	strcpy_s(sound_data->m_waveName, sizeof(sound_data->m_waveName), wavename);
    //
    // Read in the wave file
    //
	if(FAILED(sound_data->m_Wave->Open( wavename, NULL, WAVEFILE_READ )))
	{
		MessageBox(0, "Kunde inte öppna fil", "Fel", 0);
		SAFE_DELETE(sound_data);
		return NULL;
	}

    // Get format of wave file
    sound_data->m_WaveFormat = sound_data->m_Wave->GetFormat();

    // Calculate how many bytes and samples are in the wave
    sound_data->m_cbWaveSize = sound_data->m_Wave->GetSize();

    // Read the sample data into memory
    //SAFE_DELETE_ARRAY( m_pbSampleData );

    sound_data->m_pbSampleData = myNew BYTE[ sound_data->m_cbWaveSize ];

    if(FAILED(sound_data->m_Wave->Read( sound_data->m_pbSampleData, sound_data->m_cbWaveSize, &sound_data->m_cbWaveSize ) ))
	{
		SAFE_DELETE(sound_data);
		return NULL;
	}

	m_SoundCache.push_back(sound_data);

	return sound_data;
}

D3DXVECTOR3 SoundManager::GetListenerPos()
{
	return mListenerPos;
}

D3DXVECTOR3	SoundManager::GetListenerUp()
{
	return mListenerUp;
}

D3DXVECTOR3 SoundManager::GetListenerLook()
{
	return mListenerLook;
}

void SoundManager::SetListenerPos(D3DXVECTOR3 pos)
{
	mListenerPos = pos;
}

void SoundManager::SetListenerUp(D3DXVECTOR3 up)
{
	mListenerUp = up;
}

void SoundManager::SetListenerLook(D3DXVECTOR3 look)
{
	mListenerLook = look;
}

void SoundManager::SetAllowPlaying(bool _allow)
{
	m_AllowPlaying = _allow;
}

bool SoundManager::GetAllowPlaying()
{
	return m_AllowPlaying;
}

HRESULT SoundManager::CacheFolder(char* folderPath)
{
	if(!folderPath) return E_FAIL;

	std::vector<std::string> fileList;

	char tileMeshFolder[MAX_PATH];
	strcpy_s(tileMeshFolder, sizeof(tileMeshFolder), folderPath);
	if(FAILED(GetFilenamesInDirectory(tileMeshFolder, ".wav", fileList)))
		return E_FAIL;

	char tmpStr[MAX_PATH];

	for(std::vector<std::string>::iterator file = fileList.begin(); file != fileList.end(); file++)
	{
		sprintf_s(tmpStr, sizeof(tmpStr), "%s/%s", tileMeshFolder, (char*)file->c_str());

		if(FAILED(CacheAudio(tmpStr)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT SoundManager::GetFilenamesInDirectory(char* folderPath, char* fileType, std::vector<std::string>& listToFill)
{
	WIN32_FIND_DATA fdata;
	HANDLE dhandle;

	// must append \* to the path
	{
		char buf[MAX_PATH];
		sprintf_s(buf, sizeof(buf), "%s\\*", folderPath);
		if((dhandle = FindFirstFile(buf, &fdata)) == INVALID_HANDLE_VALUE) {
			return E_FAIL;
		}
	}

	// even an "empty" directory will give two results - . and ..
	// we don't want them :-)
	//fileList.push_back(fdata.cFileName);

	while(true)
	{
		if(FindNextFile(dhandle, &fdata))
		{
			//we only want ".obj"-files
			if(strlen(fdata.cFileName) > 4)
				if(strcmp(&fdata.cFileName[strlen(fdata.cFileName)-4], fileType) == 0)
					listToFill.push_back(fdata.cFileName);
		}
		else
		{
			if(GetLastError() == ERROR_NO_MORE_FILES)
			{
				break;
			}
			else
			{
				FindClose(dhandle);
				return E_FAIL;
			}
		}
	}

	if(FindClose(dhandle) == 0)
	{
		return E_FAIL;
	}

	return S_OK;
}