#include "stdafx.h"
#include "Ring.h"

CRing::CRing(LPDIRECT3DDEVICE9 pDevice)
: CUnit(pDevice)
, m_fRotZ(0.f)
{

}

CRing::~CRing(void)
{
	Release();
}

HRESULT CRing::Initialize(void)
{
	m_dwVtxCnt			= 4;
	m_pVertex			= new ENGINE::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXTEX[m_dwVtxCnt];

	m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer Ring", m_pVertex);

	FAILED_CHECK(AddComponent());
	return S_OK;
}

HRESULT CRing::AddComponent(void)
{
	CUnit::AddComponent();

	ENGINE::CComponent*		pComponent = NULL;

	// Buffer
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer Ring");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture Ring");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;	
}

void CRing::Update(void)
{
	CUnit::SetDirection();

	m_fRotZ += 2.f;

	D3DXMatrixScaling(&m_mMat[ENGINE::MAT_SCALE], 10.f, 10.f, 10.f);
	D3DXMatrixRotationX(&m_mMat[ENGINE::MAT_ROT_X], D3DXToRadian(m_fRotZ));
	D3DXMatrixTranslation(&m_mMat[ENGINE::MAT_TRANS], m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

	D3DXMatrixIdentity(&m_pInfo->m_matWorld);
	m_pInfo->m_matWorld =	m_mMat[ENGINE::MAT_SCALE] *  
		m_mMat[ENGINE::MAT_ROT_X] * 
		m_mMat[ENGINE::MAT_ROT_Y] * 
		m_mMat[ENGINE::MAT_ROT_Z] * 
		m_mMat[ENGINE::MAT_TRANS] * 
		m_mMat[ENGINE::MAT_AXIS] *
		m_mMat[ENGINE::MAT_PARENT];

	D3DXMATRIX		matBill;
	D3DXMatrixIdentity(&matBill);

	matBill = *m_pCameraMgr->GetView();
	ZeroMemory(&matBill.m[3][0], sizeof(D3DXVECTOR3));

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_pInfo->m_matWorld = matBill * m_pInfo->m_matWorld;

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
	}
}

void CRing::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000000);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer Ring", m_pConvertVertex);

	m_pTexture->Render(0);

	if(m_pBuffer != NULL)
		m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CRing::Release(void)
{

}

CRing* CRing::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRing* pRing = new CRing(pDevice);

	if(FAILED(pRing->Initialize()))
		ENGINE::Safe_Delete(pRing);

	return pRing;
}

