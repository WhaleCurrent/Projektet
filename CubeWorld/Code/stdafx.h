#ifndef INCLUDE_H
#define INCLUDE_H

#include <d3dx10.h>
#include <d3d10.h>
#include <dxerr.h>

#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"dxerr.lib")
#pragma comment (lib,"dxgi.lib")
#pragma comment (lib,"Xinput.lib")

#pragma comment (lib,"winmm.lib")
#pragma comment (lib,"x3daudio.lib")

// Direct3D9 includes
#include <d3d9.h>
#include <d3dx9.h>

static float PI = 3.14159265358979323f;
extern float gDeltaTime;
extern float gameTime;
extern int camRot;
extern float camY;

extern bool HUDOptions;
extern bool HUDHighScore;
extern bool HUDLevelSelect;
extern bool HUDdeath;
extern bool HUDNoTime;
extern bool HUDwin;
extern bool menu;
extern bool win;
extern int levelTime;
extern float timePart;
extern bool updateParticle;
extern bool particleTimer;
extern bool updateVolumePlus;
extern bool updateVolumeMinus;

extern int isLoading;

extern int miniSelector;
extern float mMiniRotator;
extern float mMiniScale;
extern int currentLevel;
extern bool arrowIsAligned;

extern D3DXVECTOR3 gCubePosition;
extern D3DXVECTOR3 gCubeRotation;
extern D3DXMATRIX gCubeMatrix;

extern float timePartMove;
extern bool updateParticleMove;
extern bool particleTimerMove;

extern D3DXVECTOR3 gParticleColor;
extern D3DXVECTOR2 gParticleSize;
extern D3DXVECTOR3 gParticleEmitPos;

extern D3DXVECTOR3 gParticleColorMove;
extern D3DXVECTOR2 gParticleSizeMove;
extern D3DXVECTOR3 gParticleEmitPosMove;

const unsigned int SCREEN_WIDTH = 1024;
const unsigned int SCREEN_HEIGHT = 768;

const int POINTS_PER_SECOND = 37;

#include <windows.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>

D3DX10INLINE float RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

D3DX10INLINE float RandF(float a, float b)
{
	return a + RandF()*(b-a);
}

using namespace std;

struct VertexPosNorTex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;
};

struct VertexPos4NorTex
{
	D3DXVECTOR4 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texC;
};

struct VertexPosNorTanTex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR2 texC;
};

#include "InputLayouts.h"
#include "Effects.h"
#include "Light.h"
#include "TextureManager.h"
#include "Camera.h"

#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = (x); if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#else
#ifndef V
#define V(x)           { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
  #define myMalloc(s)       _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
  #define myCalloc(c, s)    _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
  #define myRealloc(p, s)   _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
  #define myExpand(p, s)    _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
  #define myFree(p)         _free_dbg(p, _NORMAL_BLOCK)
  #define myMemSize(p)      _msize_dbg(p, _NORMAL_BLOCK)
  #define myNew new(_NORMAL_BLOCK, __FILE__, __LINE__)
  #define myDelete delete  // Set to dump leaks at the program exit.
  #define myInitMemoryCheck() \
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#else
  #define myMalloc malloc
  #define myCalloc calloc
  #define myRealloc realloc
  #define myExpand _expand
  #define myFree free
  #define myMemSize _msize
  #define myNew new
  #define myDelete delete
  #define myInitMemoryCheck()
#endif 

#endif