
#pragma once

#ifndef _KSW3D_OBJECT_MANAGER_H_
#define _KSW3D_OBJECT_MANAGER_H_

#include "stdafx.h"
#include "KShader.h"
using namespace std;

class KSW3DApp;
class KSW3DBaseObject;
class KSW3DMeshManager;
class KSW3DShaderManager;

typedef struct __SHADER_RENDERING 
{
	void operator() (pair<KSW3DShader*, vector<KSW3DBaseObject*>>& elem)
	{
		elem.first->Render(elem.second);
	}
} __SHADER_RENDERING;

class KSW3DObjectManager
{
/* member variables */
protected :
	KSW3DApp*					m_pApp;								// app pointer
	KSW3DMeshManager*			m_pMeshManager;						// mesh manager
	KSW3DShaderManager*			m_pShaderManager;					// shader manager

	// member variables for objects
	map<KSW3DShader*, vector<KSW3DBaseObject*>>		m_objects;		// objects (collected by shaders)

/* constructor, destructor */
public :
	KSW3DObjectManager(void);
	~KSW3DObjectManager(void);

/* callback methods */
protected :

/* methods */
public :
	HRESULT Init(KSW3DApp* pApp);
	void Render(void);
};

#endif