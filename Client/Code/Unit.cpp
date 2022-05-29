#include "stdafx.h"
#include "Unit.h"


CUnit::CUnit(LPDIRECT3DDEVICE9 pDevice)
: CClientObj(pDevice)
, m_eUnitStateType(ENGINE::US_IDLE)
, m_fSpeed(0.f)
, m_iHp(3)
{
	for(int i=0; i<ENGINE::BP_END; ++i)
		m_pBodyPart[i] = NULL;

	for(int i=0; i<ENGINE::MAT_END; ++i)
		D3DXMatrixIdentity(&m_mMat[i]);
}

CUnit::~CUnit(void)
{
	Release();
}

HRESULT CUnit::Initialize(void)
{
	return S_OK;
}

void CUnit::Update(void)
{
	SetDirection();
}

void CUnit::Render(void)
{

}

void CUnit::Release(void)
{

}

CUnit* CUnit::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CUnit* pUnit = new CUnit(pDevice);

	if(FAILED(pUnit->Initialize()))
		ENGINE::Safe_Delete(pUnit);

	return pUnit;
}

HRESULT CUnit::AddComponent(void)
{
	ENGINE::CComponent*		pComponent = NULL;

	pComponent = m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));


	return S_OK;
}

void CUnit::SetDirection(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &ENGINE::g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);
}
