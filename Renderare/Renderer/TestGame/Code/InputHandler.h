#pragma once

#include <Windows.h>

class InputHandler
{
private:
	InputHandler();
	InputHandler(InputHandler const&);
	void operator=(InputHandler const&);

public:
	static InputHandler& GetInstance()
	{
		static InputHandler instance;
		return instance;
	}

	static LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param);
};

