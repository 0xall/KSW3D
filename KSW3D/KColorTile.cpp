
#include "stdafx.h"
#include "KColorTile.h"

KSW3DColorTile::KSW3DColorTile(void)
{
}

KSW3DColorTile::~KSW3DColorTile(void)
{
}

void KSW3DColorTile::_SetVertexBufferInfo(void)
{
	m_vertexBufferSize = (m_numOfWidth + 1) * (m_numOfHeight + 1) * sizeof(KSW3D_COLORTILE_VERTEX);
	m_vertexFVF = KSW3D_FVF_COLORTILE_VERTEX;
	m_vertexSize = sizeof(KSW3D_COLORTILE_VERTEX);
}

HRESULT	KSW3DColorTile::_ConstructTileVertices()
{
	size_t i, j, k = 0;

	float width = m_numOfWidth * m_lengthOfTile;
	float height = m_numOfHeight * m_lengthOfTile;
	float startX = -width / 2; 
	float startZ = -height / 2;

	m_pTileVertices = new KSW3D_COLORTILE_VERTEX[m_vertexBufferSize / sizeof(KSW3D_COLORTILE_VERTEX)];
	for(i=0; i<=m_numOfHeight; ++i)
	{
		for(j=0; j<=m_numOfWidth; ++j)
		{
			KSW3D_COLORTILE_VERTEX& cVertex = ((KSW3D_COLORTILE_VERTEX*)m_pTileVertices)[k++];
			cVertex.m_position = D3DXVECTOR3(startX + j * m_lengthOfTile , (float)(rand() / 10) / 300, startZ  + i * m_lengthOfTile );
			cVertex.diffuse = D3DCOLOR_XRGB(120, 0, 180);
		}
	}

	return S_OK;
}

void KSW3DColorTile::_SetIndexBufferInfo(void)
{
	m_indexBufferSize = sizeof(WORD) * m_numOfHeight * m_numOfWidth * 3 * 2;
	m_indexFVF = D3DFMT_INDEX16;
}

HRESULT	KSW3DColorTile::_ConstructTileIndices()
{
	size_t i, j;
	WORD* pIndex;

	m_pTileIndices = new WORD[m_indexBufferSize / sizeof(WORD)];
	pIndex = (WORD*) m_pTileIndices;

	for(i=0; i<m_numOfHeight; ++i)
	{
		for(j=0; j<m_numOfWidth; ++j)
		{
			*pIndex++ = i * (m_numOfHeight + 1) + j;
			*pIndex++ = i * (m_numOfHeight + 1) + j + 1;
			*pIndex++ = (i+1) * (m_numOfHeight + 1) + j + 1;
			
			*pIndex++ = i * (m_numOfHeight + 1) + j;
			*pIndex++ = (i+1) * (m_numOfHeight + 1) + j + 1;
			*pIndex++ = (i+1) * (m_numOfHeight + 1) + j;
		}
	}

	return S_OK;
}

void KSW3DColorTile::_RenderTile(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_vertexSize);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->SetFVF(m_vertexFVF);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, (m_numOfHeight + 1) * (m_numOfWidth + 1), 0, (m_numOfHeight * m_numOfWidth) * 2);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}