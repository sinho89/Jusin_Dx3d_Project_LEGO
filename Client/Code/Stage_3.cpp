#include "stdafx.h"
#include "Stage_3.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "Pipeline.h"
#include "Component.h"
#include "Transform.h"
#include "InfoSubject.h"

CStage_3::CStage_3(LPDIRECT3DDEVICE9 pDevice)
: CClientScene(pDevice)
{

}

CStage_3::~CStage_3(void)
{
	Release();
}

HRESULT CStage_3::Initialize(void)
{
	HRESULT		hr = NULL;

	return S_OK;
}

void CStage_3::Update(void)
{
	CScene::Update();
}

void CStage_3::Render(void)
{
	CScene::Render();
}

void CStage_3::Release(void)
{
	ENGINE::Safe_Single_Destory(m_pResourcesMgr);
	ENGINE::Safe_Single_Destory(m_pLayerMgr);
}

CStage_3* CStage_3::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage_3*	pStage_3 = new CStage_3(pDevice);

	if(FAILED(pStage_3->Initialize()))
		ENGINE::Safe_Delete(pStage_3);

	return pStage_3;
}

HRESULT CStage_3::AddBuffer(void)
{
	return S_OK;
}

HRESULT CStage_3::AddTexture(void)
{
	return S_OK;
}

HRESULT CStage_3::AddObject(void)
{
	return S_OK;
}

ENGINE::TERRAIN_INFO* CStage_3::LoadTerrain( void )
{
	return NULL;
}


