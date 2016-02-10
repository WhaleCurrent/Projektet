#include "stdafx.h"
#include "LevelHandler.h"
#include "Cube.h"
#include "SoundManager.h"
#include "HUD.h"
#include "Menu.h"
#include "Text.h"

#include <sstream>

// The main window handle; this is used to identify a created window.
HWND g_Hwnd = 0;
float gDeltaTime = 0.0f;
int camRot = 0;
float camY = 1;
bool arrowIsAligned = true;

bool camUp = false;
bool camDown = false;



int currentLevel = -1;

void initMain();

// Wraps the code necessary to initialize a Windows
// application.  Function returns true if initialization
// was successful, otherwise it returns false.
bool InitWindowsApp(HINSTANCE instanceHandle, int show); 

// Wraps the message loop code.
int  Run();

// The window procedure handles events our window receives.
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); 
DXGI_SWAP_CHAIN_DESC sd;
ID3D10RenderTargetView* mRenderTargetView;
ID3D10Texture2D* backBuffer;
D3D10_TEXTURE2D_DESC depthStencilDesc;
D3D10_VIEWPORT vp;
D3DXCOLOR mClearColor(0.0f,0.0f,0.0f,1.0f);


ID3D10Device*	device;
IDXGISwapChain*	mSwapChain;
ID3D10Texture2D* mDepthStencilBuffer;
ID3D10DepthStencilView* mDepthStencilView;

void Update(float deltaTime);
void Render();

POINT tempMousePos;
POINT mousePos;
RECT windowCorner;
float mRotate = 0.0f;
float gameTime = 0.0f;
float levelCooldown = 0.0f;

int realClock, compareTime;
int levelTime = 0;

LevelHandler* levelHandler;
HUD* mHud;
Menu* mMenu;
Text* mText;

Cube cube(levelHandler);

float sound = 0.5f;
int timeTick, timeLeft, levelNo, levelLe;

bool HUDOptions = false;
bool HUDHighScore = false;
bool HUDLevelSelect = false;
bool HUDdeath = false;
bool HUDNoTime = false;
bool HUDWin = false;
bool menu = true;
bool win = false;
int isLoading = 0;
bool updateParticle = false;
bool particleTimer = false;
float timePart = 0.0f;

bool updateParticleMove = false;
bool particleTimerMove = false;
float timePartMove = 0.0f;

int miniSelector = 0;
float mMiniRotator = 0.0f;
float mMiniScale = 1.0f;
D3DXVECTOR3 gCubePosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 gCubeRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXMATRIX gCubeMatrix;

D3DXVECTOR3 gParticleColor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR2 gParticleSize = D3DXVECTOR2(0.0f, 0.0f);
D3DXVECTOR3 gParticleEmitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

D3DXVECTOR3 gParticleColorMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR2 gParticleSizeMove = D3DXVECTOR2(0.0f, 0.0f);
D3DXVECTOR3 gParticleEmitPosMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



void changeCameraMenu()
{
	GetCamera().mPosition = D3DXVECTOR3(3, 3, 8);
	GetCamera().mLook = D3DXVECTOR3(0,0,0);
	GetCamera().rebuildView(gDeltaTime);
}

void changeCameraEnter()
{
	GetCamera().mPosition = D3DXVECTOR3(0, 0, 8);
	GetCamera().mLook = D3DXVECTOR3(0,0,0);
	GetCamera().rebuildView(gDeltaTime);
}

// Windows equivalant to main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	SetThreadAffinityMask(GetCurrentThread(), 1);

	if(!InitWindowsApp(hInstance, nShowCmd)) 
		return 0;
	srand((unsigned)time(0)); 

	initMain();
	myInitMemoryCheck();
	Effects::InitAll(device);
	InputLayout::InitAll(device);
	GetLight().init();
	mText = new Text(device);
	
	GetLight().update();
	return Run(); 
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{
	// The first task to creating a window is to describe some of its 
	// characteristics by filling out a WNDCLASS structure.
	WNDCLASS wc; 

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = instanceHandle;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "BasicWndClass";


	// Next, we register this WNDCLASS instance with Windows so that we can 
	// create a window based on it.
	if(!RegisterClass(&wc)) 
	{
		MessageBox(0, "RegisterClass FAILED", 0, 0);
		return false;
	}

	// With our WNDCLASS instance registered, we can create a window with the
	// CreateWindow function.  This function returns a handle to the window it
	// creates (an HWND).  If the creation failed, the handle will have the value
	// of zero.  A window handle is a way to refer to the window, which is internally
	// managed by Windows.  Many of the Win32 API functions that operate on windows
	// require an HWND so that they know what window to act on.

	g_Hwnd = CreateWindow(
		"BasicWndClass",    // Registered WNDCLASS instance to use.
		"Cube vs World",       // window title
		WS_OVERLAPPEDWINDOW, // style flags
		CW_USEDEFAULT,       // x-coordinate
		CW_USEDEFAULT,       // y-coordinate
		SCREEN_WIDTH,       // width
		SCREEN_HEIGHT,       // height
		0,                   // parent window
		0,                   // menu handle
		instanceHandle,      // app instance
		0);                  // extra creation parameters

	if(g_Hwnd == 0)
	{
		MessageBox(0, "CreateWindow FAILED", 0, 0);
		return false;
	}

	// Even though we just created a window, it is not initially shown.  
	// Therefore, the final step is to show and update the window we just
	// created, which can be done with the following two function calls.
	// Observe that we pass the handle to the window we want to show and
	// update so that these functions know which window to show and update.
	ShowWindow(g_Hwnd, show);
	UpdateWindow(g_Hwnd);

	return true;
}
void Update(float deltaTime)
{

	if(isLoading == 1)
	{
		TextureManager::InitAll(device);
		levelHandler = new LevelHandler(device);
		levelHandler->loadScore();
	
		mHud = new HUD("../Bin/Models/hud.obj", device, TextureManager::mHud);
		mMenu = new Menu("../Bin/Models/menu.obj", device, TextureManager::mMenu, TextureManager::mMenu);

		D3DXMatrixIdentity(&gCubeMatrix);

		levelLe = -1;
		levelNo = -1;

		realClock = 0;
		compareTime = 0;

		cube.setLevelHandler(levelHandler);
		changeCameraMenu();

		for(int i = 0; i < levelHandler->getSize() - 1; i++)
		{
			mMenu->loadTextures(i);
		}

		isLoading = 2;
	}

	if(GetForegroundWindow() == g_Hwnd && (isLoading == 2 || isLoading == 3))
	{
		gDeltaTime = deltaTime;
		gameTime += deltaTime;

		SOUNDMANAGER->PauseAudio(true);
 		SOUNDMANAGER->Update(deltaTime);
		SOUNDMANAGER->SetGlobalVolume(sound);

		GetCursorPos(&tempMousePos);
		
		GetWindowRect(g_Hwnd, &windowCorner);
		ScreenToClient(g_Hwnd, &mousePos);
		if(!menu && !HUDWin)
		{

			if(camUp)
			{
				if(camY < 14)
					camY += gDeltaTime * 4.0f;
			}
			else if(camDown)
			{
				if(camY > -14)
					camY -= gDeltaTime * 4.0f;
			}

			GetLight().mPointLight.pos = D3DXVECTOR3(30, 40, 30);
			ShowCursor(false);
			//mousePos.x = windowCorner.left + (int)SCREEN_WIDTH / 2;
			//mousePos.y = windowCorner.top + (int)SCREEN_HEIGHT / 2;
			//SetCursorPos(mousePos.x, mousePos.y);

			D3DXVECTOR3 player = levelHandler->getPlayerPos();
			D3DXVECTOR3 temp;

			if(camRot == 0)
				temp = D3DXVECTOR3(player.x - 10, player.y + 30 * camY, player.z - 30);
			else if(camRot == 1)
				temp = D3DXVECTOR3(player.x + 30, player.y + 30 * camY, player.z - 10);
			else if(camRot == 2)
				temp = D3DXVECTOR3(player.x + 10, player.y + 30 * camY, player.z + 30);
			else if(camRot == 3)
				temp = D3DXVECTOR3(player.x - 30, player.y + 30 * camY, player.z + 10);

			GetCamera().mLook = player;
			GetCamera().mPosition = temp;
			GetCamera().rebuildView(gDeltaTime);

			levelHandler->update(deltaTime);
			if(!HUDdeath && !HUDWin)
				levelHandler->setPlayerMatrix(cube.move(deltaTime));

			gCubeRotation = D3DXVECTOR3(cube.rotationX, cube.rotationY, cube.rotationZ);
			gCubeMatrix = cube.cubeMatrix;
			gCubePosition = player;
			
			if(win)
			{
				GetAsyncKeyState('W');
				GetAsyncKeyState('A');
				GetAsyncKeyState('S');
				GetAsyncKeyState('D');
				win = false;
				timePart = 0.0f;
				updateParticle = false;
				timePartMove = 0.0f;
				updateParticleMove = false;
				cube.checkWin();
				if(currentLevel == 1)
					mMenu->mHighscores[0].setTexture(TextureManager::mLevel1);
				else if(currentLevel == 2)
					mMenu->mHighscores[1].setTexture(TextureManager::mLevel2);
				else if(currentLevel == 3)
					mMenu->mHighscores[2].setTexture(TextureManager::mLevel3);
				else if(currentLevel == 4)
					mMenu->mHighscores[3].setTexture(TextureManager::mLevel4);
			}
			if(levelHandler->puzzlePiecesLeft() && !win)
			{
				timePart = 0.0f;
				updateParticle = false;
				timePartMove = 0.0f;
				updateParticleMove = false;
				levelTime = 0;
				cube.sendTime(timeLeft);
				HUDWin = true;
				win = true;
				mMenu->setTexture();
			}
		
			if(levelHandler->enemyCollision(cube.getOBBPoints()))
			{
				updateParticle = false;
				timePart = 0.0f;
				timePartMove = 0.0f;
				updateParticleMove = false;
				GetAsyncKeyState('W');
				GetAsyncKeyState('A');
				GetAsyncKeyState('S');
				GetAsyncKeyState('D');
				HUDdeath = true;
			}
		}
		else if(menu && !HUDWin)
		{
			updateParticle = false;
			timePart = 0.0f;
			timePartMove = 0.0f;
			updateParticleMove = false;
			GetLight().mPointLight.pos = D3DXVECTOR3(0, 0, 8);
			mMenu->Update();
		}

		mHud->Update();
		SOUNDMANAGER->SetListenerPos(GetCamera().mPosition);
		SOUNDMANAGER->SetListenerLook(GetCamera().mLook);
		SOUNDMANAGER->SetListenerUp(GetCamera().mUp);
	}
	else
	{
		SOUNDMANAGER->PauseAudio(false);
	}

	if(HUDOptions && mMenu->mSide != 2)
		HUDOptions = false;
	if(HUDHighScore && mMenu->mSide != 3)
		HUDHighScore = false;
	if(HUDLevelSelect && mMenu->mSide != 4)
	{
		HUDLevelSelect = false;
		levelLe = -1;
		levelNo = -1;
	}
	GetLight().update();
}
void Render(float deltaTime)
{
	device->ClearRenderTargetView(mRenderTargetView, mClearColor);
	device->ClearDepthStencilView(mDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);

	device->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	if(isLoading == 0)
	{
		RECT loadRect;
		loadRect.bottom = SCREEN_HEIGHT;
		loadRect.top= SCREEN_HEIGHT / 2;
		loadRect.left = 0;
		loadRect.right = SCREEN_WIDTH;
		mText->DrawString("LOADING CUBE...", loadRect, D3DXCOLOR(1,1,1,1), 15);
		isLoading = 1;
	}
	else if(isLoading == 2 || isLoading == 3)
	{
		if(!menu)
		{
			levelHandler->drawLevel();

			if(!HUDWin)
			{
				if(!HUDdeath)
				{
					mHud->Draw();
				}
				else
				{
					camUp = false;
					camDown = false;
					mHud->Draw();
					if(HUDNoTime)
					{
						mText->DrawCenter("Time's up!", 10);
					}
					else
					{
						mText->DrawCenter("R.I.P", 5);
					}
				}
				if(GetForegroundWindow() == g_Hwnd)
				{
					realClock = clock()/CLOCKS_PER_SEC;
					if(compareTime <= realClock)
					{
						compareTime = realClock + 1;
						levelTime++;
					}
				}
				else
				{
					realClock = clock()/CLOCKS_PER_SEC;
					compareTime = realClock + 1;
				}

				if(!HUDdeath)
				{
					timeLeft = levelHandler->getTime() - levelTime;
				}

				if(timeLeft < 1)
				{
					timeLeft = 0;
					HUDNoTime = true;
					HUDdeath = true;
				}
				RECT rc;
				rc.left = 0;
				rc.right = 0; 
				rc.top = 1;
				rc.bottom = 1;
				mText->DrawString("", rc,  D3DXCOLOR(1,1,1,1), 0);
				mText->DrawLevelHUD(timeLeft, cube.points, levelHandler->getPuzzlesLeft());
			}
			else
			{
				mHud->Draw();
				mText->DrawCompleted();
				mText->DrawLevelHUD(timeLeft, cube.points, levelHandler->getPuzzlesLeft());
			}
		}
		else
		{
			realClock = clock()/CLOCKS_PER_SEC;
			compareTime = realClock + 1;
			mMenu->Draw();
			if(HUDOptions)
			{
				//mHud->Draw();

				//mText->DrawOptions();
			}
			else if(HUDHighScore)
			{
				//mHud->Draw();

				RECT rc;
				rc.left=300;
				rc.right=400; 
				rc.top=SCREEN_HEIGHT * 0.2f;
				rc.bottom=SCREEN_HEIGHT * 0.4f;

				mText->DrawString("", rc, D3DXCOLOR(1,1,1,1), 0);

				for(int i = 0; i < levelHandler->getSize() - 1; i++)
				{
					rc.top=SCREEN_HEIGHT * 0.15f + (120 * i);
					rc.bottom = SCREEN_HEIGHT * 0.35f + (120 * i);
					mText->DrawInt(levelHandler->getScore(i), rc, D3DXCOLOR(0,0,1,1));
				}
			}
			else if(HUDLevelSelect)
			{
				/*mHud->Draw();
				mText->DrawLevelSelect(levelLe, levelNo);*/
			}
		}
	}

	//if(!menu)
	//{
	//	levelHandler->drawParticle();
	//}
	mSwapChain->Present(0, 0);
	if(isLoading == 2)
	{
		isLoading = 3;
	}
}

int Run()
{
	MSG msg={0}; 
	
	LARGE_INTEGER freq, old, current;
	QueryPerformanceFrequency (&freq);
	QueryPerformanceCounter (&old);

	while( WM_QUIT != msg.message)
    {
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
			// update counter
			QueryPerformanceCounter(&current);

			// get delta then divide double delta (ticks) by frequency
			float dt = float (current.QuadPart - old.QuadPart) / float(freq.QuadPart);

			Update(dt);
			Render(dt);

			// copy current counter to old for next frame
			old.QuadPart = current.QuadPart;
        }
    }

	return (int)msg.wParam;
}

LRESULT CALLBACK 
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Handle some specific messages.  Note that if we handle a message,
	// we should return 0.
	if(msg == WM_KEYDOWN)
	{

		if(menu)
		{	
			if(!HUDHighScore && !HUDLevelSelect && !HUDOptions && !mMenu->mIsMoving)
			{
				if(levelCooldown < gameTime && wParam == VK_LEFT)
				{
					changeCameraMenu();
					levelCooldown = gameTime + 0.1f;
					mMenu->changeMenu(2);
					
				}
				else if(levelCooldown < gameTime && wParam == VK_RIGHT)
				{
					levelCooldown = gameTime + 0.1f;
					mMenu->changeMenu(1);
				}
				else if(levelCooldown < gameTime && wParam == VK_UP)
				{
					levelCooldown = gameTime + 0.1f;
					mMenu->changeMenu(3);
				}
				else if(levelCooldown < gameTime && wParam == VK_DOWN)
				{
					levelCooldown = gameTime + 0.1f;
					mMenu->changeMenu(4);
				}
			}
			if(levelCooldown < gameTime && wParam == VK_RETURN)
			{
				levelCooldown = gameTime + 0.1f;
				switch(mMenu->mSide)
				{
				case 1:
					{
						menu = false;
						GetAsyncKeyState('W');
						GetAsyncKeyState('A');
						GetAsyncKeyState('S');
						GetAsyncKeyState('D');
						levelHandler->startGame();
						//levelTime = 0;
						cube.setPosition(levelHandler->getPlayerPos());
					}
					break;
				case 2:
					{
						if(HUDOptions)
						{
							//changeCameraMenu();
							HUDOptions = false;
						}
						else
						{
							//changeCameraEnter();
							HUDOptions = true;
						}
					}
					break;
				case 3:
					{
						if(HUDHighScore)
						{
							changeCameraMenu();
							HUDHighScore = false;
						}
						else
						{
							changeCameraEnter();
							HUDHighScore = true;
						}
					}
					break;
				case 4:
					{
						if(HUDLevelSelect)
						{
							if(levelHandler->chooseLevel(miniSelector) != -1)
							{
								mMenu->mMiniLevel[miniSelector].mRotation.y = 0;
								mMenu->mMiniLevel[miniSelector].mScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

								HUDLevelSelect = false;
								cube.setPosition(levelHandler->getPlayerPos());
								cube.reset();
								menu = false;
								levelTime = 0;
							}
							else
							{
								SOUND* sfx = SOUNDMANAGER->PlaySound("../Bin/Sounds/puzzle2.wav");
							}
						}
						else
						{
							changeCameraEnter();
							miniSelector = levelHandler->getSize() - 1;
							HUDLevelSelect = true;
						}
					}
					break;
				case 5:
					{
						levelHandler->saveScore();
						DestroyWindow(g_Hwnd);
						PostQuitMessage(0);
					}
					break;
				}
				//else if(wParam == VK_ESCAPE)
				//{
				//	levelHandler->saveScore();
				//	DestroyWindow(g_Hwnd);
				//	PostQuitMessage(0);
				//}
			}
			else if(HUDHighScore)
			{
				if(wParam == VK_ESCAPE)
				{
					changeCameraMenu();
					HUDHighScore = false;
				}
			}
		}
		else if(!menu && !HUDdeath && !HUDWin)
		{
			if(wParam == VK_UP)
			{
				camUp = true;
			}
			else if(wParam == VK_DOWN)
			{
				camDown = true;
			}
			else if(levelCooldown < gameTime && wParam == VK_RIGHT)
			{
				if(camRot == 0)
				{
					camRot = 1;
				}
				else if(camRot == 1)
				{
					camRot = 2;
				}
				else if(camRot == 2)
				{
					camRot = 3;
				}
				else if(camRot == 3)
				{
					camRot = 0;
				}

				cube.rotateControls(true);
			}
			else if(wParam == VK_LEFT)
			{
				arrowIsAligned = false;
				if(camRot == 0)
				{
					camRot = 3;
				}
				else if(camRot == 3)
				{
					camRot = 2;
				}
				else if(camRot == 2)
				{
					camRot = 1;
				}
				else if(camRot == 1)
				{
					camRot = 0;
				}

				cube.rotateControls(false);
			}
			else if(wParam == 'R' && levelCooldown < gameTime && !cube.moving)
			{
				levelCooldown = gameTime + 0.1f;
				levelHandler->resetCurrentLevel();
				cube.setPosition(levelHandler->getPlayerPos());
				cube.reset();
				levelTime = 0;
			}
			else if(wParam == VK_ESCAPE)
			{
				SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/menu.wav");
				changeCameraMenu();
				mMenu->setSkyTexture(levelHandler->skybox->mShaderResourceDiff);
				menu = true;
			}
		}
		if(HUDdeath)
		{
			GetAsyncKeyState('W');
			GetAsyncKeyState('A');
			GetAsyncKeyState('S');
			GetAsyncKeyState('D');
			if(wParam == VK_RETURN)
			{	
				HUDdeath = false;
				HUDNoTime = false;
				levelCooldown = gameTime + 0.1f;
				levelHandler->resetCurrentLevel();
				cube.setPosition(levelHandler->getPlayerPos());
				cube.reset();
				levelTime = 0;
			}
			else if(wParam == VK_ESCAPE)
			{
				changeCameraMenu();
				mMenu->setSkyTexture(levelHandler->skybox->mShaderResourceDiff);
				menu = true;
			}
		}
		if(HUDWin)
		{
			GetAsyncKeyState('W');
			GetAsyncKeyState('A');
			GetAsyncKeyState('S');
			GetAsyncKeyState('D');
			if(wParam == VK_RETURN)
			{
				updateParticle = false;
				timePart = 0.0f;
				timePartMove = 0.0f;
				updateParticleMove = false;
				HUDWin = false;
			}
		}
		if(HUDLevelSelect)
		{
			if(levelCooldown < gameTime && wParam == VK_LEFT)
			{
				SOUNDMANAGER->PlaySound("../Bin/Sounds/levelselect.wav");
				if(miniSelector == 0)
					miniSelector = 20;
				else if(miniSelector == 1)
					miniSelector = 21;
				else if(miniSelector == 2)
					miniSelector = 22;
				else if(miniSelector == 3)
					miniSelector = 23;
				else if(miniSelector == 4)
					miniSelector = 24;
				else
					miniSelector -= 5;
			}
			else if(levelCooldown < gameTime && wParam == VK_RIGHT)
			{
				SOUNDMANAGER->PlaySound("../Bin/Sounds/levelselect.wav");
				if(miniSelector == 20)
					miniSelector = 0;
				else if(miniSelector == 21)
					miniSelector = 1;
				else if(miniSelector == 22)
					miniSelector = 2;
				else if(miniSelector == 23)
					miniSelector = 3;
				else if(miniSelector == 24)
					miniSelector = 4;
				else
					miniSelector += 5;
			}
			else if(levelCooldown < gameTime && wParam == VK_UP)
			{
				SOUNDMANAGER->PlaySound("../Bin/Sounds/levelselect.wav");
				if(miniSelector == 0)
					miniSelector = 24;
				else
					miniSelector -= 1;
			}
			else if(levelCooldown < gameTime && wParam == VK_DOWN)
			{
				SOUNDMANAGER->PlaySound("../Bin/Sounds/levelselect.wav");
				if(miniSelector == 24)
					miniSelector = 0;
				else
					miniSelector += 1;
			}
			if(wParam == VK_ESCAPE)
			{
				changeCameraMenu();
				HUDLevelSelect = false;
			}
		}
		if(HUDOptions)
		{
			if(wParam == VK_ESCAPE)
			{
				changeCameraMenu();
				HUDOptions = false;
			}
			else if(wParam == VK_LEFT)
			{
				sound -= 0.01f;
				if(sound < 0.0f)
				{
					sound = 0.0f;
				}
				else
				{
					mMenu->mVolumeObject->UpdateMinus();
				}			
			}
			else if(wParam == VK_RIGHT)
			{
				sound += 0.01f;
				if(sound > 1.0f)
				{
					sound = 1.0f;
				}
				else
				{
					mMenu->mVolumeObject->UpdatePlus();
				}				
			}
		}
		return 0;
	}
	else if(msg == WM_KEYUP)
	{
		if(wParam == VK_LEFT && menu)
		{
			mMenu->mRotatingY = 0;
		}
		else if(wParam == VK_RIGHT && menu)
		{
			mMenu->mRotatingY = 0;
		}
		else if(wParam == VK_UP && menu)
		{
			mMenu->mRotatingX = 0;
		}
		else if(wParam == VK_DOWN && menu)
		{
			mMenu->mRotatingX = 0;
		}

		if(wParam == VK_DOWN && !menu)
		{
			camDown = false;
		}
		else if(wParam == VK_UP && !menu)
		{
			camUp = false;
		}
		return 0;
	}
		// In the case of a destroy message, then send a 
		// quit message, which will terminate the message loop.
	else if(msg == WM_DESTROY)
	{
			PostQuitMessage(0); 
			return 0;
	}

	// Forward any other messages we did not handle above to the default window procedure.
	// Note that our window procedure must return the return value of DefWindowProc.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void initMain()
{
	srand((int)time(NULL));
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = g_Hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) ||defined(_DEBUG)
	createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

	D3D10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags,
		D3D10_SDK_VERSION, &sd, &mSwapChain, &device);

	mSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D),
		reinterpret_cast<void**>(&backBuffer));
	device->CreateRenderTargetView(backBuffer, 0 , &mRenderTargetView);
	backBuffer->Release();

	depthStencilDesc.Width = SCREEN_WIDTH;
	depthStencilDesc.Height = SCREEN_HEIGHT;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	device->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	device->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	device->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	device->RSSetViewports(1, &vp);

	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined DEBUG
	dwShaderFlags |= D3D10_SHADER_DEBUG;
	dwShaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	device->ClearRenderTargetView(mRenderTargetView, mClearColor);

	//No backface-culling
	D3D10_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D10_RASTERIZER_DESC));
	rsDesc.FillMode = D3D10_FILL_SOLID;
	rsDesc.CullMode = D3D10_CULL_BACK;
	rsDesc.FrontCounterClockwise = false;

	ID3D10RasterizerState* rzState;
	device->CreateRasterizerState(&rsDesc, &rzState);

	device->RSSetState(rzState);

	float ratio = (float)SCREEN_WIDTH/SCREEN_HEIGHT;
	GetCamera().setCameraLens(0.3f * PI, ratio, 0.5f, 500.0f);

	SOUNDMANAGER->Init();
	//SOUNDMANAGER->SetReverb(23);
	
	if(FAILED(SOUNDMANAGER->CacheFolder("../Bin/Sounds")))
	{
		//något fel
	}
	SOUNDMANAGER->SetNewBackgroundSound("../Bin/Sounds/menu.wav");
	
}