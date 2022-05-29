#include "stdafx.h"
#include "MainApp.h"

#include "Engine_Include.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "CameraMgr.h"

CMainApp::CMainApp(void)
: m_pDevice(NULL)
, m_pGraphicDev(ENGINE::Get_GraphicDev())
, m_pManagement(ENGINE::Get_Management())
, m_pTimeMgr(ENGINE:: Get_Time())
, m_pCameraMgr(ENGINE::Get_CameraMgr())
{
	//AllocConsole(); 
	//freopen( "CONOUT$",  "wt", stdout);
	//SetConsoleTitleA("Debug");
}

CMainApp::~CMainApp(void)
{
	Release();
	//FreeConsole();
}

void CMainApp::Update(void)
{
	m_pTimeMgr->SetTime();
	m_pManagement->Update();
}

void CMainApp::Render(void)
{
	m_pManagement->Render(m_pTimeMgr->GetTime());
}

HRESULT CMainApp::Initialize(void)
{
	HRESULT	hr	=	NULL;

	

	hr = m_pGraphicDev->InitGraphicDev(ENGINE::CGraphicDev::MODE_FULL, 
										ENGINE::g_hWnd, 1920, 1080);//ENGINE::WINCX, ENGINE::WINCY);
	FAILED_CHECK(hr);

	m_pDevice = m_pGraphicDev->GetDevice();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	hr = m_pManagement->InitManagement(m_pDevice);
	FAILED_CHECK_MSG(hr, L"Management Init Failed");

	hr = m_pManagement->SceneChange(CSceneSelector(ENGINE::SC_LOGO));
	FAILED_CHECK_MSG(hr, L"Scene Change Failed");

	m_pTimeMgr->InitTime();
	m_pCameraMgr->CamInit(m_pDevice);

	return S_OK;
}

void CMainApp::Release(void)
{
	ENGINE::Safe_Single_Destory(m_pTimeMgr);
	ENGINE::Safe_Single_Destory(m_pManagement);
	ENGINE::Safe_Single_Destory(m_pGraphicDev);
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pMainApp = new CMainApp;

	if(FAILED(pMainApp->Initialize()))
		ENGINE::Safe_Delete(pMainApp);

	return pMainApp;
}

