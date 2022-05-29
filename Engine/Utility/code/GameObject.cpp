#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

ENGINE::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_pInfo(NULL)
, m_bAlive(true)
{

}

ENGINE::CGameObject::~CGameObject()
{
	Release();
}

HRESULT ENGINE::CGameObject::Initialize(void)
{
	return S_OK;
}

void ENGINE::CGameObject::Update(void)
{
	MAPCOMPONENT::iterator		iter		= m_mapComponent.begin();
	MAPCOMPONENT::iterator		iter_end	= m_mapComponent.end();

	for( ;iter != iter_end; ++iter)
	{
		iter->second->Update();
	}
}

void ENGINE::CGameObject::Render(void)
{	

}

void ENGINE::CGameObject::Release(void)
{
	for_each(m_mapComponent.begin(), m_mapComponent.end(), CDeleteMap());
	m_mapComponent.clear();
}

ENGINE::CComponent* ENGINE::CGameObject::GetComponent(const wstring& wstrComponentKey)
{
	MAPCOMPONENT::iterator	iter = m_mapComponent.find(wstrComponentKey);

	if(iter == m_mapComponent.end())
		return NULL;

	return iter->second;
}

D3DXMATRIX* ENGINE::CGameObject::GetWorldMatrix(void)
{
	return &m_pInfo->m_matWorld;
}

D3DXMATRIX* ENGINE::CGameObject::GetViewMatrix(void)
{
	return &m_pInfo->m_matView;
}

D3DXMATRIX* ENGINE::CGameObject::GetProjMatrix(void)
{
	return &m_pInfo->m_matProj;
}

LPDIRECT3DDEVICE9* ENGINE::CGameObject::GetDevice(void)
{
	return &m_pDevice;
}



bool& ENGINE::CGameObject::GetAlive(void)
{
	return m_bAlive;
}

void ENGINE::CGameObject::SetAlive(bool bAlive)
{
	m_bAlive = bAlive;
}

