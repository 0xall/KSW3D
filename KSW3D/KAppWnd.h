
#pragma once

#ifndef _KSW3D_APPWND_H_
#define _KSW3D_APPWND_H_

#include "stdafx.h"
#include "KSW3DDef.h"
#include "K3DApp.h"
using namespace std;

#if defined KSW3D_DEBUG

#  if !defined KSW3D_FRAMECHECK
#    define KSW3D_FRAMECHECK
#  endif

#  if !defined KSW3D_TIMECHECK
#    define KSW3D_TIMECHECK
#  endif

#  if !defined KSW3D_UPDATETIMECHECK
#    define KSW3D_UPDATETIMECHECK
#  endif

#endif

class KSW3DAppWnd
{
/* member variables */
private :

	DWORD					m_lastErrorCode;					// last error code

// app window error informations
#define KSW3D_ERR_NONE				0x00
#define KSW3D_ERR_WNDCREATE			0x01
#define KSW3D_ERR_DIRECT3DCREATE	0x02


protected :

	// member variables for windows
	HWND					m_hWnd;								// handle window
	DWORD					m_windowX, m_windowY;				// window position x,y
	DWORD					m_windowWidth, m_windowHeight;		// window width and height
	DWORD					m_backBufWidth, m_backBufHeight;	// backbuffer width and height
	bool					m_bFullscreen;						// whether the window is fullscreen mode, or windowed mode

	// member variables for direct3d
#if (DIRECT3D_VERSION == 0x900)		// direct version is 9

	LPDIRECT3D9				m_pD3D;								// direct3d interface
	LPDIRECT3DDEVICE9		m_pDevice;							// direct3d device
	D3DPRESENT_PARAMETERS	m_d3dpp;							// direct3d settings

#endif

	// member variables for rendering
	bool					m_bStopped;							// whether updating the window or not
	int 					m_slowDegree;						// if slowDegree increases, update frequency would decrease. (minimum is 100)
	
	map<tstring, KSW3DApp*>	m_apps;								// registered app
	KSW3DApp*				m_pCurrentApp;						// current executed app

	LPD3DXFONT				m_pDefFont;							// default font
	DWORD					m_renderInfoList;					// rendering information list

// rendering information properties
#define KSW3D_RI_FPS			0x00000001
#define KSW3D_RI_UPDATETIME		0x00000002

#if defined KSW3D_FRAMECHECK
	UINT					m_fps;								// current frame per second
	UINT					__fpsCount;							// temp variables for measuring fps
#endif

/* constructors, destructor */
public :

	KSW3DAppWnd(void);
	~KSW3DAppWnd(void);


/* callback methods */
protected :

	virtual HRESULT		OnInit() { return S_OK; };								// initializing callback function (called after default initializing)
	virtual void		OnUpdate() {};											// update callback function (called before default updating)
	virtual void		OnRender() {};											// render callback function (called after rendering is started)
	virtual HRESULT		OnRelease() { return S_OK; };							// release callback function (called before default releasing)

	// callback functions on Init()
	virtual HRESULT		_InitDirect3D(void);									// initializing about direct3d
	virtual HRESULT		_SetDirect3DParemeters();								// set direct3d parameters

	virtual BOOL		_MakeWndClass(HINSTANCE, WNDPROC, PTCHAR, PTCHAR);		// make window class

	virtual void		_CreateDefaultFont(void);								// create default font
	virtual void		_CreateDefaultApp(void);								// create default app

	// callback functions on Render()
	virtual void		_ClearWindow();											// clear screen before rendering
	virtual HRESULT		_PresentWindow();										// display the created frame on the screen
#if defined KSW3D_FRAMECHECK
	void				__OnRenderFPS(RECT& rt);								// rendering frame per second
#endif

	// callback functions on Update()
#if defined KSW3D_FRAMECHECK
	void				__OnUpdateFPS(void);									// updateing frame per second
#endif
	
	// callback functions for resetting device
	void				_OnLostDevice(void);									// free resources for reset device
	void				_OnResetDevice(void);									// validate resources
	virtual void		OnFreeVolatileResources(void);							// callback function on freeing user-defined resources
	virtual void		OnValidateVolatileResources(void);						// callback function on validating user-defined resources

	// callback function for setting error code
	void				_SetErr(DWORD errCode) { m_lastErrorCode = errCode; }	// set error code

/* methods */
public :

	virtual HRESULT		Init(HINSTANCE hInstance, WNDPROC wndProc, bool fullscreen = false, 
								DWORD windowx = 0, DWORD windowy = 0,
								DWORD width = KSW3D_DEF_WND_WIDTH, DWORD height = KSW3D_DEF_WND_HEIGHT,
								DWORD backbufWidth = KSW3D_DEF_BACKBUF_WIDTH, DWORD backbufHeight = KSW3D_DEF_BACKBUF_HEIGHT,
								PTCHAR wndClassName = TEXT("KSW3D"), PTCHAR wndName = TEXT("KSW3D PHYSICS"));					// initializing the window
	virtual HRESULT		Update(void);											// updating the window
	virtual HRESULT		Render(void);											// rendering the window
	virtual HRESULT		Release(void);											// releasing the window


	// methods for app
	HRESULT				RegisterApp(KSW3DApp* app, PTCHAR appName);				// register app
	HRESULT				SetApp(PTCHAR appName);									// set app by name
	HRESULT				SetApp(KSW3DApp* app);									// set app by pointer


	// methods for window
	void				ShowWindow(int nCmdShow);								// show or unshow window
	virtual UINT		WindowLoop(void);										// window loop

	void				SetScreenMode(bool bFullscreen);						// set screen mode to full screen mode or to windowed mode
	void				ToggleScreenMode(void);									// if windowed mode, set fullscreen mode, and if fullscreen mode, set windowed mode

	virtual void		WndProc(HWND, UINT, WPARAM, LPARAM);					// window procedure callback function


	// methods for rendering
	void				EnableRenderInformation(DWORD info, BOOL enable = TRUE);	// set rendering informations


	// getter function
	HWND				GetWindowHandle(void) const { return m_hWnd; }
	LPDIRECT3D9			GetDirect3DInterface(void) const { return m_pD3D; }
	LPDIRECT3DDEVICE9	GetDirect3DDevice(void) const { return m_pDevice; }
	DWORD				GetWindowX(void) const { return m_windowX; }
	DWORD				GetWindowY(void) const { return m_windowY; }
	DWORD				GetWindowWidth(void) const { return m_windowWidth; }
	DWORD				GetWindowHeight(void) const { return m_windowHeight; }
	DWORD				GetBackBufferWidth(void) const { return m_backBufWidth; }
	DWORD				GetBackBufferHeight(void) const { return m_backBufHeight; }
	void				GetBackBufferClientRect(RECT& rt) const { SetRect(&rt, 0, 0, m_backBufWidth, m_backBufHeight); }
	bool				IsFullscreenMode(void) const { return m_bFullscreen; } 
	DWORD				GetErr(void) const { return m_lastErrorCode; }
};

#define KSW3D_APPWND_BASIC_CALLBACK()	\
	virtual HRESULT OnInit(void);		\
	virtual void OnUpdate(void);		\
	virtual void OnRender(void);		\
	virtual HRESULT OnRelease(void);

#endif