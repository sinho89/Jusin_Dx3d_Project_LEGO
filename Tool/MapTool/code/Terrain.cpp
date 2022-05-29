#include "stdafx.h"
#include "Terrain.h"
#include "Export_Function.h"

#include "ResourcesMgr.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "InfoSubject.h"
#include "CollisionMgr.h"
#include "MouseCol.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv)
: CGameObject(pDevice)
, m_pResourcesMgr(ENGINE::Get_ResourceMgr())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfo(NULL)
, m_dwVtxCnt(0)
, m_pConvertVertex(NULL)
, m_pVertex(NULL)
, m_VtxCntX(0)
, m_VtxCntZ(0)
, m_Itv(1)
, m_pMouseCol(NULL)
, m_pCollisionMgr(CCollisionMgr::GetInstance())
{
	m_VtxCntX = _iCntX;
	m_VtxCntZ = _iCntZ;
	m_Itv = _iItv;

	m_dwVtxCnt			= m_VtxCntX * m_VtxCntZ;
	m_pVertex			= new ENGINE::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXTEX[m_dwVtxCnt];

	m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Terrain", m_pVertex);
}

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv, const wstring& wstrResourceKey)
: CGameObject(pDevice)
, m_pResourcesMgr(ENGINE::Get_ResourceMgr())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfo(NULL)
, m_dwVtxCnt(0)
, m_pConvertVertex(NULL)
, m_pVertex(NULL)
, m_VtxCntX(0)
, m_VtxCntZ(0)
, m_Itv(1)
, m_pMouseCol(NULL)
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_wstrResourceKey(wstrResourceKey)
{
	m_VtxCntX = _iCntX;
	m_VtxCntZ = _iCntZ;
	m_Itv = _iItv;

	m_dwVtxCnt			= m_VtxCntX * m_VtxCntZ;
	m_pVertex			= new ENGINE::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXTEX[m_dwVtxCnt];

	m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, wstrResourceKey, m_pVertex);
}

CTerrain::~CTerrain()
{
	Release();
}
HRESULT CTerrain::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	FAILED_CHECK(AddComponent());

	return S_OK;
}

void CTerrain::SetDirection(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &ENGINE::g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv)
{
	CTerrain*		pTerrain = new CTerrain(pDevice, _iCntX, _iCntZ, _iItv);

	if(FAILED(pTerrain->Initialize()))
		ENGINE::Safe_Delete(pTerrain);

	return pTerrain;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv, const wstring& wstrResourceKey)
{
	CTerrain*		pTerrain = new CTerrain(pDevice, _iCntX, _iCntZ, _iItv, wstrResourceKey);

	if(FAILED(pTerrain->Initialize()))
		ENGINE::Safe_Delete(pTerrain);

	return pTerrain;
}

void CTerrain::Update(void)
{
	SetDirection();	

	ENGINE::CPipeline::MakeWorldMatrix(&m_pInfo->m_matWorld, 
		&D3DXVECTOR3(1.f, 1.f, 1.f), 
		m_pInfo->m_fAngle, 
		&m_pInfo->m_vPos);

	SetTransform();

}

void CTerrain::Render(void)
{
	m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, m_wstrResourceKey, m_pConvertVertex);
	
	m_pTexture->Render(0);
	m_pBuffer->Render();

}

void CTerrain::Release(void)
{
	if(m_pVertex != NULL)
	{
		ENGINE::Safe_Delete_Array(m_pVertex);
	}

	if(m_pConvertVertex != NULL)
	{
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
	}

	m_pInfoSubject->UnSubscribe(m_pCameraObserver);
	ENGINE::Safe_Delete(m_pCameraObserver);
}

HRESULT CTerrain::AddComponent(void)
{
	ENGINE::CComponent*		pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	pComponent = m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_MOUSE);
	m_pMouseCol = dynamic_cast<CMouseCol*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"MouseCol", pComponent));
	
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture Terrain");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	// Buffer
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, m_wstrResourceKey);//L"Buffer_Terrain");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}


void CTerrain::SetTransform(void)
{
	const D3DXMATRIX*		pMatView = m_pCameraObserver->GetView();
	NULL_CHECK(pMatView);

	const D3DXMATRIX*		pMatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pMatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pMatView);

		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pMatProj);

	}



}

void CTerrain::SetVtx(int iCntX, int iCntZ, int iItv)
{
	m_VtxCntX = iCntX;
	m_VtxCntZ = iCntZ;
	m_Itv = iItv;
}

bool CTerrain::PickTerrain(CPoint _Point, float _fHeight)
{
	m_pMouseCol->PickTerrain(_Point, &m_vDestPos, m_pVertex, m_VtxCntX, m_VtxCntZ, m_Itv, _fHeight);
	return false;
}

