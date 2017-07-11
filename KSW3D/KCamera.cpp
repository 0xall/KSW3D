
#include "stdafx.h"
#include "KCamera.h"
#include "K3DApp.h"

KSW3DCamera::KSW3DCamera(void)
{
}

KSW3DCamera::KSW3DCamera(const KSW3DCamera& cam)
{
}

KSW3DCamera::~KSW3DCamera(void)
{
}

HRESULT KSW3DCamera::Init(KSW3DApp* app, float eyeX, float eyeY, float eyeZ, float atX, float atY, float atZ, float posX, float posY, float posZ,
		float fFOV, float fAspect, float fNearDistance, float fFurthestDistance)
{
	return Init(app, D3DXVECTOR3(eyeX, eyeY, eyeZ), D3DXVECTOR3(atX, atY, atZ), D3DXVECTOR3(posX, posY, posZ), fFOV, fAspect, fNearDistance, fFurthestDistance);
}

HRESULT KSW3DCamera::Init(KSW3DApp* app, D3DXVECTOR3 vEye, D3DXVECTOR3 vAt, D3DXVECTOR3 pos, float fFOV, float fAspect, float fNearDistance, float fFurthestDistance)
{
	if(app == NULL)
	{
		return E_FAIL;
	}

	m_pDevice = app->GetDevice();

	m_pApp = app;
	m_vEye = vEye;
	m_vAt = vAt;
	m_fFOV = fFOV;		m_fAspect = fAspect;
	m_fNearPlane = fNearDistance;
	m_fFarPlane = fFurthestDistance;
	m_interpolation = 0.95f;

	m_vDefaultEye = m_vEye;
	m_vDefaultAt = m_vAt;

	m_vPos = pos;
	m_qPrevRot = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	m_qCurrentRot = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

	return OnInit();
}

void KSW3DCamera::UpdateAll(void)
{
	UpdateView();
	UpdatePerspective();
}

void KSW3DCamera::_SetMatrixView(void)
{
	D3DXQUATERNION qRt, qvRt;
	D3DXMATRIX mRt, mvRt, mTrans;

	//D3DXQuaternionRotationYawPitchRoll(&qvRt, m_fvYawAngle, m_fvPitchAngle, m_fvRollAngle);
	D3DXQuaternionSlerp(&qvRt, &m_qCurrentRot, &m_qPrevRot, m_interpolation);
	D3DXMatrixRotationQuaternion(&mvRt, &qvRt);

	m_qPrevRot = qvRt;

	D3DXMatrixTranslation(&mTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXVec3TransformCoord(&m_vUp, &vUp, &mvRt);

	D3DXVec3TransformCoord(&m_vEye, &m_vDefaultEye, &mvRt);
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &mTrans);

	D3DXVec3TransformCoord(&m_vAt, &m_vDefaultAt, &mvRt);
	D3DXVec3TransformCoord(&m_vAt, &m_vAt, &mTrans);

	D3DXMatrixLookAtLH(&m_mView, &m_vEye, &m_vAt, &m_vUp);
}

void KSW3DCamera::_SetMatrixPerspective(void)
{
	D3DXMatrixPerspectiveFovLH(&m_mProj, m_fFOV, m_fAspect, m_fNearPlane, m_fFarPlane);
}

void KSW3DCamera::Use(bool use)
{
	m_bUsed = use;
	if(use) UpdateAll();
}

void KSW3DCamera::UpdateView(void)
{
	_SetMatrixView();
	if(m_bUsed)
	{
		m_pDevice->SetTransform(D3DTS_VIEW, &m_mView);
	}
}

void KSW3DCamera::UpdatePerspective(void)
{
	_SetMatrixPerspective();
	if(m_bUsed)
	{
		m_pDevice->SetTransform(D3DTS_PROJECTION, &m_mProj);
	}
}

void KSW3DCamera::MoveFront(float distance)
{
	D3DXVECTOR3 move = m_vAt - m_vEye;
	D3DXVec3Normalize(&move, &move);
	m_vPos += distance * move;
}

void KSW3DCamera::MoveBehind(float distance)
{
	D3DXVECTOR3 move = m_vEye - m_vAt;
	D3DXVec3Normalize(&move, &move);
	m_vPos += distance * move;
}

void KSW3DCamera::MoveLeft(float distance)
{
	D3DXVECTOR3 move = m_vAt - m_vEye;
	D3DXVec3Cross(&move, &move, &m_vUp);
	D3DXVec3Normalize(&move, &move);
	m_vPos += distance * move;
}

void KSW3DCamera::MoveRight(float distance)
{
	D3DXVECTOR3 move = m_vEye - m_vAt;
	D3DXVec3Cross(&move, &move, &m_vUp);
	D3DXVec3Normalize(&move, &move);
	m_vPos += distance * move;
}

void KSW3DCamera::MoveUp(float distance)
{
	D3DXVECTOR3 move = m_vUp;
	D3DXVec3Normalize(&move, &move);
	m_vPos += distance * move;
}

void KSW3DCamera::MoveDown(float distance)
{
	D3DXVECTOR3 move = -m_vUp;
	D3DXVec3Normalize(&move, &move);
	m_vPos += distance * move;
}

void KSW3DCamera::ViewUp(float angle)
{
	/*m_fvPitchAngle += angle;*/
	D3DXQUATERNION upAngle;
	D3DXQuaternionRotationYawPitchRoll(&upAngle, 0.0f, angle, 0.0f);
	m_qCurrentRot = upAngle * m_qCurrentRot;
}

void KSW3DCamera::ViewDown(float angle)
{
	//m_fvPitchAngle -= angle;
	D3DXQUATERNION upAngle;
	D3DXQuaternionRotationYawPitchRoll(&upAngle, 0.0f, -angle, 0.0f);
	m_qCurrentRot = upAngle * m_qCurrentRot;
}

void KSW3DCamera::ViewLeft(float angle)
{
}

void KSW3DCamera::ViewRight(float angle)
{
	D3DXQUATERNION yawAngle;
	D3DXQuaternionRotationYawPitchRoll(&yawAngle, angle, 0.0f, 0.0f);
	m_qCurrentRot *= yawAngle;
}