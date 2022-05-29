#include "Scene.h"
#include "LayerMgr.h"

ENGINE::CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{
	
}

ENGINE::CScene::~CScene(void)
{
	Release();
}

HRESULT ENGINE::CScene::Initialize(void)
{
	return S_OK;
}

void ENGINE::CScene::Update(void)
{
	ENGINE::CLayerMgr::GetInstance()->Update();
}

void ENGINE::CScene::Render(void)
{	
	ENGINE::CLayerMgr::GetInstance()->Render();
}

void ENGINE::CScene::Release(void)
{
	ENGINE::CLayerMgr::GetInstance()->Release();
}

HRESULT ENGINE::CScene::AddBuffer(void)
{
	return S_OK;
}

HRESULT ENGINE::CScene::AddTexture(void)
{
	return S_OK;
}

HRESULT ENGINE::CScene::AddObject(void)
{
	return S_OK;
}

