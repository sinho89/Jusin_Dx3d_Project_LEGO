#include "stdafx.h"
#include "Boss_2.h"

CBoss_2::CBoss_2(LPDIRECT3DDEVICE9 pDevice)
: CUnit(pDevice)
{

}

CBoss_2::~CBoss_2(void)
{
	Release();
}

HRESULT CBoss_2::Initialize(void)
{
	m_dwVtxCnt			= 4;
	m_pVertex			= new ENGINE::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXTEX[m_dwVtxCnt];

	m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer SauronEye", m_pVertex);

	FAILED_CHECK(AddComponent());
	return S_OK;
}

HRESULT CBoss_2::AddComponent(void)
{
	CUnit::AddComponent();

	ENGINE::CComponent*		pComponent = NULL;

	// Buffer
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer SauronEye");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture SauronEye");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;	
}

void CBoss_2::Update(void)
{
	CUnit::SetDirection();

	D3DXMatrixScaling(&m_mMat[ENGINE::MAT_SCALE], 60.f, 60.f, 60.f);
	D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], D3DXToRadian(80.f));
	D3DXMatrixTranslation(&m_mMat[ENGINE::MAT_TRANS], m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

	D3DXMatrixIdentity(&m_pInfo->m_matWorld);
	m_pInfo->m_matWorld =	m_mMat[ENGINE::MAT_SCALE] *  
							m_mMat[ENGINE::MAT_ROT_X] * 
							m_mMat[ENGINE::MAT_ROT_Y] * 
							m_mMat[ENGINE::MAT_ROT_Z] * 
							m_mMat[ENGINE::MAT_TRANS] * 
							m_mMat[ENGINE::MAT_AXIS] *
							m_mMat[ENGINE::MAT_PARENT];

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
	}
}

void CBoss_2::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000000);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer SauronEye", m_pConvertVertex);

	m_pTexture->Render(0);

	if(m_pBuffer != NULL)
		m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CBoss_2::Release(void)
{

}

CBoss_2* CBoss_2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoss_2* pBoss_2 = new CBoss_2(pDevice);

	if(FAILED(pBoss_2->Initialize()))
		ENGINE::Safe_Delete(pBoss_2);

	return pBoss_2;
}
