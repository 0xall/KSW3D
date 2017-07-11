
#include "stdafx.h"
#include "KAppWnd.h"
#include "K3DApp.h"

KSW3DAppWnd::KSW3DAppWnd(void)
{
}

KSW3DAppWnd::~KSW3DAppWnd(void)
{
	Release();
}

HRESULT KSW3DAppWnd::Init(HINSTANCE hInstance, WNDPROC wndProc, bool fullscreen, 
						  DWORD windowx, DWORD windowy, DWORD width, DWORD height, DWORD backBufWidth, DWORD backBufHeight,
						  PTCHAR wndClassName, PTCHAR wndName)
{
	_SetErr(KSW3D_ERR_NONE);
	m_bFullscreen = fullscreen;
	m_windowX = windowx;
	m_windowY = windowy;
	m_windowWidth = width;
	m_windowHeight = height;
	m_backBufWidth = backBufWidth;
	m_backBufHeight = backBufHeight;
	m_slowDegree = 100;
	
	if(_MakeWndClass(hInstance, wndProc, wndClassName, wndName) == FALSE)
	{
		_SetErr(KSW3D_ERR_WNDCREATE);
		return E_FAIL;
	}

	if(FAILED(_InitDirect3D()))
	{
		_SetErr(KSW3D_ERR_DIRECT3DCREATE);
		return E_FAIL;
	}

	_CreateDefaultFont();
	_CreateDefaultApp();
	return OnInit();
}

HRESULT KSW3DAppWnd::Update(void)
{
	// app updating
	if(m_pCurrentApp) m_pCurrentApp->Update();
	// user-defined updating
	OnUpdate();

	// window updating
#if defined KSW3D_FRAMECHECK
	__OnUpdateFPS();
#endif

	return S_OK;
}

#if defined KSW3D_FRAMECHECK
void KSW3DAppWnd::__OnUpdateFPS()
{
	static DWORD elapsed;
	static DWORD prevTime = GetTickCount(), currentTime;
	currentTime = GetTickCount();
	elapsed += currentTime - prevTime;
	prevTime = currentTime;

	if(elapsed >= 1000)
	{
		m_fps = __fpsCount;
		__fpsCount = 0;
		elapsed = 0;
	}
}
#endif

HRESULT KSW3DAppWnd::Render(void)
{
	RECT rt;
	SetRect(&rt, 10, 10, 0, 0);

	_ClearWindow();

	if(SUCCEEDED(m_pDevice->BeginScene()))
	{
		// app rendering
		if(m_pCurrentApp) m_pCurrentApp->Render();
		// user-defined rendering
		OnRender();

#if defined KSW3D_FRAMECHECK
		__fpsCount++;
		if(m_renderInfoList & KSW3D_RI_FPS)
		{
			__OnRenderFPS(rt);
			rt.top += 15;
		}
#endif

		m_pDevice->EndScene();
	}

	if(FAILED(_PresentWindow()))
	{
		HRESULT hr = m_pDevice->TestCooperativeLevel();
		if(hr == D3DERR_DEVICELOST)
		{
 			Sleep(300);
		}

		else if(hr == D3DERR_DEVICENOTRESET)
		{
			_OnLostDevice();
			HRESULT hr = m_pDevice->Reset(&m_d3dpp);
			_OnResetDevice();
			m_bStopped = false;
		}
	}

	return S_OK;
}

#if defined KSW3D_FRAMECHECK
void KSW3DAppWnd::__OnRenderFPS(RECT& rt)
{
	static TCHAR strFPS[25];
	wsprintf(strFPS, TEXT("FPS : %d (frame/sec)"), m_fps);
	m_pDefFont->DrawText(0, strFPS, -1, &rt, DT_NOCLIP, D3DCOLOR_XRGB(240, 240, 0));
}
#endif

HRESULT KSW3DAppWnd::Release(void)
{
	// user-defined releasing
	if(FAILED(OnRelease())) return E_FAIL;

	// direct3d releasing
	KSW3D_SAFE_RELEASE(m_pDevice);
	KSW3D_SAFE_RELEASE(m_pD3D);

	return S_OK;
}

void KSW3DAppWnd::SetScreenMode(bool bFullscreen)
{
	if(bFullscreen != m_bFullscreen) m_bStopped = true;
	m_bFullscreen = bFullscreen;
	_SetDirect3DParemeters();

	if(!bFullscreen)	// if window mode
	{
		DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE) & (~(WS_POPUP | WS_EX_TOPMOST));
		dwStyle |= WS_OVERLAPPEDWINDOW;
		SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
		SetWindowPos(m_hWnd, HWND_NOTOPMOST, m_windowX, m_windowY, m_windowX + m_windowWidth, m_windowY + m_windowHeight, SWP_SHOWWINDOW);
	}

	else	// if full screen mode
	{
		DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE) & (~(WS_OVERLAPPEDWINDOW));
		dwStyle |= WS_POPUP | WS_EX_TOPMOST;
		SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	}

	m_pDevice->Reset(&m_d3dpp);
}

void KSW3DAppWnd::ToggleScreenMode(void)
{
	SetScreenMode(!m_bFullscreen);
	m_bStopped = true;
}

void KSW3DAppWnd::ShowWindow(int nCmdShow)
{
	::ShowWindow(m_hWnd, nCmdShow);
}

void KSW3DAppWnd::_ClearWindow(void)
{
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
}

HRESULT KSW3DAppWnd::_PresentWindow(void)
{
	return m_pDevice->Present(NULL, NULL, NULL, NULL);
}

HRESULT KSW3DAppWnd::_SetDirect3DParemeters()
{
	D3DDISPLAYMODE d3ddm;
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	ZeroMemory(&m_d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_d3dpp.BackBufferWidth = m_backBufWidth;
	m_d3dpp.BackBufferHeight = m_backBufHeight;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dpp.Windowed = ((m_bFullscreen) ? FALSE : TRUE);

	return S_OK;
}

HRESULT KSW3DAppWnd::_InitDirect3D(void)
{
	if((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		return E_FAIL;
	}

	if(FAILED(_SetDirect3DParemeters()))
	{
		return E_FAIL;
	}

	D3DCAPS9 caps;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	int vertexProcessing = ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)?
		D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING);

	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, vertexProcessing,
		&m_d3dpp, &m_pDevice)))
	{
		return E_FAIL;
	}

	return S_OK;
}

BOOL KSW3DAppWnd::_MakeWndClass(HINSTANCE hInstance, WNDPROC wndProc, PTCHAR className, PTCHAR wndName)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = wndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = className;

	if(!RegisterClassEx(&wc))
	{
		return FALSE;
	}

	m_hWnd = CreateWindowEx(NULL,
		className,
		wndName,
		((m_bFullscreen)? WS_EX_TOPMOST | WS_POPUP : WS_OVERLAPPEDWINDOW),
		m_windowX, m_windowY,
		m_windowWidth, m_windowHeight,
		NULL, NULL, hInstance, NULL);

	if(m_hWnd) return TRUE;
	return FALSE;
}

void KSW3DAppWnd::_CreateDefaultFont(void)
{
	D3DXCreateFont(m_pDevice, 25, 10, FW_SEMIBOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_pDefFont);
}

void KSW3DAppWnd::EnableRenderInformation(DWORD info, BOOL enable)
{
	if(enable)
	{
		m_renderInfoList |= enable;
	}

	else
	{
		m_renderInfoList &= ~(info);
	}
}

UINT KSW3DAppWnd::WindowLoop(void)
{
	MSG msg;
	int updateChecker = 0;

	while(true)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			if(!m_bStopped)		// if not stopped
			{
				updateChecker += 100;
				if(updateChecker >= m_slowDegree)
				{
					updateChecker %= m_slowDegree;
					Update();
				}
			}

			Render();
		}
	}

	Release();
	return msg.wParam;
}

void KSW3DAppWnd::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	}
}

void KSW3DAppWnd::_OnLostDevice(void) 
{
	m_pDefFont->OnLostDevice();
	if(m_pCurrentApp) m_pCurrentApp->OnLostDevice();
	OnFreeVolatileResources();
}
void KSW3DAppWnd::OnFreeVolatileResources(void) 
{
}

void KSW3DAppWnd::_OnResetDevice(void) 
{
	m_pDefFont->OnResetDevice();
	if(m_pCurrentApp) m_pCurrentApp->OnResetDevice();
	OnValidateVolatileResources();
}
void KSW3DAppWnd::OnValidateVolatileResources(void) {}

void KSW3DAppWnd::_CreateDefaultApp(void)
{
	KSW3DApp* app = new KSW3DApp();
	app->Init(this);
	RegisterApp(app, TEXT("default_app"));
	SetApp(app);
}

HRESULT KSW3DAppWnd::RegisterApp(KSW3DApp* app, PTCHAR appName)
{
	map<tstring, KSW3DApp*>::iterator it;
	it = m_apps.find(appName);
	if(it == m_apps.end())
	{
		m_apps.insert(map<tstring, KSW3DApp*>::value_type(appName, app));
		return S_OK;
	}

	else
	{
		return E_FAIL;
	}
}

HRESULT KSW3DAppWnd::SetApp(PTCHAR appName)
{
	map<tstring, KSW3DApp*>::iterator it;
	it = m_apps.find(appName);
	if(it == m_apps.end()) return E_FAIL;

	if(m_pCurrentApp) m_pCurrentApp->Use(false);
	m_pCurrentApp = it->second;
	m_pCurrentApp->Use();
	return S_OK;
}

HRESULT KSW3DAppWnd::SetApp(KSW3DApp* app)
{
	if(app == NULL) return E_FAIL;
	if(m_pCurrentApp) m_pCurrentApp->Use(false);
	m_pCurrentApp = app;
	m_pCurrentApp->Use();
	return S_OK;
}

