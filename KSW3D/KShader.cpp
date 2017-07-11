
#include "stdafx.h"
#include "KShader.h"
#include "KAppWnd.h"
#include "KBaseObject.h"

KSW3DShader::KSW3DShader(void) :
	m_pAppWnd(NULL),
	m_pDevice(NULL),
	m_pEffect(NULL),
	m_strFileName(NULL),
	m_strCompileErrorInfo(NULL)
{
}

KSW3DShader::~KSW3DShader(void)
{
	Release();
}

HRESULT KSW3DShader::Init(KSW3DAppWnd* pAppWnd, PTCHAR fxFile)
{
	if(!pAppWnd) return E_FAIL;
	m_pAppWnd = pAppWnd;

	m_pDevice = pAppWnd->GetDirect3DDevice();
	if(FAILED(_LoadShader(fxFile)))
	{
		Release();
		return E_FAIL;
	}

	return S_OK;
}

HRESULT KSW3DShader::_LoadShader(PTCHAR fxFile)
{
	LPD3DXBUFFER pError;
	DWORD dwShaderFlags = 0;

#if defined KSW3D_DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(m_pDevice, fxFile, NULL, NULL, dwShaderFlags, NULL, &m_pEffect, &pError);
	
	if(!m_pEffect && pError)	// if compiled error
	{
		size_t size = pError->GetBufferSize();
		VOID* ack = pError->GetBufferPointer();
		
		if(ack)
		{
			if(m_strCompileErrorInfo) delete[] m_strCompileErrorInfo;
			m_strCompileErrorInfo = new TCHAR[size];
			wsprintf(m_strCompileErrorInfo, (PTCHAR) ack, size);

#if defined KSW3D_DEBUG
			OutputDebugString(m_strCompileErrorInfo);	// output debug string
#endif
		}

		return E_FAIL;
	}

	size_t len;
#if defined UNICODE
	m_strFileName = new TCHAR[len = wcslen(fxFile)];
#else
	m_strFileName = new TCHAR[len = strlen(fxFile)];
#endif
	memcpy(m_strFileName, fxFile, len * sizeof(TCHAR));

	return S_OK;
}

void KSW3DShader::OnLostDevice(void)
{
	if(m_pEffect)
		m_pEffect->OnLostDevice();
}

void KSW3DShader::OnResetDevice(void)
{
	if(m_pEffect)
		m_pEffect->OnResetDevice();
}

HRESULT KSW3DShader::Release(void)
{
	if(m_strFileName)
	{
		delete[] m_strFileName;
		m_strFileName = NULL;
	}

	if(m_strCompileErrorInfo)
	{
		delete[] m_strCompileErrorInfo;
		m_strCompileErrorInfo = NULL;
	}

	KSW3D_SAFE_RELEASE(m_pEffect);
	return S_OK;
}

void KSW3DShader::Render(vector<KSW3DBaseObject*>& objects)
{
	UINT numPasses, i;

	m_pEffect->Begin(&numPasses, NULL);

	for(i=0; i<numPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		//for_each(objects.begin(), objects.end(), __RENDERING_VECTOR_OBJECT());
		m_pEffect->EndPass();
	}

	m_pEffect->End();
}