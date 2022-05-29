#include "stdafx.h"
#include "Player2Skill.h"
#include "Export_Function.h"
#include "Player.h"
#include "Pipeline.h"
#include "CameraMgr.h"


CPlayer2_Skill::CPlayer2_Skill(LPDIRECT3DDEVICE9 pDevice, float fX, float fY)
: CRcObject(pDevice)
, m_bClick(false)
{
	m_fX = fX;
	m_fY = fY;
}

CPlayer2_Skill::~CPlayer2_Skill(void)
{
	Release();
}

HRESULT CPlayer2_Skill::Initialize(void)
{
	m_pInfo->m_vPos.z = 0.f;

	m_dwVtxCnt			= 4;
	m_pVertex			= new ENGINE::VTXTEX[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXTEX[m_dwVtxCnt];

	m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer RcHealSkill", m_pVertex);
	FAILED_CHECK(AddComponent());


	m_pInfo->m_vPos.x = m_fX;
	m_pInfo->m_vPos.y = m_fY;

	return S_OK;
}

HRESULT CPlayer2_Skill::AddComponent(void)
{
	ENGINE::CComponent*		pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// Buffer
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer RcHealSkill");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"RcHealSkill", pComponent));

	// Texture
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture HealSkill");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"HealSkill", pComponent));

	return S_OK;
}

CPlayer2_Skill* CPlayer2_Skill::Create(LPDIRECT3DDEVICE9 pDevice, float fX, float fY)
{
	CPlayer2_Skill*		pSkill = new CPlayer2_Skill(pDevice, fX, fY);

	if(FAILED(pSkill->Initialize()))
		ENGINE::Safe_Delete(pSkill);

	return pSkill;
}

void CPlayer2_Skill::Update(void)
{
	ENGINE::CGameObject::Update();	


	D3DXMATRIX matScale, matTrans;

	/*if(GetAsyncKeyState('4') & 0x8000)
	{
		m_bClick = true;
	}*/

	if(m_bClick)
	{
		D3DXMatrixScaling(&matScale, 20.f, 20.f, 20.f);
		m_bClick = false;
	}
	else
	{
		D3DXMatrixScaling(&matScale, 30.f, 30.f, 30.f);
	}
	D3DXMatrixTranslation(&matTrans, m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

	m_pInfo->m_matWorld = matScale * matTrans;
}

void CPlayer2_Skill::Render(void)
{
	if(m_pCameraMgr->GetDisType() != m_pCameraMgr->DIS_STAGE2_BOSS)
	{
		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
		m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		for(size_t i = 0; i < m_dwVtxCnt; ++i)
		{
			m_pConvertVertex[i] = m_pVertex[i];

			D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
		}

		D3DXMATRIX matView;

		D3DXMatrixIdentity(&matView);

		m_pDevice->SetTransform(D3DTS_VIEW, &matView);

		D3DXMatrixOrthoLH(&m_pInfo->m_matProj, ENGINE::WINCX, ENGINE::WINCY, 0.f ,1.f);

		m_pDevice->SetTransform(D3DTS_PROJECTION, &m_pInfo->m_matProj);

		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer RcHealSkill", m_pConvertVertex);

		m_pTexture->Render(0);
		m_pBuffer->Render();

		D3DXMatrixPerspectiveFovLH(
			&m_pInfo->m_matProj,
			D3DX_PI * 0.25f, // 45 - degree
			ENGINE::WINCX / ENGINE::WINCY,
			1.0f,
			1000.0f);

		m_pDevice->SetTransform(D3DTS_PROJECTION, &m_pInfo->m_matProj);

		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

void CPlayer2_Skill::Release(void)
{

}

void CPlayer2_Skill::SetPos(float fX, float fY, float fZ)
{
	m_pInfo->m_vPos.x = fX;
	m_pInfo->m_vPos.x = fY;
	m_pInfo->m_vPos.x = fZ;
}

