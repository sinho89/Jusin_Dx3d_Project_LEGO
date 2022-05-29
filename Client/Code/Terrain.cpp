#include "stdafx.h"
#include "Terrain.h"
#include "Export_Function.h"
#include "Engine_Include.h"

#include "ResourcesMgr.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "Pipeline.h"
#include "CollisionMgr.h"
#include "MouseCol.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv)
: CTerrainObject(pDevice)
, m_dwVtxCnt(0)
, m_pConvertVertex(NULL)
, m_pVertex(NULL)
, m_VtxCntX(0)
, m_VtxCntZ(0)
, m_Itv(1)
, m_fAngle(0.f)
{
	m_VtxCntX = _iCntX;
	m_VtxCntZ = _iCntZ;
	m_Itv = _iItv;

	m_dwVtxCnt			= m_VtxCntX * m_VtxCntZ;
	m_pVertex			= new ENGINE::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXTEX[m_dwVtxCnt];

}

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv, const wstring& wstrResourceKey)
: CTerrainObject(pDevice)
, m_dwVtxCnt(0)
, m_pConvertVertex(NULL)
, m_pVertex(NULL)
, m_VtxCntX(0)
, m_VtxCntZ(0)
, m_Itv(1)
, m_wstrResourceKey(wstrResourceKey)
, m_fAngle(0.f)
{

	m_VtxCntX = _iCntX;
	m_VtxCntZ = _iCntZ;
	m_Itv = _iItv;

	m_dwVtxCnt			= m_VtxCntX * m_VtxCntZ;
	m_pVertex			= new ENGINE::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXTEX[m_dwVtxCnt];
}

CTerrain::~CTerrain()
{
	Release();
}
HRESULT CTerrain::Initialize(void)
{
	m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, m_wstrResourceKey, m_pVertex);

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
	
	D3DXMATRIX	matRot, matTrans;

	D3DXMatrixRotationY(&matRot, m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);
	D3DXMatrixTranslation(&matTrans, m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

	m_pInfo->m_matWorld = matRot * matTrans;

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
	}
}

void CTerrain::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

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
}

HRESULT CTerrain::AddComponent(void)
{
	ENGINE::CComponent*		pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture Terrain");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
	 //Buffer
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, m_wstrResourceKey);//L"Buffer_Terrain");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

const ENGINE::VTXTEX* CTerrain::GetTerrainVertex(void)
{
	return m_pVertex;
}

DWORD* CTerrain::GetVtxCnt()
{
	return &m_dwVtxCnt;
}	

ENGINE::VTXTEX* CTerrain::GetVtxTex()
{
	return m_pVertex;
}

ENGINE::VTXTEX* CTerrain::GetVtxConvertTex()
{
	return m_pConvertVertex;
}

int* CTerrain::GetVtxCntX()
{
	return &m_VtxCntX;
}

int* CTerrain::GetVtxCntZ()
{
	return &m_VtxCntZ;
}

int* CTerrain::GetVtxItv()
{	
	return &m_Itv;
}

