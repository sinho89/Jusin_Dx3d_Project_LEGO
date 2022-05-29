#include "stdafx.h"
#include "CubeObj.h"

#include "Export_Function.h"
#include "Include.h"
#include "TimeMgr.h"
#include "Management.h"

#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "InfoSubject.h"
#include "CollisionMgr.h"
#include "Stage_1.h"

CCubeObj::CCubeObj(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_pResourcesMgr(ENGINE::Get_ResourceMgr())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pManagement(ENGINE::Get_Management())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_fTime(0.f)
, m_dwVtxCnt(0)
, m_pInfo(NULL)
, m_pConvertVertex(NULL)
, m_pVertex(NULL)
, m_pCameraObserver(NULL)
{

}
CCubeObj::~CCubeObj()
{
	Release();
}
HRESULT CCubeObj::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_fSpeed = 0.2f;

	m_dwVtxCnt			=	8;


	m_pVertex			=	new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pConvertVertex	=	new ENGINE::VTXCUBE[m_dwVtxCnt];

	m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer CubeCol", m_pVertex);


	FAILED_CHECK(AddComponent());
	return S_OK;
}

void CCubeObj::SetDirection(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);
}
HRESULT CCubeObj::AddComponent(void)
{
	ENGINE::CComponent*		pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// Buffer
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer CubeCol");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// Texture
	/*pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture Cube");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));*/

	return S_OK;
}
void CCubeObj::SetTransform(void)
{
	const D3DXMATRIX*		pMatView = m_pCameraObserver->GetView();
	NULL_CHECK(pMatView);

	const D3DXMATRIX*		pMatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pMatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{

		m_pConvertVertex[i] = m_pVertex[i];

		ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pMatView);

		if(m_pConvertVertex[i].vPos.z < 1.f)
			m_pConvertVertex[i].vPos.z = 1.f;

		ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pMatProj);

	}
}

CCubeObj* CCubeObj::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCubeObj*		pCubeObj = new CCubeObj(pDevice);

	if(FAILED(pCubeObj->Initialize()))
		ENGINE::Safe_Delete(pCubeObj);

	return pCubeObj;
}

void CCubeObj::Update(void)
{
	SetDirection();

	ENGINE::CPipeline::MakeWorldMatrix(&m_pInfo->m_matWorld, 
		&D3DXVECTOR3(1.f, 1.f, 1.f), 
		m_pInfo->m_fAngle, 
		&m_pInfo->m_vPos);

	ENGINE::CGameObject::Update();

	SetTransform();
}

void CCubeObj::Render(void)
{
	/*m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);*/


	m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer CubeCol", m_pConvertVertex);

	//m_pTexture->Render(0);
	m_pBuffer->Render();

	//m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CCubeObj::Release(void)
{
	if(m_pVertex != NULL)
	{
		ENGINE::Safe_Delete_Array(m_pVertex);
	}

	if(m_pConvertVertex != NULL)
	{
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
	}

	ENGINE::Safe_Delete(m_pCameraObserver);
}
void CCubeObj::SetPos(const D3DXVECTOR3& vPos)
{
	m_pInfo->m_vPos = vPos;
}
