
#pragma once

#ifndef _KSW3D_TEXTURED_TILE_H_
#define _KSW3D_TEXTURED_TILE_H_

#include "stdafx.h"
#include "KBaseTile.h"
using namespace std;

typedef struct KSW3D_TEXTUREDTILE_VERTEX
{
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_normal;
	float tu, tv;
} KSW3D_TEXTUREDTILE_VERTEX;

#define KSW3D_FVF_TEXTUREDTILE_VERTEX	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

class KSW3DTexturedTile : public KSW3DBaseTile 
{
/* member variables */
protected :
	vector<LPDIRECT3DTEXTURE9>		m_textures;					// textures

/* constructor, destructor */
public :
	KSW3DTexturedTile(void);
	~KSW3DTexturedTile(void);

/* overriding */
protected :
	// callback methods for textures
	virtual HRESULT		_CreateTexture(void) = 0;				// create textures

	// callback methods for vertex buffer
	virtual void		_SetVertexBufferInfo(void);				// set vertex buffer size and vertex format
	virtual HRESULT		_ConstructTileVertices();				// construct tile vertices

	// callback methods for index buffer
	virtual void		_SetIndexBufferInfo(void);				// set index buffer size and index format
	virtual HRESULT		_ConstructTileIndices();				// construct tile indices

	// callback methods for rendering
	virtual void		_RenderTile(void);						// rendering tile
};

#endif