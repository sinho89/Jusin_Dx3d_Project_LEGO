#include "stdafx.h"
#include "CamTarget.h"

#include "Export_Function.h"
#include "ResourcesMgr.h"
#include "TimeMgr.h"

#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "InfoSubject.h"

CCamTarget::CCamTarget(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_pResourcesMgr(ENGINE::Get_ResourceMgr())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_fTime(0.f)
, m_pInfo(NULL)
, m_dwVtxCnt(0)
, m_pConvertVertex(NULL)
, m_pVertex(NULL)
, m_pCameraObserver(NULL)
{
	
}

CCamTarget::~CCamTarget()
{
	Release();
}

HRESULT CCamTarget::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_fDistance = 10.f;
	m_fCameraAngle = D3DXToRadian(45.f);
	m_fSpeed = 0.1f;

	m_dwVtxCnt			= 4;
	m_pVertex			= new ENGINE::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXTEX[m_dwVtxCnt];

	//m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer RcTex", m_pVertex);

	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos.x = 50.f;
	m_pInfo->m_vPos.y = 0.f;
	m_pInfo->m_vPos.z = 50.f;
	return S_OK;
}

void CCamTarget::KeyInput(void)
{

	m_fTime = m_pTimeMgr->GetTime();

	if(GetAsyncKeyState('W') & 0x8000)
	{
		m_pInfo->m_vPos += m_pInfo->m_vDir * m_fSpeed;
	}

	if(GetAsyncKeyState('S') & 0x8000)
	{
		m_pInfo->m_vPos -= m_pInfo->m_vDir * m_fSpeed;
	}

	if(GetAsyncKeyState('Q') & 0x8000)
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_Y] += D3DXToRadian(0.3f);
	}

	if(GetAsyncKeyState('E') & 0x8000)
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_Y] -= D3DXToRadian(0.3f);
	}

	if(GetAsyncKeyState('A') & 0x8000)
	{
		m_pInfo->m_vPos.x -= m_fSpeed;
	}

	if(GetAsyncKeyState('D') & 0x8000)
	{
		m_pInfo->m_vPos.x += m_fSpeed;
	}

	/*if(GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_fCameraAngle += D3DXToRadian(0.3f);
	}

	if(GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_fCameraAngle -= D3DXToRadian(0.3f);
	}

	if(GetAsyncKeyState('O') & 0x8000)
	{
		m_fDistance += m_fSpeed;
	}

	if(GetAsyncKeyState('P') & 0x8000)
	{
		m_fDistance -= m_fSpeed;
	}*/
}

void CCamTarget::SetDirection(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &ENGINE::g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);
}

CCamTarget* CCamTarget::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CCamTarget*		pCamTarget = new CCamTarget(pDevice);

	if(FAILED(pCamTarget->Initialize()))
		ENGINE::Safe_Delete(pCamTarget);

	return pCamTarget;
}

void CCamTarget::Update(void)
{
	KeyInput();
	SetDirection();
	
	ENGINE::CPipeline::MakeWorldMatrix(&m_pInfo->m_matWorld, 
		&D3DXVECTOR3(1.f, 1.f, 1.f), 
		m_pInfo->m_fAngle, 
		&m_pInfo->m_vPos);

	SetTransform();

}

void CCamTarget::Render(void)
{
	//m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer RcTex", m_pConvertVertex);

	//m_pTexture->Render(0);
	//m_pBuffer->Render();
	
}

void CCamTarget::Release(void)
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

HRESULT CCamTarget::AddComponent(void)
{

	m_fAngle = 0.f;

	ENGINE::CComponent*		pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	/*// Buffer
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer RcTex");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture Player");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));*/

	return S_OK;
}

void CCamTarget::StaticCamera(void)
{
	D3DXVECTOR3		vEye = m_pInfo->m_vDir * (-1);
	D3DXVec3Normalize(&vEye, &vEye);

	vEye *= m_fDistance;

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pInfo->m_matWorld.m[0][0], sizeof(float) * 3);

	D3DXMATRIX		matAxis;
	D3DXMatrixRotationAxis(&matAxis, &vRight, m_fCameraAngle);
	D3DXVec3TransformNormal(&vEye, &vEye, &matAxis);	

	vEye += m_pInfo->m_vPos;

	D3DXMATRIX		matView;
	ENGINE::CPipeline::MakeViewSpaceMatrix(&matView, 
		&vEye, 
		&m_pInfo->m_vPos, 
		&D3DXVECTOR3(0.f, 1.f, 0.f));

	m_pDevice->SetTransform(D3DTS_VIEW, &matView);
}

void CCamTarget::SetTransform(void)
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



