#pragma once

#include <windows.h>
#include <sstream>

class Window
{
private:
	HWND window_;

	void CalculateFrameStats(float total_time);

public:
	HWND InitWindowApp(HINSTANCE instance, int show_cmd, LRESULT(CALLBACK *WndProc)(HWND, UINT, WPARAM, LPARAM));

	void Update(float total_time);
	HWND get_window() const;
};