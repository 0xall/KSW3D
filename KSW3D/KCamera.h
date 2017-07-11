
#pragma once

#ifndef _KSW3D_CAMERA_H_
#define _KSW3D_CAMERA_H_

#include "stdafx.h"
#include "KSW3DDef.h"

class KSW3DApp;

class KSW3DCamera
{
/* member variables */
public :
	KSW3DApp*			m_pApp;											// app pointer

	// member variables for direct3d
	LPDIRECT3DDEVICE9	m_pDevice;										// direct3d device

	// member variables for the position of eye and the direction of the camera
	D3DXVECTOR3			m_vDefaultEye, m_vDefaultAt;					// eye and at vector when initializing
	D3DXVECTOR3			m_vEye, m_vAt, m_vUp;							// current eye and at vector
	D3DXVECTOR3			m_vPos;											// position of camera

	// member variables for rotation
	D3DXQUATERNION		m_qCurrentRot;									// quaternion for current rotation
	D3DXQUATERNION		m_qPrevRot;										// temp quaternion for interpolation

	// member variables for camera
	float				m_fFOV, m_fAspect;								// FOV and apsect
	float				m_fNearPlane, m_fFarPlane;						// the nearest and the most furthest distance of rendering
	float				m_interpolation;
	bool				m_bUsed;										// whether using the camera in the app

	// member variables for matrix
	D3DXMATRIX			m_mView, m_mProj;								// matrix view and projection


/* constructors, destructor */
public :
	KSW3DCamera(void);
	KSW3DCamera(const KSW3DCamera&);
	~KSW3DCamera(void);


/* callback methods */
protected :
	virtual HRESULT		OnInit() { return S_OK; };						// initializing callback function (called after default initializing)

	virtual void		_SetMatrixView(void);							// set matrix view
	virtual void		_SetMatrixPerspective(void);					// set matrix perspective

/* methods */
public :
	virtual HRESULT		Init(KSW3DApp* app, float eyeX, float eyeY, float eyeZ,
								float atX = KSW3D_CAM_DEF_AT_X, float atY = KSW3D_CAM_DEF_AT_Y, float atZ = KSW3D_CAM_DEF_AT_Z,
								float posX = KSW3D_CAM_DEF_POS_X, float posY = KSW3D_CAM_DEF_POS_Y, float posZ = KSW3D_CAM_DEF_POS_Z,
								float fFOV = KSW3D_CAM_DEF_FOVY, float fAspect = KSW3D_CAM_DEF_ASPECT,
								float fNearDistance = KSW3D_CAM_DEF_ZN, float fFurthestDistance = KSW3D_CAM_DEF_ZF);			// initializing camera

	virtual HRESULT		Init(KSW3DApp* app, D3DXVECTOR3 eye,
								D3DXVECTOR3 at = D3DXVECTOR3(KSW3D_CAM_DEF_AT_X, KSW3D_CAM_DEF_AT_Y, KSW3D_CAM_DEF_AT_Z),
								D3DXVECTOR3 pos = D3DXVECTOR3(KSW3D_CAM_DEF_POS_X, KSW3D_CAM_DEF_POS_Y, KSW3D_CAM_DEF_POS_Z),
								float fFOV = KSW3D_CAM_DEF_FOVY, float fAspect = KSW3D_CAM_DEF_ASPECT,
								float fNearDistance = KSW3D_CAM_DEF_ZN, float fFurthestDistance = KSW3D_CAM_DEF_ZF);			// initializing camera


	// methods for updaing
	void		UpdateAll(void);								// update camera
	void		UpdateView(void);								// update view	
	void		UpdatePerspective(void);						// update perspective


	// methods for moving
	void		MoveFront(float distance);						// move front
	void		MoveBehind(float distance);						// move behind
	void		MoveUp(float distance);							// move upward
	void		MoveDown(float distance);						// move downward
	void		MoveLeft(float distance);						// move left
	void		MoveRight(float distance);						// move right

	void		ViewUp(float angle);
	void		ViewDown(float angle);
	void		ViewLeft(float angle);
	void		ViewRight(float angle);
	

	// methods for camera
	void		Use(bool use = TRUE);							// use or disuse this camera


	// getter methods
	bool		IsUsed(void) { return m_bUsed; }
};

#endif