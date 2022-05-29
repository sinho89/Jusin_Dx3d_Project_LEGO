#include "stdafx.h"
#include "LeftLeg.h"

CLeftLeg::CLeftLeg(LPDIRECT3DDEVICE9 pDevice)
: CCubeObject(pDevice)
{
}

CLeftLeg::CLeftLeg(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType)
: CCubeObject(pDevice)
, m_eUnitType(eUnitType)
{

}

CLeftLeg::~CLeftLeg(void)
{
	Release();
}

HRESULT CLeftLeg::Invalidate(void)
{
	CCubeObject::Initialize();

	switch(m_eUnitType)
	{
	case UT_PLAYER_ELOND:
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Normal_Cube", m_pVertex);
		break;

	case UT_PLAYER_ESILDUR:
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Normal_Cube", m_pVertex);
		break;

	case UT_ORC:
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Orc_Cube", m_pVertex);
		break;

	case UT_ELF_SOLDIER:
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Normal_Cube", m_pVertex);
		break;

	case UT_BOSS:
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Boss_Cube", m_pVertex);
		break;
	}

	FAILED_CHECK(AddComponent());

	return S_OK;
}

HRESULT CLeftLeg::AddComponent(void)
{
	ENGINE::CComponent*		pComponent = NULL;

	pComponent = m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	switch(m_eUnitType)
	{
	case UT_PLAYER_ESILDUR:
		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Normal_Cube");
		m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture EsildurArmLeg");
		m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"EsildurArmLeg", pComponent));
		break;

	case UT_PLAYER_ELOND:
		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Normal_Cube");
		m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture ElondArmLeg");
		m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"ElondArmLeg", pComponent));
		break;

	case UT_ORC:
		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Orc_Cube");
		m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture OrcArmLeg");
		m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"OrcArmLeg", pComponent));
		break;

	case UT_ELF_SOLDIER:
		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Normal_Cube");
		m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture ElfArmLeg");
		m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"ElfArmLeg", pComponent));
		break;

	case UT_BOSS:
		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Boss_Cube");
		m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture OrcFace");
		m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
		NULL_CHECK_RETURN(m_pTexture, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"OrcFace", pComponent));
		break;
	}


	return S_OK;
}	

void CLeftLeg::Update(void)
{
	CCubeObject::SetDirection();

	if(m_eUnitType == UT_BOSS)
		D3DXMatrixScaling(&m_mMat[ENGINE::MAT_SCALE], 40.f, 40.f, 40.f);
	else
		D3DXMatrixScaling(&m_mMat[ENGINE::MAT_SCALE], 5.f, 5.f, 5.f);

	D3DXMatrixRotationX(&m_mMat[ENGINE::MAT_ROT_X], m_pInfo->m_fAngle[ENGINE::ANGLE_X]);
	D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);
	D3DXMatrixRotationZ(&m_mMat[ENGINE::MAT_ROT_Z], m_pInfo->m_fAngle[ENGINE::ANGLE_Z]);
	
	if(m_eUnitType == UT_BOSS)
		D3DXMatrixTranslation(&m_mMat[ENGINE::MAT_TRANS], m_pInfo->m_vPos.x * 40.f, m_pInfo->m_vPos.y * 40.f, m_pInfo->m_vPos.z * 40.f);
	else
		D3DXMatrixTranslation(&m_mMat[ENGINE::MAT_TRANS], m_pInfo->m_vPos.x * 5.f, m_pInfo->m_vPos.y * 5.f, m_pInfo->m_vPos.z * 5.f);

	D3DXMatrixIdentity(&m_pInfo->m_matWorld);
	m_pInfo->m_matWorld =	m_mMat[ENGINE::MAT_SCALE] *  
		m_mMat[ENGINE::MAT_ROT_X] * 
		m_mMat[ENGINE::MAT_ROT_Y] * 
		m_mMat[ENGINE::MAT_ROT_Z] * 
		m_mMat[ENGINE::MAT_TRANS] * 
		m_mMat[ENGINE::MAT_AXIS] *
		m_mMat[ENGINE::MAT_PARENT];

	CCubeObject::Update();
}

void CLeftLeg::Render(void)
{
	CCubeObject::Render();

	switch(m_eUnitType)
	{
	case UT_PLAYER_ELOND:
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Normal_Cube", m_pConvertVertex);
		break;

	case UT_PLAYER_ESILDUR:
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Normal_Cube", m_pConvertVertex);
		break;

	case UT_ORC:
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Orc_Cube", m_pConvertVertex);
		break;

	case UT_ELF_SOLDIER:
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Normal_Cube", m_pConvertVertex);
		break;

	case UT_BOSS:
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Boss_Cube", m_pConvertVertex);
		break;
	}

	m_pTexture->Render(0);
	
	if(m_pBuffer != NULL)
		m_pBuffer->Render();
}

void CLeftLeg::Release(void)
{

}

CLeftLeg* CLeftLeg::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLeftLeg* pLeftLeg = new CLeftLeg(pDevice);

	if(FAILED(pLeftLeg->Invalidate()))
		ENGINE::Safe_Delete(pLeftLeg);

	return pLeftLeg;
}

CLeftLeg* CLeftLeg::Create(LPDIRECT3DDEVICE9 pDevice, UNITTYPE m_eUnitType)
{
	CLeftLeg* pLeftLeg = new CLeftLeg(pDevice, m_eUnitType);

	if(FAILED(pLeftLeg->Invalidate()))
		ENGINE::Safe_Delete(pLeftLeg);

	return pLeftLeg;
}
