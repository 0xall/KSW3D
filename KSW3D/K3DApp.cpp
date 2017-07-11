
#include "stdafx.h"
#include "K3DApp.h"
#include "KAppWnd.h"
#include "KColorTile.h"

KSW3DApp::KSW3DApp(void)
{
}

KSW3DApp::KSW3DApp(const KSW3DApp& app)
{
}

KSW3DApp::~KSW3DApp(void)
{
	Release();
}

// Init() is initializing the app before app is started
// it should be called before app is started
HRESULT KSW3DApp::Init(KSW3DAppWnd* appWnd)
{
	if(appWnd == NULL) return E_FAIL;

	m_pAppWnd = appWnd;
	m_pDevice = appWnd->GetDirect3DDevice();
	m_pCurrentCam = NULL;

	///////////// test //////////////
	D3DXCreateSphere(m_pDevice, 2.0f, 12, 16, &m_pMesh, NULL);

	_SetDefaultViewport();
	_CreateDefaultCamera();

	m_pTile = new KSW3DColorTile();
	m_pTile->Init(this, 200, 200, 15.0f);
	//m_pTile = NULL;

	m_ptCenter.x = GetAppWindow()->GetWindowWidth() / 2;
	m_ptCenter.y = GetAppWindow()->GetWindowHeight() / 2;
	ClientToScreen(GetAppWindow()->GetWindowHandle(), &m_ptCenter);
	ShowCursor(false);
	SetCursorPos(m_ptCenter.x, m_ptCenter.y);

	return OnInit();
}

// _CreateeDefaultCamera() is callback function that create camera when app created
void KSW3DApp::_CreateDefaultCamera(void)
{
	KSW3DCamera* defaultCamera = new KSW3DCamera();
	defaultCamera->Init(this, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	defaultCamera->m_interpolation = 0.95f;
	RegisterCamera(TEXT("default_camera"), defaultCamera);
	SelectCamera(defaultCamera);
}

// _SetDefaultViewport() is callback function that create viewport when app created
void KSW3DApp::_SetDefaultViewport(void)
{
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = m_pAppWnd->GetWindowWidth();
	m_viewport.Height = m_pAppWnd->GetWindowHeight();
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	m_pDevice->SetViewport(&m_viewport);
}

void KSW3DApp::Update(void)
{
	OnUpdate();

	/////////////// test /////////////
	KSW3DCamera& cam = *m_pCurrentCam;

	float moveSpeed = 3.0f;
	if(GetAsyncKeyState(VK_SHIFT))
		moveSpeed = 0.6f;

	if(GetAsyncKeyState('W'))
	{
		cam.MoveFront(moveSpeed);
	}

	if(GetAsyncKeyState('S'))
	{
		cam.MoveBehind(moveSpeed);
	}

	if(GetAsyncKeyState('A'))
	{
		cam.MoveLeft(moveSpeed);
	}

	if(GetAsyncKeyState('D'))
	{
		cam.MoveRight(moveSpeed);
	}

	if(GetAsyncKeyState(VK_LEFT))
	{
		D3DXQUATERNION qRt;
		D3DXQuaternionRotationYawPitchRoll(&qRt, 0.0f, 0.0f, -0.02f);
		cam.m_qCurrentRot = cam.m_qCurrentRot * qRt;
	}

	if(GetAsyncKeyState(VK_RIGHT))
	{
		cam.ViewRight(0.02f);
	}

	if(GetAsyncKeyState(VK_UP))
	{
		cam.ViewUp(0.02f);
	}

	if(GetAsyncKeyState(VK_DOWN))
	{
		cam.ViewDown(0.02f);
	}

	cam.UpdateView();
	POINT pt;
	GetCursorPos(&pt);
	cam.ViewUp((pt.y - m_ptCenter.y) / 300.0f);
	cam.ViewRight((pt.x - m_ptCenter.x) / 300.0f);
	SetCursorPos(m_ptCenter.x, m_ptCenter.y);
}

void KSW3DApp::Render(void)
{
	if(m_pTile) m_pTile->Render();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	HRESULT hr;
	if(m_pMesh) hr = m_pMesh->DrawSubset(0);

	//////// test /////////
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(&matWorld, 4.0f, 4.0f, 4.0f);
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	if(m_pMesh) m_pMesh->DrawSubset(0);
	D3DXMatrixTranslation(&matWorld, 0.0f, 0.0f, 0.0f);
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
}

HRESULT KSW3DApp::Release(void)
{
	// this app is not used when released
	m_bUsed = false;
	// all camera is not used
	m_pCurrentCam->Use(false);
	// delete all cameras
	DeleteAllCamera();

	return S_OK;
}

HRESULT KSW3DApp::RegisterCamera(PTCHAR cameraName, KSW3DCamera* cam)
{
	map<tstring, KSW3DCamera*>::iterator it;
	it = m_cams.find(cameraName);
	if(it == m_cams.end())
	{
		m_cams.insert(map<tstring, KSW3DCamera*>::value_type(cameraName, cam));
		return S_OK;
	}

	else
	{
		return E_FAIL;
	}
}

HRESULT KSW3DApp::SelectCamera(PTCHAR cameraName)
{
	map<tstring, KSW3DCamera*>::iterator it;
	it = m_cams.find(cameraName);
	if(it == m_cams.end()) return E_FAIL;	// if not exist, return E_FAIL

	if(m_pCurrentCam) m_pCurrentCam->Use(false);
	m_pCurrentCam = it->second;
	m_pCurrentCam->Use();
	return S_OK;
}

HRESULT KSW3DApp::SelectCamera(KSW3DCamera* cam)
{
	if(cam == NULL) return E_FAIL;
	if(m_pCurrentCam) m_pCurrentCam->Use(false);
	m_pCurrentCam = cam;
	m_pCurrentCam->Use();
	return S_OK;
}

HRESULT KSW3DApp::DeleteCamera(PTCHAR cameraName)
{
	KSW3DCamera* cam = GetCameraByName(cameraName);
	if(cam == NULL || cam->IsUsed())	// if camera is current used or not exist
	{
		return E_FAIL;
	}

	delete cam;		// free memory for the camera
	m_cams.erase(cameraName);	// remove camera
	return S_OK;
}

void KSW3DApp::DeleteAllCamera(void)
{
	map<tstring, KSW3DCamera*>::iterator it;
	for(it = m_cams.begin(); it != m_cams.end(); ++it)
	{
		if(it->second->IsUsed()) continue;		// if used, cannot remove
		delete it->second;	// free memory for the camera
	}

	m_cams.clear();		// clear the map for cameras
}

LPDIRECT3DDEVICE9 KSW3DApp::GetDevice(void) const { return m_pAppWnd->GetDirect3DDevice(); }

KSW3DCamera* KSW3DApp::GetCameraByName(PTCHAR cameraName)
{
	map<tstring, KSW3DCamera*>::iterator it;
	it = m_cams.find(cameraName);			// find the camera
	if(it == m_cams.end()) return NULL;		// if cannot find camera, return E_FAIL
	else return it->second;		// if find camera return the camera pointer
}

void KSW3DApp::OnLostDevice(void)
{
	if(m_pTile) m_pTile->FreeResources();
	///////////// test ///////////
	if(m_pMesh)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
}

void KSW3DApp::OnResetDevice(void)
{
	if(m_pTile) m_pTile->ValidateResources();

	// test ////
	D3DXCreateSphere(m_pDevice, 2.0f, 60, 80, &m_pMesh, NULL);

	m_pDevice->SetViewport(&m_viewport);
	m_pCurrentCam->Use();

	SetCursorPos(m_ptCenter.x, m_ptCenter.y);
}