
#pragma once

#ifndef _KSW3D_MESH_MANAGER_H_
#define _KSW3D_MESH_MANAGER_H_

#include "stdafx.h"
#include "KMesh.h"
using namespace std;

class KSW3DAppWnd;

class KSW3DMeshManager
{
/* member variables */
protected :
	KSW3DAppWnd*				m_pAppWnd;

	// member variables for mesh
	map<tstring, KSW3DMesh*>	m_meshes;

/* constructor, destructor */
public :
	KSW3DMeshManager();
	~KSW3DMeshManager();

/* callback methods */
protected :

/* methods */
public :

};

#endif