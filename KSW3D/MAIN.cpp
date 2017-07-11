
// include pre-compiled header
#include "stdafx.h"

// include KSW3D header
#include "KAppWnd.h"

KSW3DAppWnd appWnd;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI 
#if defined UNICODE
	_tWinMain(HINSTANCE hInstance, HINSTANCE /* hPrevInstance */, PTCHAR /* lpCmdLine */, int nCmdShow)
#else
	WinMain(HINSTANCE hInstance, HINSTANCE /* hPrevInstance */, PTCHAR /* lpCmdLine */, int nCmdShow)
#endif
{
	if(FAILED(appWnd.Init(hInstance, WindowProc, true, 100, 100)))
	{
		switch(appWnd.GetErr())
		{
		case KSW3D_ERR_WNDCREATE :
			MessageBox(NULL, TEXT("Cannot create window."), TEXT("ERROR"), NULL);
			break;

		case KSW3D_ERR_DIRECT3DCREATE :
			MessageBox(NULL, TEXT("Cannot create direct3d device."), TEXT("ERROR"), NULL);
		}

		return FALSE;
	}

#if defined KSW3D_FRAMECHECK
	appWnd.EnableRenderInformation(KSW3D_RI_FPS);
#endif

	appWnd.ShowWindow(nCmdShow);
	return appWnd.WindowLoop();
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	appWnd.WndProc(hWnd, message, wParam, lParam);

	switch(message)
	{
	case WM_KEYDOWN :
		switch(wParam)
		{
			case VK_ESCAPE :
				PostQuitMessage(0);
				break;

			case VK_F1 :
				appWnd.ToggleScreenMode();
				break;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}