
#pragma once

#ifndef _KSW3D_SHADER_H_
#define _KSW3D_SHADER_H_

#include "stdafx.h"
#include "KBaseObject.h"
using namespace std;

class KSW3DAppWnd;

class KSW3DShader
{
/* member variables */
protected :
	KSW3DAppWnd*		m_pAppWnd;						// app window pointer

	// member variables for direct3d
	LPDIRECT3DDEVICE9	m_pDevice;						// direct3d device
	LPD3DXEFFECT		m_pEffect;						// shader

	// member variables for shader file
	PTCHAR				m_strFileName;					// HLSL(.fx) file name
	PTCHAR				m_strCompileErrorInfo;			// compiling error information if compiled error

/* constructor, destructor */
public :
	KSW3DShader(void);
	~KSW3DShader(void);

/* callback methods */
protected :
	HRESULT			_LoadShader(PTCHAR fxFile);						// load shader by filename

/* methods */
public :
	HRESULT			Init(KSW3DAppWnd* pAppWnd, PTCHAR fxFile);		// initializing the shader
	void			Render(vector<KSW3DBaseObject*>&);				// rendering by the shader
	HRESULT			Release(void);									// releasing the shader

	void			OnLostDevice(void);								// free resources when lost device
	void			OnResetDevice(void);							// reset resources when reset device

	// getter methods
	KSW3DAppWnd*	GetAppWindow(void) const { return m_pAppWnd; }
	const PTCHAR	GetFileName(void) const { return m_strFileName; }
};

#endif