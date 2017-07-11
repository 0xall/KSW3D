
#pragma once

#ifndef _KSW3D_APP_H_
#define _KSW3D_APP_H_

#include "stdafx.h"
#include "KCamera.h"
#include "KBaseTile.h"
using namespace std;

class KSW3DAppWnd;

class KSW3DApp 
{
/* member variables */
protected :
	KSW3DAppWnd*					m_pAppWnd;				// app window pointer

	// member variables for apps
	LPDIRECT3DDEVICE9				m_pDevice;				// direct3d device
	D3DVIEWPORT9					m_viewport;				// viewport

	POINT							m_ptCenter; 

	bool							m_bUsed;				// whether this app is used
	DWORD							m_properties;			// properties about the app

// app properties
#define KSW3D_APP_REMOVE	0x00000001		// if checked, the app window remove this app when updated

	/////////////////// test ///////////////////////
	LPD3DXMESH						m_pMesh;

	// member variables for camera
	map<tstring, KSW3DCamera*>		m_cams;					// registered camera list
	KSW3DCamera*					m_pCurrentCam;			// current camera

	KSW3DBaseTile*					m_pTile;				// tile of the app

/* constructors, destructor */
public :
	KSW3DApp(void);
	KSW3DApp(const KSW3DApp&);
	~KSW3DApp(void);

/* callback methods */
protected :
	virtual HRESULT		OnInit(void) { return S_OK; }								// initializing callback function (called after default initializing)
	virtual void		OnUpdate(void) {}											// updating callback function (called before default updating)

	virtual void		_CreateDefaultCamera(void);									// callback function initializing default camera
	virtual void		_SetDefaultViewport(void);									// callback function initializing default viewport

public :
	void				OnLostDevice(void);											// free resources for reset device
	void				OnResetDevice(void);										// validate resources

/* methods */
public :
	HRESULT				Init(KSW3DAppWnd*);											// intializing the app
	void				Update();														// updating the app
	void				Render();														// rendering the app
	HRESULT				Release();													// release the app

	// methods for camera
	HRESULT				RegisterCamera(PTCHAR cameraName, KSW3DCamera* cam);		// register camera
	HRESULT				SelectCamera(PTCHAR cameraName);							// set camera by name
	HRESULT				DeleteCamera(PTCHAR cameraName);							// delete camera (if used, cannot remove)
	HRESULT				SelectCamera(KSW3DCamera* cam);								// set camera by pointer
	void				DeleteAllCamera(void);										// delete all cameras

	// getter methods
	KSW3DAppWnd*		GetAppWindow(void) const { return m_pAppWnd; }				// get app window pointer
	LPDIRECT3DDEVICE9	GetDevice(void) const;										// get direct3d device
	bool				IsUsed(void) const { return m_bUsed; }						// whether this app is used or not
	D3DVIEWPORT9		GetViewport(void) const { return m_viewport; }				// get viewport
	KSW3DCamera*		GetCurrentCamera(void) const { return m_pCurrentCam; }		// get current camera by pointer
	KSW3DBaseTile*		GetTile(void) const { return m_pTile; }						// get tile
	KSW3DCamera*		GetCameraByName(PTCHAR cameraName);							// get camera by name

	virtual void		Use(bool use = true) { m_bUsed = use; };
};

#endif