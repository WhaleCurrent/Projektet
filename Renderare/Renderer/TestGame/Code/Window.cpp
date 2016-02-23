#include "Window.h"

HWND Window::InitWindowApp(HINSTANCE instance_handle, int show_cmd, LRESULT(CALLBACK *WndProc)(HWND, UINT, WPARAM, LPARAM))
{
	WNDCLASS window_class;

	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WndProc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance_handle;
	window_class.hIcon = LoadIcon(0, IDI_APPLICATION);
	window_class.hCursor = LoadCursor(0, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	window_class.lpszMenuName = 0;
	window_class.lpszClassName = L"BasicWindowClass";

	if (!RegisterClass(&window_class))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}

	window_ = CreateWindow(
		L"BasicWindowClass",
		L"WindowTitle",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		instance_handle,
		0);

	if (window_ == 0)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	ShowWindow(window_, show_cmd);
	UpdateWindow(window_);

	return window_;
}

void Window::Update(float total_time)
{
	CalculateFrameStats(total_time);
}

HWND Window::get_window() const
{
	return window_;
}

void Window::CalculateFrameStats(float total_time)
{
	//static int frame_count = 0;
	//static float time_elapsed = 0.0f;

	//frame_count++;

	//if ((total_time - time_elapsed) >= 1.0f)
	//{
	//	float fps = (float)frame_count; //fps = frame_count / 1
	//	float mspf = 1000.0f / fps;

	//	std::wostringstream outs;
	//	outs.precision(4);
	//	outs <<
	//		L"FPS: " <<
	//		fps;
	//		//L"	 " <<
	//		//L"Frame Time: " <<
	//		//mspf <<
	//		//L"	ms";

	//	SetWindowText(window_, outs.str().c_str());

	//	frame_count = 0;
	//	time_elapsed += 1.0f;
	//}
}