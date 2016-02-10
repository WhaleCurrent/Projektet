//--------------------------------------------------------------------------------------
// File: XAudio2Sound3D.cpp
//
// XNA Developer Connection
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "audio.h"

//#define DEBUG_VS   // Uncomment this line to debug vertex shaders
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
//CDXUTDialogResourceManager      g_DialogResourceManager; // manager for shared resources of dialogs
//CD3DSettingsDlg                 g_SettingsDlg;          // Device settings dialog
//CDXUTDialog                     g_HUD;                  // dialog for standard controls

const LPWSTR g_SOUND_NAMES[] =
{
    L"Heli.wav",
    L"MusicMono.wav",
};

enum CONTROL_MODE
{
    CONTROL_SOURCE=0,
    CONTROL_LISTENER
}

g_eControlMode = CONTROL_SOURCE;

// Must match order of g_PRESET_PARAMS
const LPWSTR g_PRESET_NAMES[ NUM_PRESETS ] =
{
    L"Forest",
    L"Default",
    L"Generic",
    L"Padded cell",
    L"Room",
    L"Bathroom",
    L"Living room",
    L"Stone room",
    L"Auditorium",
    L"Concert hall",
    L"Cave",
    L"Arena",
    L"Hangar",
    L"Carpeted hallway",
    L"Hallway",
    L"Stone Corridor",
    L"Alley",
    L"City",
    L"Mountains",
    L"Quarry",
    L"Plain",
    L"Parking lot",
    L"Sewer pipe",
    L"Underwater",
    L"Small room",
    L"Medium room",
    L"Large room",
    L"Medium hall",
    L"Large hall",
    L"Plate",
};

#define FLAG_MOVE_UP        0x1
#define FLAG_MOVE_LEFT      0x2
#define FLAG_MOVE_RIGHT     0x4
#define FLAG_MOVE_DOWN      0x8

int                             g_moveFlags = 0;
const float                     MOTION_SCALE = 10.0f;

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );

void InitApp();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // DXUT will create and use the best device (either D3D9 or D3D10)
    // that is available on the system depending on which D3D callbacks are set below

    InitApp();

    HRESULT hr = InitAudio();
    if( FAILED( hr ) )
    {
        OutputDebugString( L"InitAudio() failed.  Disabling audio support\n" );
    }


    hr = PrepareAudio( g_SOUND_NAMES[0] );
    if( FAILED( hr ) )
    {
        OutputDebugString( L"PrepareAudio() failed\n" );
    }

    CleanupAudio();

	return 0;
}


////--------------------------------------------------------------------------------------
//// Initialize the app
////--------------------------------------------------------------------------------------
void InitApp()
{

}

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
    if( fElapsedTime > 0 )
    {		
		D3DXVECTOR3* vec = NULL;

		if(g_eControlMode == CONTROL_LISTENER)
		{
			vec = &g_audioState.vListenerPos;
		}
		else
		{
			vec = &g_audioState.vEmitterPos;
		}

        if( g_moveFlags & FLAG_MOVE_UP )
        {
            vec->z += fElapsedTime * MOTION_SCALE;
            vec->z = min( float( ZMAX ), vec->z );
        }

        if( g_moveFlags & FLAG_MOVE_LEFT )
        {
            vec->x -= fElapsedTime * MOTION_SCALE;
            vec->x = max( float( XMIN ), vec->x );
        }

        if( g_moveFlags & FLAG_MOVE_RIGHT )
        {
            vec->x += fElapsedTime * MOTION_SCALE;
            vec->x = min( float( XMAX ), vec->x );
        }

        if( g_moveFlags & FLAG_MOVE_DOWN )
        {
            vec->z -= fElapsedTime * MOTION_SCALE;
            vec->z = max( float( ZMIN ), vec->z );
        }
    }

	if(GetAsyncKeyState('O'))
	{
		PauseAudio(false);
	}
	if(GetAsyncKeyState('P'))
	{
		PauseAudio(true);
	}

    UpdateAudio( fElapsedTime );
}

//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
    switch( nChar )
    {
        case 'W':
        case 'w':
            if( bKeyDown )
                g_moveFlags |= FLAG_MOVE_UP;
            else
                g_moveFlags &= ~FLAG_MOVE_UP;
            break;

        case 'A':
        case 'a':
            if( bKeyDown )
                g_moveFlags |= FLAG_MOVE_LEFT;
            else
                g_moveFlags &= ~FLAG_MOVE_LEFT;
            break;

        case 'D':
        case 'd':
            if( bKeyDown )
                g_moveFlags |= FLAG_MOVE_RIGHT;
            else
                g_moveFlags &= ~FLAG_MOVE_RIGHT;
            break;

        case 'S':
        case 's':
            if( bKeyDown )
                g_moveFlags |= FLAG_MOVE_DOWN;
            else
                g_moveFlags &= ~FLAG_MOVE_DOWN;
            break;
    }

}


