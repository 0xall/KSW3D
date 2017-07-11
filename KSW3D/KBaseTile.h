
#pragma once

#ifndef _KSW3D_TILE_H_
#define _KSW3D_TILE_H_

#include "stdafx.h"

class KSW3DApp;

class KSW3DBaseTile
{
/* member variables */
protected :
	KSW3DApp*					m_pApp;							// app pointer

	// member variables for direct3d
	LPDIRECT3DDEVICE9			m_pDevice;						// direct3d device
	LPDIRECT3DVERTEXBUFFER9		m_pVB;							// buffer for vertice
	LPDIRECT3DINDEXBUFFER9		m_pIB;							// buffer for indice
	LPDIRECT3DTEXTURE9			m_pTexture;						// texture

	// member variables for tile information
	size_t						m_numOfWidth, m_numOfHeight;	// the number of tiles at width and height
	float						m_lengthOfTile;					// length of a tile

	size_t						m_vertexBufferSize;				// vertex buffer size
	size_t						m_indexBufferSize;				// index buffer size
	size_t						m_vertexSize;					// vertex information size
	DWORD						m_vertexFVF;					// vertex format
	D3DFORMAT					m_indexFVF;						// index format

	VOID*						m_pTileVertices;				// tile vertices
	VOID*						m_pTileIndices;					// tile indices

/* constructors, destructor */
public :
	KSW3DBaseTile(void);
	~KSW3DBaseTile(void);

/* callback methods */
protected :
	virtual HRESULT		OnInit(void);							// initializing callback function (called after default initialzing)
	virtual void		OnUpdate(void);							// update callback function (called before default updating)
	virtual void		OnRender(void);							// render callback function (called after rendering is started)
	virtual HRESULT		OnRelease(void);						// release callback function (called before default releasing)

	// callback methods for textures
	virtual HRESULT		_CreateTexture(void);					// create textures

	// callback methods for vertex buffer
	virtual void		_SetVertexBufferInfo(void) = 0;			// set vertex buffer size and vertex format
	virtual HRESULT		_ConstructTileVertices() = 0;			// construct tile vertices
	virtual HRESULT		_CreateVertexBuffer(void);				// create vertex buffer and copy tile vertices to vertex buffer

	// callback methods for index buffer
	virtual void		_SetIndexBufferInfo(void) = 0;			// set index buffer size and index format
	virtual HRESULT		_ConstructTileIndices() = 0;			// construct tile indices
	virtual HRESULT		_CreateIndexBuffer(void);				// create index buffer and copy tile indices to index buffer

	// callback methods for lost device
	virtual HRESULT		_ValidateVertexBuffer(void);			// validate vertex buffer
	virtual HRESULT		_ValidateIndexBuffer(void);				// validate index buffer

	// callback methods for rendering
	virtual void		_RenderTile(void) = 0;					// rendering tile

public :
	void				FreeResources(void);					// free resources
	void				ValidateResources(void);				// validate resources

/* methods */
public :
	HRESULT				Init(KSW3DApp* pApp, size_t numOfWidth, size_t numOfHeight, float lengthOfTile);
	void				Update(void);
	virtual void		Render(void);
	HRESULT				Release(void);
};

#endif