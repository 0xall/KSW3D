
#include "stdafx.h"
#include "KMesh.h"
#include "KAppWnd.h"

KSW3DMesh::KSW3DMesh(void)
{
}

KSW3DMesh::~KSW3DMesh(void)
{
	Release();
}

HRESULT	KSW3DMesh::Init(KSW3DAppWnd* pAppWnd, PTCHAR strFileName)
{
	if(!pAppWnd) return E_FAIL;

	m_pDevice = m_pAppWnd->GetDirect3DDevice();
	if(FAILED(_LoadMeshFromX(strFileName))) return E_FAIL;
	return S_OK;
}

HRESULT KSW3DMesh::_LoadMeshFromX(PTCHAR strFileName)
{
	LPD3DXBUFFER pD3DXMtrlCtrl;
	D3DXMATERIAL* pMaterials;
	DWORD i;

	if(FAILED(D3DXLoadMeshFromX(strFileName, D3DXMESH_SYSTEMMEM, m_pDevice, NULL, &pD3DXMtrlCtrl, NULL, &m_dwNumMaterials, &m_pMesh)))
	{
		return E_FAIL;
	}

	pMaterials = (D3DXMATERIAL*) pD3DXMtrlCtrl->GetBufferPointer();
	m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
	m_pMeshTextures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

	for(i=0; i<m_dwNumMaterials; ++i)
	{
		m_pMeshMaterials[i] = pMaterials[i].MatD3D;
		m_pMeshMaterials[i].Diffuse = m_pMeshMaterials[i].Diffuse;

		m_pMeshTextures[i] = NULL;
		if(pMaterials[i].pTextureFilename != NULL && strlen(pMaterials[i].pTextureFilename) > 0)
		{
			D3DXCreateTextureFromFileA(m_pDevice, pMaterials[i].pTextureFilename, &m_pMeshTextures[i]);
		}
	}

	pD3DXMtrlCtrl->Release();
	return S_OK;
}

void KSW3DMesh::Render(void)
{
	DWORD i;

	for(i=0; i<m_dwNumMaterials; ++i)
	{
		m_pDevice->SetMaterial(&m_pMeshMaterials[i]);
		m_pDevice->SetTexture(0, m_pMeshTextures[i]);

		m_pMesh->DrawSubset(0);
	}
}

HRESULT	KSW3DMesh::Release(void)
{
	if(m_pMeshMaterials)
	{
		delete[] m_pMeshMaterials;
		m_pMeshMaterials = NULL;
	}

	if(m_pMeshTextures)
	{
		DWORD i;

		for(i=0; i<m_dwNumMaterials; ++i)
		{
			m_pMeshTextures[i]->Release();
		}

		delete[] m_pMeshTextures;
	}

	KSW3D_SAFE_RELEASE(m_pMesh);
	return S_OK;
}