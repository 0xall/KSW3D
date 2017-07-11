
#pragma once

#ifndef _KSW3D_SHADER_MANAGER_H_
#define _KSW3D_SHADER_MANAGER_H_

#include "stdafx.h"
#include "KShader.h"
using namespace std;

class KSW3DAppWnd;

class KSW3DShaderManager
{
/* member variables */
protected :
	KSW3DAppWnd*				m_pAppWnd;				// app window pointer

	// member variables for shaders
	KSW3DShader*				m_pDefaultShader;		// default shader
	map<tstring, KSW3DShader*>	m_shaders;				// shaders

/* constructor, destructor */
public :
	KSW3DShaderManager(void);
	~KSW3DShaderManager(void);

/* callback methods */
protected :
	HRESULT			_RegisterDefaultShader(void);

/* methods */
public :
	HRESULT			Init(KSW3DAppWnd*);										// initializing the shader manager

	HRESULT			RegisterShader(KSW3DShader* shader, PTCHAR name);		// register a shader
	HRESULT			UnregisterShader(PTCHAR shaderName);					// unregister a shader by its name
	void			UnregisterAllShader(void);								// unregister all shaders
	KSW3DShader*	FindShader(PTCHAR shaderName);							// search a shader
};

#endif