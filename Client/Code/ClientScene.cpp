#include "stdafx.h"
#include "Export_Function.h"
#include "ClientScene.h"
#include "SoundMgr.h"

CClientScene::CClientScene(LPDIRECT3DDEVICE9 pDevice)
: ENGINE::CScene(pDevice)
, m_pManagement(ENGINE::Get_Management())
, m_pResourcesMgr(ENGINE::Get_ResourceMgr())
, m_pLayerMgr(ENGINE::Get_LayerMgr())
, m_pCameraMgr(ENGINE::Get_CameraMgr())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pSoundMgr(ENGINE::CSoundMgr::GetInstance())
{
	m_fTime = 0.f;
}

CClientScene::~CClientScene(void)
{

}

HRESULT CClientScene::Initialize(void)
{
	return S_OK;
}

void CClientScene::Update(void)
{

}

void CClientScene::Render(void)
{

}



void CClientScene::Release(void)
{

}


ENGINE::TERRAIN_INFO* CClientScene::LoadTerrain(void)
{
	return NULL;
}

HRESULT CClientScene::AddBuffer(void)
{
	return S_OK;
}

HRESULT CClientScene::AddTexture(void)
{
	return S_OK;
}

HRESULT CClientScene::AddObject(void)
{
	return S_OK;
}
