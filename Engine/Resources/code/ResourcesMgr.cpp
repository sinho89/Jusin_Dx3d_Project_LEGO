#include "ResourcesMgr.h"
#include "Texture.h"
#include "CubeTex.h"
#include "CubeCol.h"
#include "TerrainTex.h"
#include "RcTerrain.h"
#include "RcTex.h"
#include "..\..\Client\Code\CubeObj.h"

IMPLEMENT_SINGLETON(ENGINE::CResourcesMgr)

ENGINE::CResourcesMgr::CResourcesMgr(void)
: m_pResources(NULL)
{

}

ENGINE::CResourcesMgr::~CResourcesMgr(void)
{
	Release();
}

void ENGINE::CResourcesMgr::SetTerrainBuffer(LPDIRECT3DDEVICE9 pDevice, 
											 RESOURCETYPE eResourceType, 
											 CVIBuffer::BUFFERTYPE eBufferType, 
											 const wstring& wstrResourceKey, 
											 int iCntX, 
											 int iCntZ, 
											 int iItv)
{
	MAPRESOURCE::iterator		iter = m_MapResoure[eResourceType].find(wstrResourceKey);

	if(iter == m_MapResoure[eResourceType].end())
	{
		return;
	}

	*((CRcTerrain*)(iter->second))->GetVtxCnt() = iCntX * iCntZ;
	*((CRcTerrain*)(iter->second))->GetTriCnt() = (iCntX - 1) * (iCntZ - 1) * 2;
}

HRESULT ENGINE::CResourcesMgr::AddBuffer(LPDIRECT3DDEVICE9 pDevice, 
										 RESOURCETYPE eResourceType, 
										 CVIBuffer::BUFFERTYPE eBufferType, 
										 const wstring& wstrResourceKey, 
										 const WORD& wCntX /*= 0*/, 
										 const WORD& wCntZ /*= 0*/, 
										 const WORD& wItv /*= 1*/)
{
	MAPRESOURCE::iterator		iter = m_MapResoure[eResourceType].find(wstrResourceKey);

	// 키값이 tchar* 형 일시
/*
	MAPRESOURCE::iterator		iter = find_if(m_MapResoure[eResourceType].begin(), 
		m_MapResoure[eResourceType].end(), CTagFinder(wstrResourceKey));*/

	if(iter != m_MapResoure[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"중복된 리소스");
		return E_FAIL;
	}

	CResources*		pResource = NULL;

	switch(eBufferType)
	{
	case CVIBuffer::BUFFER_TERRAIN:
		pResource = CTerrainTex::Create(pDevice, wCntX, wCntZ, wItv);
		break;

	case CVIBuffer::BUFFER_CUBECOL:
		pResource = CCubeCol::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_RCTEX:
		pResource = CRcTex::Create(pDevice);
		break;

	case CVIBuffer::BUFFER_CUBETEX:
		pResource = CCubeTex::Create(pDevice);
		break;
	}

	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_MapResoure[eResourceType].insert(MAPRESOURCE::value_type(wstrResourceKey, pResource));

	return S_OK;
}

HRESULT ENGINE::CResourcesMgr::AddObjBuffer(LPDIRECT3DDEVICE9 pDevice, 
										 RESOURCETYPE eResourceType, 
										 CVIBuffer::BUFFERTYPE eBufferType, 
										 const wstring& wstrResourceKey, 
										 const float& wItvX /*= 1*/, 
										 const float& wItvY /*= 1*/, 
										 const float& wItvZ /*= 1*/,
										 const DWORD& dwColor)
{
	MAPRESOURCE::iterator		iter = m_MapResoure[eResourceType].find(wstrResourceKey);

	if(iter != m_MapResoure[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"중복된 리소스");
		return E_FAIL;
	}

	CResources*		pResource = NULL;

	switch(eBufferType)
	{
	case CVIBuffer::BUFFER_CUBETEX_CUSTOM:
		pResource = CCubeTex::Create(pDevice, wItvX, wItvY, wItvZ);
		break;

	case CVIBuffer::BUFFER_CUBECOL_CUSTOM:
		pResource = CCubeCol::Create(pDevice, wItvX, wItvY, wItvZ, dwColor);
		break;
	}

	NULL_CHECK_RETURN(pResource, E_FAIL);

	m_MapResoure[eResourceType].insert(MAPRESOURCE::value_type(wstrResourceKey, pResource));

	return S_OK;
}

void ENGINE::CResourcesMgr::Render(const wstring& wstrResourceKey)
{
	MAPRESOURCE::iterator		iter = m_MapResoure[0].find(wstrResourceKey);

	if(iter == m_MapResoure[0].end())
		return;

	iter->second->Render();
}

void ENGINE::CResourcesMgr::Release(void)
{
	for(size_t i = 0; i < RESOURCE_END; ++i)
	{
		for_each(m_MapResoure[i].begin(), m_MapResoure[i].end(), CDeleteMap());
		m_MapResoure[i].clear();
	}


}

void ENGINE::CResourcesMgr::ResetDynamic(void)
{
	for_each(m_MapResoure[RESOURCE_DYNAMIC].begin(), m_MapResoure[RESOURCE_DYNAMIC].end(), CDeleteMap());
	m_MapResoure[RESOURCE_DYNAMIC].clear();
}

HRESULT ENGINE::CResourcesMgr::AddTexture(LPDIRECT3DDEVICE9 pDevice, 
										  RESOURCETYPE eResourceType, 
										  TEXTURETYPE eTextureType, 
										  const wstring& wstrResourcesKey, 
										  const wstring& wstrFilePath, 
										  const WORD& wCnt)
{
	MAPRESOURCE::iterator		iter = m_MapResoure[eResourceType].find(wstrResourcesKey);

	if(iter != m_MapResoure[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourcesKey.c_str(), L"중복된 리소스");
		return E_FAIL;
	}

	CResources*		pResources = CTexture::Create(pDevice, eTextureType, wstrFilePath, wCnt);

	NULL_CHECK_RETURN(pResources, E_FAIL);

	m_MapResoure[eResourceType].insert(MAPRESOURCE::value_type(wstrResourcesKey, pResources));

	return S_OK;
}

ENGINE::CComponent* ENGINE::CResourcesMgr::CloneResource(RESOURCETYPE eResourveType, 
												 const wstring& wstrResourceKey)
{
	MAPRESOURCE::iterator		iter = m_MapResoure[eResourveType].find(wstrResourceKey.c_str());

	if(iter == m_MapResoure[eResourveType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"복사 실패");
		return NULL;
	}

	return iter->second->CloneResource();
}

void ENGINE::CResourcesMgr::SetVtxInfo(RESOURCETYPE eResourceType, 
									   const wstring& wstrResourceKey, 
									   void* pVertex)
{
	MAPRESOURCE::iterator	iter = m_MapResoure[eResourceType].find(wstrResourceKey);

	if(iter == m_MapResoure[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"버텍스를 찾을 수 없음(Set)");
		return;
	}

	((CVIBuffer*)iter->second)->SetVtxInfo(pVertex);
}

void ENGINE::CResourcesMgr::GetVtxInfo(RESOURCETYPE eResourceType, 
									   const wstring& wstrResourceKey, 
									   void* pVertex)
{
	MAPRESOURCE::iterator	iter = m_MapResoure[eResourceType].find(wstrResourceKey);

	if(iter == m_MapResoure[eResourceType].end())
	{
		TAGMSG_BOX(wstrResourceKey.c_str(), L"버텍스를 찾을 수 없음(Get)");
		return;
	}

	((CVIBuffer*)iter->second)->GetVtxInfo(pVertex);
}


