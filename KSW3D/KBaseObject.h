
#pragma once

#ifndef _KSW3D_UNIT_H_
#define _KSW3D_UNIT_H_

#include "stdafx.h"
#include "KSW3DDef.h"

class KSW3DApp;
class KSW3DObjectManager;
class KSW3DMeshManager;
class KSW3DShaderManager;

class KSW3DBaseObject
{
/* member variables */
protected :
	KSW3DObjectManager*			m_pObjectManager;				// object manager pointer

	// member variables for direct3d
	LPDIRECT3DDEVICE9			m_pDevice;						// direct3d device

	LPD3DXMESH					m_pMesh;						// mesh
	LPD3DXEFFECT				m_pShader;						// shader

	// member variables for the object
	D3DXVECTOR3					m_vPosition;					// the position of the object
	D3DXVECTOR3					m_vSpeed;						// the speed of the object
	D3DXVECTOR3					m_vAcceleration;				// the acceleration of the object

	D3DXMATRIX					m_mScale;						// matrix for scaling
	D3DXMATRIX					m_mWorld;						// matrix for translation
	D3DXMATRIX					m_mRotation;					// matrix for rotation

/* constructors, destructor */
public :
	KSW3DBaseObject(void);
	KSW3DBaseObject(const KSW3DBaseObject&);
	~KSW3DBaseObject(void);

/* callback methods */
protected :
	virtual HRESULT		OnInit(void);
	virtual void		OnUpdate(void);
	virtual void		OnRender(void);
	virtual HRESULT		OnRelease(void);

	virtual HRESULT		_LoadMesh(KSW3DMeshManager* meshManager);				// load mesh from mesh manager
	virtual HRESULT		_LoadShader(KSW3DShaderManager* shaderManager);			// load shader from shader manager

/* methods */
public :
	HRESULT				Init(KSW3DObjectManager* pObjectManager,	// initialize the member variables of the object and register the object to the shader manager
							D3DXVECTOR3 vPosition = D3DXVECTOR3(KSW3D_OBJ_DEF_POS_X, KSW3D_OBJ_DEF_POS_Y, KSW3D_OBJ_DEF_POS_Z),
							D3DXVECTOR3 vSpeed = D3DXVECTOR3(KSW3D_OBJ_DEF_SPEED_X, KSW3D_OBJ_DEF_SPEED_Y, KSW3D_OBJ_DEF_SPEED_Z),
							D3DXVECTOR3 vAccel = D3DXVECTOR3(KSW3D_OBJ_DEF_ACCEL_X, KSW3D_OBJ_DEF_ACCEL_Y, KSW3D_OBJ_DEF_ACCEL_Z));
	void				Update(void);
	void				Render(void);
	HRESULT				Release(void);
};

typedef struct __RENDERING_VECTOR_OBJECT
{
	void operator() (KSW3DBaseObject*& obj)
	{
		obj->Render();
	}
} __RENDERING_VECTOR_OBJECT;

#endif