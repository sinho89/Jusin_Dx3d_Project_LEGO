#include "stdafx.h"
#include "Logo.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "LogoBack.h"
#include "SoundMgr.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pDevice)
: CClientScene(pDevice)
{

}

CLogo::~CLogo(void)
{
	Release();
}

HRESULT CLogo::Initialize(void)
{
	AddBuffer();
	AddTexture();
	AddObject();

	

	m_pLayerMgr->SetDivisionType(ENGINE::CLayerMgr::DIV_LOGO);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

void CLogo::Update(void)
{
	if(GetAsyncKeyState(VK_RETURN))
	{
		m_pLayerMgr->Release();
		m_pManagement->SceneChange(CSceneSelector(ENGINE::SC_STAGE_1));
		return;
	}

	ENGINE::CScene::Update();
}

void CLogo::Render(void)
{
	ENGINE::CScene::Render();
}

HRESULT CLogo::AddBuffer(void)
{
	HRESULT	hr = NULL;	

	hr = m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer RcTex");
	FAILED_CHECK(hr);

	return S_OK;
}

HRESULT CLogo::AddTexture(void)
{
	HRESULT	hr = NULL;	

	hr = m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture Logo", 
		L"../../Texture/Logo/Logo_Lord_of_The_Ring.jpg", 1);
	FAILED_CHECK(hr);

	return S_OK;
}

HRESULT CLogo::AddObject(void)
{
	HRESULT	hr = NULL;	

	ENGINE::CGameObject* pGameObject = NULL;

	pGameObject = CLogoBack::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	m_pLayerMgr->AddObject(L"LogoBack", pGameObject, ENGINE::LAYER_UI);

	return S_OK;
}

void CLogo::Release(void)
{
	ENGINE::Safe_Single_Destory(m_pResourcesMgr);
	ENGINE::Safe_Single_Destory(m_pLayerMgr);
}


CLogo* CLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogo*	pLogo = new CLogo(pDevice);

	if(FAILED(pLogo->Initialize()))
		ENGINE::Safe_Delete(pLogo);

	return pLogo;
}



ENGINE::TERRAIN_INFO* CLogo::LoadTerrain( void )
{
	return NULL;
}


