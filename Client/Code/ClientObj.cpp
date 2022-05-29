#include "stdafx.h"
#include "ClientObj.h"

CClientObj::CClientObj( LPDIRECT3DDEVICE9 pDevice )
: ENGINE::CGameObject(pDevice)
, m_pResourcesMgr(ENGINE::Get_ResourceMgr())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pManagement(ENGINE::Get_Management())
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_pCameraMgr(ENGINE::Get_CameraMgr())
, m_pLayerMgr(ENGINE::Get_LayerMgr())
, m_pSoundMgr(ENGINE::CSoundMgr::GetInstance())
{
	m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	
	m_pInfo->m_iHp = 5;
	
}

CClientObj::~CClientObj()
{
	Release();
}

void CClientObj::Update( void )
{
	ENGINE::CGameObject::Update();
}

void CClientObj::Render( void )
{

}

HRESULT CClientObj::Initialize( void )
{
	return S_OK;
}

D3DXMATRIX* CClientObj::GetViewMatrix(void)
{
	return &m_pInfo->m_matView;
}

D3DXMATRIX* CClientObj::GetProjMatrix(void)
{
	return &m_pInfo->m_matProj;
}

LPDIRECT3DDEVICE9* CClientObj::GetDevice(void)
{
	return &m_pDevice;
}

void CClientObj::SetInfo(ENGINE::SAVECUBE* pSaveInfo)
{
	m_pInfo->m_fAngle[ENGINE::ANGLE_X] = pSaveInfo->m_fAngle[ENGINE::ANGLE_X];
	m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = pSaveInfo->m_fAngle[ENGINE::ANGLE_Y];
	m_pInfo->m_fAngle[ENGINE::ANGLE_Z] = pSaveInfo->m_fAngle[ENGINE::ANGLE_Z];

	//m_pInfo->m_matWorld	=	pSaveInfo->m_matWorld;
	m_pInfo->m_vDir		=	pSaveInfo->m_vDir;
	m_pInfo->m_vPos		=	pSaveInfo->m_vPos;
	//SetPos(pSaveInfo->m_vPos);
}

void CClientObj::Release(void)
{
	//ENGINE::Safe_Delete(m_pInfo);

	//ENGINE::Safe_Single_Destory(m_pResourcesMgr);
	//ENGINE::Safe_Single_Destory(m_pTimeMgr);
	//ENGINE::Safe_Single_Destory(m_pManagement);
}

D3DXMATRIX* CClientObj::GetWorldMatrix(void)
{
	return &m_pInfo->m_matWorld;
}


