
#include "stdafx.h"
#include "KBaseTile.h"
#include "K3DApp.h"

KSW3DBaseTile::KSW3DBaseTile(void) : m_pTexture(NULL)
{
}

KSW3DBaseTile::~KSW3DBaseTile(void)
{
	if(m_pTileVertices != NULL) delete[] m_pTileVertices;
	if(m_pTileIndices != NULL) delete[] m_pTileIndices;
	Release();
}

HRESULT KSW3DBaseTile::Init(KSW3DApp* pApp, size_t numOfWidth, size_t numOfHeight, float lengthOfTile)
{
	if(!pApp) return E_FAIL;	// if app is not valid, return false

	// set member variables
	m_pApp = pApp;
	m_pDevice = m_pApp->GetDevice();

	m_numOfWidth = numOfHeight;
	m_numOfHeight = numOfHeight;
	m_lengthOfTile = lengthOfTile;

	if(FAILED(_CreateTexture()))	// if cannot create texture
	{
		return E_FAIL;
	}

	if(FAILED(_CreateVertexBuffer()))	// if cannot create vertex buffer
	{
		return E_FAIL;
	}
	
	
	if(FAILED(_CreateIndexBuffer()))	// if cannot create index buffer
	{
		return E_FAIL;
	}
	
	return OnInit();
}

HRESULT KSW3DBaseTile::_CreateTexture(void)
{
	return S_OK;
}

HRESULT KSW3DBaseTile::_CreateVertexBuffer(void)
{
	_SetVertexBufferInfo();

	if(FAILED(_ConstructTileVertices()))
	{
		return E_FAIL;
	}

	if(FAILED(m_pDevice->CreateVertexBuffer(m_vertexBufferSize, 0, m_vertexFVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
	{
		return E_FAIL;
	}

	VOID* pVertices;
	m_pVB->Lock(0, m_vertexBufferSize, &pVertices, 0);
	memcpy(pVertices, m_pTileVertices, m_vertexBufferSize);
	m_pVB->Unlock();
	return S_OK;
}

HRESULT KSW3DBaseTile::_CreateIndexBuffer()
{	
	_SetIndexBufferInfo();

	if(FAILED(_ConstructTileIndices()))
	{
		return E_FAIL;
	}

	if(FAILED(m_pDevice->CreateIndexBuffer(m_indexBufferSize, 
		0, m_indexFVF, D3DPOOL_DEFAULT, &m_pIB, NULL)))
	{
		return E_FAIL;
	}

	VOID* pIndices;
	m_pIB->Lock(0, sizeof(WORD) * m_numOfWidth * m_numOfHeight * 3 * 2, &pIndices, 0);
	memcpy(pIndices, m_pTileIndices, m_indexBufferSize);
	m_pIB->Unlock();
	return S_OK;
}

void KSW3DBaseTile::Update(void)
{
	OnUpdate();
}

void KSW3DBaseTile::Render(void)
{
	_RenderTile();
	OnRender();
}

HRESULT KSW3DBaseTile::Release(void)
{
	if(FAILED(OnRelease())) return E_FAIL;

	KSW3D_SAFE_RELEASE(m_pTexture);
	KSW3D_SAFE_RELEASE(m_pIB);
	KSW3D_SAFE_RELEASE(m_pVB);
	return S_OK;
}

HRESULT KSW3DBaseTile::_ValidateVertexBuffer(void)
{
	VOID* pVertices;
	m_pVB->Lock(0, m_vertexBufferSize, &pVertices, 0);
	memcpy(pVertices, m_pTileVertices, m_vertexBufferSize);
	m_pVB->Unlock();
	return S_OK;
}

HRESULT KSW3DBaseTile::_ValidateIndexBuffer(void)
{
	VOID* pIndices;
	m_pIB->Lock(0, m_indexBufferSize, &pIndices, 0);
	memcpy(pIndices, m_pTileIndices, m_indexBufferSize);
	m_pIB->Unlock();
	return S_OK;
}

void KSW3DBaseTile::FreeResources(void)
{
	Release();
}

void KSW3DBaseTile::ValidateResources(void)
{
	Init(m_pApp, m_numOfWidth, m_numOfHeight, m_lengthOfTile);
	_CreateTexture();
	_ValidateVertexBuffer();
	_ValidateIndexBuffer();
}

HRESULT KSW3DBaseTile::OnInit(void) { return S_OK; }
void KSW3DBaseTile::OnUpdate(void) { }
void KSW3DBaseTile::OnRender(void) { }
HRESULT KSW3DBaseTile::OnRelease(void) { return S_OK; }