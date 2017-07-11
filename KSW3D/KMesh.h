
#pragma once

#ifndef _KSW3D_MESH_H_
#define _KSW3D_MESH_H_

#include "stdafx.h"

class KSW3DAppWnd;

class KSW3DMesh
{
/* member variables */
protected :
	KSW3DAppWnd*		m_pAppWnd;			// app window pointer

	// member variables for direct3d
	LPDIRECT3DDEVICE9	m_pDevice;			// direct3d device
	LPD3DXMESH			m_pMesh;			// mesh

	D3DMATERIAL9*		m_pMeshMaterials;	// materials
	LPDIRECT3DTEXTURE9*	m_pMeshTextures;	// textures for mesh

	// member variables for mesh
	PTCHAR				m_strXFile;			// file(.x) name

	DWORD				m_dwNumMaterials;	// the number of materials

/* constructor, destructor */
public :
	KSW3DMesh(void);
	~KSW3DMesh(void);

/* callback methods */
protected :
	HRESULT				_LoadMeshFromX(PTCHAR strFileName);					// load .x file

/* methods */
public :
	HRESULT				Init(KSW3DAppWnd* pAppWnd, PTCHAR strFileName);		// initializing the mesh
	void				Render(void);
	HRESULT				Release(void);
};

#endif