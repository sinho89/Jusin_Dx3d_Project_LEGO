#include "stdafx.h"
#include "LogoBack.h"
#include "Transform.h"
#include "ResourcesMgr.h"
#include "TimeMgr.h"

#include "Texture.h"

CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pDevice)
: CRcObject(pDevice)
{

}

CLogoBack::~CLogoBack(void)
{
	Release();
}

void CLogoBack::Update(void)
{

	//CCameraMgr::GetInstance()->
	/*D3DXMATRIX		matScale, matTrans;

	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.5f);
	D3DXMatrixTranslation(&matTrans, -0.5f, 0.5f, 0.f);

	m_pInfo->m_matWorld = matScale * matTrans;

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pInfo->m_matWorld);*/
}

void CLogoBack::Render(void)
{
	m_pTexture->Render(0);
	m_pBuffer->Render();
}

HRESULT CLogoBack::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	return S_OK;
}

HRESULT CLogoBack::AddComponent(void)
{	
	ENGINE::CComponent*		pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	m_pInfo->m_vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// Buffer
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer RcTex");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// Texture
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture Logo");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));


	return S_OK;
}

void CLogoBack::Release(void)
{

}

CLogoBack* CLogoBack::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CLogoBack*	pLogoBack = new CLogoBack(pDevice);

	if(FAILED(pLogoBack->Initialize()))
		ENGINE::Safe_Delete(pLogoBack);

	return pLogoBack;
}

D3DXMATRIX* CLogoBack::GetWorldMatrix(void)
{
	return &m_pInfo->m_matWorld;
}

D3DXMATRIX* CLogoBack::GetViewMatrix(void)
{
	return &m_pInfo->m_matView;
}

D3DXMATRIX* CLogoBack::GetProjMatrix(void)
{
	return &m_pInfo->m_matProj;
}

LPDIRECT3DDEVICE9* CLogoBack::GetDevice(void)
{
	return &m_pDevice;
}