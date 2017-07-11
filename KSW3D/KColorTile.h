
#pragma once

#ifndef _KSW3D_COLOR_TILE_H_
#define _KSW3D_COLOR_TILE_H_

#include "stdafx.h"
#include "KBaseTile.h"

typedef struct KSW3D_COLORTILE_VERTEX
{
	D3DXVECTOR3 m_position;
	DWORD diffuse;
} KSW3D_COLORTILE_VERTEX;

#define KSW3D_FVF_COLORTILE_VERTEX	(D3DFVF_XYZ | D3DFVF_DIFFUSE)

class KSW3DColorTile : public KSW3DBaseTile
{
/* constructor, destructor */
public :
	KSW3DColorTile(void);
	~KSW3DColorTile(void);

/* overriding */
protected :
	// callback methods for vertex buffer
	virtual void		_SetVertexBufferInfo(void);				// set vertex buffer size and vertex format
	virtual HRESULT		_ConstructTileVertices();				// construct tile vertices

	// callback methods for index buffer
	virtual void		_SetIndexBufferInfo(void);				// set index buffer size and index format
	virtual HRESULT		_ConstructTileIndices();				// construct tile indices

	// callback methods for rendering
	virtual void		_RenderTile(void);						// rendering tiles
};

#endif