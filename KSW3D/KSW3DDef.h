
#pragma once

#ifndef _KSW3D_DEF_H_
#define _KSW3D_DEF_H_

#include "stdafx.h"

#define KSW3D_VERSION			0x100

// window default settings
#define KSW3D_DEF_WND_WIDTH			800
#define KSW3D_DEF_WND_HEIGHT		600

#define KSW3D_DEF_BACKBUF_WIDTH		800
#define KSW3D_DEF_BACKBUF_HEIGHT	600

// camera default setttings
#define KSW3D_CAM_DEF_POS_X			0.0f
#define KSW3D_CAM_DEF_POS_Y			0.0f
#define KSW3D_CAM_DEF_POS_Z			0.0f

#define KSW3D_CAM_DEF_AT_X			0.0f
#define KSW3D_CAM_DEF_AT_Y			0.0f
#define KSW3D_CAM_DEF_AT_Z			0.0f

#define KSW3D_CAM_DEF_UP_X			0.0f
#define KSW3D_CAM_DEF_UP_Y			1.0f
#define KSW3D_CAM_DEF_UP_Z			0.0f

#define KSW3D_CAM_DEF_FOVY			(D3DX_PI / 4)
#define KSW3D_CAM_DEF_ASPECT		1.0f
#define KSW3D_CAM_DEF_ZN			1.0f
#define KSW3D_CAM_DEF_ZF			5000.0f

// object default settings
#define KSW3D_OBJ_DEF_POS_X			0.0f
#define KSW3D_OBJ_DEF_POS_Y			0.0f
#define KSW3D_OBJ_DEF_POS_Z			0.0f

#define KSW3D_OBJ_DEF_SPEED_X		0.0f
#define KSW3D_OBJ_DEF_SPEED_Y		0.0f
#define KSW3D_OBJ_DEF_SPEED_Z		0.0f

#define KSW3D_OBJ_DEF_ACCEL_X		0.0f
#define KSW3D_OBJ_DEF_ACCEL_Y		0.0f
#define KSW3D_OBJ_DEF_ACCEL_Z		0.0f

// class basic functions
#define KSW3D_BASIC_CALLBACK_FUNC()		\
	virtual HRESULT OnInit(void);		\
	virtual HRESULT OnUpdate(void);		\
	virtual HRESULT OnRender(void);		\
	virtual HRESULT OnRelease(void);

#define KSW3D_BASIC_FUNC(INIT_PARAMETER)	\
	virtual HRESULT Init(INIT_PARAMETER);	\
	virtual HRESULT Update(void);			\
	virtual HRESULT Render(void);			\
	virtual HRESULT Release(void);

// safe relase
#define KSW3D_SAFE_RELEASE(p) if(p){ (p)->Release(); (p) = NULL; }

#endif	/* _KSW3D_DEF_H_ */