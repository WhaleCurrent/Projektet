#pragma once

#include "Game.h"

#include "Window.h"
#include "GameTimer.h"
#include "InputHandler.h"
#include <D3DRenderer.h>

#include "LeakTracker.h"
#include "LibraryLinker.h"
#include "SafeDelete.h"

Game::Game()
{
	window_ = new Window();
	game_timer_ = new GameTimer();
	renderer_ = new D3DRenderer();
}

void Game::CleanUp()
{
	SafeDelete(game_timer_);
	SafeDelete(window_);
	SafeDelete(renderer_);
}

Game::~Game()
{
	CleanUp();
}

void Game::Update()
{
	window_->Update(game_timer_->GetTotalTime());
}

void Game::Render()
{
	renderer_->Render();
}

int Game::Run()
{
	MSG message = { 0 };

	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			game_timer_->Tick();
			
			Update();
			Render();
		}
	}

	CleanUp();

	return (int)message.wParam;
}

Window* Game::get_window() const
{
	return window_;
}

D3DRenderer* Game::get_renderer() const
{
	return renderer_;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE h_prev_instance, PSTR cmd_line, int show_cmd)
{
	myInitMemoryCheck();

	Game game;

	game.get_window()->InitWindowApp(h_instance, show_cmd, WndProc);
	if (!game.get_renderer()->Initialize(game.get_window()->get_window(), 800, 600, 4))
	{
		MessageBox(0, L"Failed to initialize Direct3D", 0, 0);
		return 0;
	}

	//RAWINPUTDEVICE Rid[2];

	//Rid[0].usUsagePage = 0x01;
	//Rid[0].usUsage = 0x02;
	//Rid[0].dwFlags = RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
	//Rid[0].hwndTarget = 0;

	//Rid[1].usUsagePage = 0x01;
	//Rid[1].usUsage = 0x06;
	//Rid[1].dwFlags = RIDEV_NOLEGACY;   // adds HID keyboard and also ignores legacy keyboard messages
	//Rid[1].hwndTarget = 0;

	//if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE) {
	//	DWORD error = GetLastError();
	//	MessageBox(nullptr, L"Failed to register raw input device.", 0, error);
	//}

	return game.Run();
}