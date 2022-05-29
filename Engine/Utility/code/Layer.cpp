#include "Layer.h"
#include "GameObject.h"
#include "Component.h"

ENGINE::CLayer::CLayer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
{

}

ENGINE::CLayer::~CLayer()
{
	Release();
}

void ENGINE::CLayer::Update(void)
{
	MAPOBJLIST::iterator	mapiter		= m_mapObjList.begin();
	MAPOBJLIST::iterator	mapiter_end = m_mapObjList.end();

	for( ;mapiter != mapiter_end; ++mapiter)
	{
		OBJLIST::iterator		iterList     = mapiter->second.begin();
		OBJLIST::iterator		iterList_end = mapiter->second.end();

		for( ;iterList != iterList_end; ++iterList)
		{
			(*iterList)->Update();
		}
	}
}

void ENGINE::CLayer::Render(void)
{
	MAPOBJLIST::iterator	mapiter		= m_mapObjList.begin();
	MAPOBJLIST::iterator	mapiter_end = m_mapObjList.end();

	for( ;mapiter != mapiter_end; ++mapiter)
	{
		OBJLIST::iterator		iterList     = mapiter->second.begin();
		OBJLIST::iterator		iterList_end = mapiter->second.end();

		for( ;iterList != iterList_end; ++iterList)
		{
			(*iterList)->Render();
		}
	}
}

void ENGINE::CLayer::Release(void)
{
	MAPOBJLIST::iterator	mapiter		= m_mapObjList.begin();
	MAPOBJLIST::iterator	mapiter_end = m_mapObjList.end();

	for( ;mapiter != mapiter_end; ++mapiter)
	{
		OBJLIST::iterator		iterList     = mapiter->second.begin();
		OBJLIST::iterator		iterList_end = mapiter->second.end();

		for( ;iterList != iterList_end; ++iterList)
		{
			ENGINE::Safe_Delete(*iterList);
		}
		mapiter->second.clear();
	}
	m_mapObjList.clear();
}

ENGINE::CLayer* ENGINE::CLayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	return new CLayer(pDevice);
}

HRESULT ENGINE::CLayer::AddObject(const wstring& wstrObjkey, 
								  CGameObject* pGameObject)
{
	if(pGameObject)
	{
		MAPOBJLIST::iterator	iter = m_mapObjList.find(wstrObjkey);

		if(iter == m_mapObjList.end())
		{
			m_mapObjList[wstrObjkey] = OBJLIST();
		}
		m_mapObjList[wstrObjkey].push_back(pGameObject);
	}

	return S_OK;
}

const ENGINE::CComponent* ENGINE::CLayer::GetComponent(const wstring& wstrObjKey, 
													   const wstring& wstrComponentKey)
{
	MAPOBJLIST::iterator		mapiter = m_mapObjList.find(wstrObjKey);

	if(mapiter == m_mapObjList.end())
		return NULL;

	OBJLIST::iterator		iterList		= mapiter->second.begin();
	OBJLIST::iterator		iterList_end	= mapiter->second.end();

	for( ; iterList != iterList_end; ++iterList)
	{
		const CComponent*	pComponent = (*iterList)->GetComponent(wstrComponentKey);

		if(pComponent != NULL)
			return pComponent;
	}

	return NULL;
}

const ENGINE::VTXTEX* ENGINE::CLayer::GetTerrainVtx(const wstring& wstrObjKey)
{
	MAPOBJLIST::iterator  iter = m_mapObjList.find(wstrObjKey);

	if(iter == m_mapObjList.end())
		return NULL;

	return iter->second.front()->GetTerrainVertex();
}

