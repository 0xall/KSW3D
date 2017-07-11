
#include "stdafx.h"
#include "KShaderManager.h"
#include "KAppWnd.h"

KSW3DShaderManager::KSW3DShaderManager(void)
{
}

KSW3DShaderManager::~KSW3DShaderManager(void)
{
}

HRESULT KSW3DShaderManager::Init(KSW3DAppWnd* pAppWnd)
{
	if(pAppWnd == NULL) return E_FAIL;
	m_pAppWnd = pAppWnd;

	if(FAILED(_RegisterDefaultShader())) return E_FAIL;

	return S_OK;
}

HRESULT KSW3DShaderManager::_RegisterDefaultShader(void)
{
	KSW3DShader* DefaultShader = new KSW3DShader();
	
	if(FAILED(DefaultShader->Init(m_pAppWnd, TEXT("DefaultShader.fx"))))
	{
		return E_FAIL;
	}

	RegisterShader(DefaultShader, TEXT("DefaultShader"));
	m_pDefaultShader = DefaultShader;
	return S_OK;
}

HRESULT KSW3DShaderManager::RegisterShader(KSW3DShader* shader, PTCHAR name)
{
	map<tstring, KSW3DShader*>::iterator it;
	it = m_shaders.find(name);
	if(it == m_shaders.end())
	{
		m_shaders.insert(map<tstring, KSW3DShader*>::value_type(name, shader));
		return S_OK;
	}

	else
	{
		return E_FAIL;
	}
}

HRESULT	KSW3DShaderManager::UnregisterShader(PTCHAR shaderName)
{
	KSW3DShader* shader = FindShader(shaderName);
	if(shader == NULL)	// if shader does not exist
	{
		return E_FAIL;
	}

	delete shader;		// free memory for the shader
	m_shaders.erase(shaderName);	// remove shader
	return S_OK;
}

KSW3DShader* KSW3DShaderManager::FindShader(PTCHAR shaderName)
{
	map<tstring, KSW3DShader*>::iterator it;
	it = m_shaders.find(shaderName);			// find the shader
	if(it == m_shaders.end()) return NULL;		// if cannot find the shader, return E_FAIL
	else return it->second;		// if find shader, return the camera pointer
}

void KSW3DShaderManager::UnregisterAllShader(void)
{
	map<tstring, KSW3DShader*>::iterator it;
	for(it = m_shaders.begin(); it != m_shaders.end(); ++it)
	{
		delete it->second;	// free memory for the shaders
	}

	m_shaders.clear();		// clear the map for cameras
}