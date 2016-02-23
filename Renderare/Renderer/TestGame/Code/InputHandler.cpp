#include "InputHandler.h"


InputHandler::InputHandler()
{
	
}

LRESULT InputHandler::WndProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;
		// MOSUE BUTTONS DOWN
	//case WM_LBUTTONDOWN:
	//case WM_MBUTTONDOWN:
	//case WM_RBUTTONDOWN:
		//	return 0;

		// MOSUE BUTTONS UP
	//case WM_LBUTTONUP:
	//case WM_MBUTTONUP:
	//case WM_RBUTTONUP:
		//	return 0;

		// MOUSE MOVEMENT
	//case WM_MOUSEMOVE:
	//	return 0;

		// KEY DOWN
	case WM_KEYDOWN:
		if (w_param == VK_ESCAPE)
			PostQuitMessage(0);
		return 0;

		// KEY DOWN
	//case WM_KEYUP:
	//	return 0;

		// DESTORY WINDOW
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// WINDOW ACTIVE OR NOT
	//case WM_ACTIVATE:
	//	if (LOWORD(w_param) == WA_INACTIVE)
	//	{
	//		//Pause
	//	}
	//	else
	//	{
	//		//Start
	//	}
	//	return 0;

		//sent when the user grabs the resize bars.
	//case WM_ENTERSIZEMOVE:
		//return 0;

		//sent when the user releases the resize bars.
	//case WM_EXITSIZEMOVE:
		//return 0;

	//case WM_GETMINMAXINFO:
		//((MINMAXINFO*)l_param)->ptMinTrackSize.x = 200;
		//((MINMAXINFO*)l_param)->ptMinTrackSize.y = 200;
		//return 0;
	}
	return DefWindowProc(window, message, w_param, l_param);
}
