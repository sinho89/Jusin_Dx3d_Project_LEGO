/*!
 * \file ResourcesMgr.h
 * \date 2016/07/14 10:38
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef ResourcesMgr_h__
#define ResourcesMgr_h__

#include "Engine_Include.h"
#include "VIBuffer.h"

BEGIN(ENGINE)

class CComponent;
class ENGINE_DLL CResourcesMgr
{
	DECLARE_SINGLETON(CResourcesMgr)

private:
	CResourcesMgr(void);
	~CResourcesMgr(void);

private:
	CResources*			m_pResources;

	typedef unordered_map<wstring, CResources*>		MAPRESOURCE;
	MAPRESOURCE			m_MapResoure[RESOURCE_END];

public:
	HRESULT	AddBuffer(LPDIRECT3DDEVICE9 pDevice, 
		RESOURCETYPE eResourceType, 
		CVIBuffer::BUFFERTYPE eBufferType, 
		const wstring& wstrResourceKey, 
		const WORD& wCntX = 0, 
		const WORD& wCntZ = 0, 
		const WORD& wItv = 1);

	HRESULT	AddObjBuffer(LPDIRECT3DDEVICE9 pDevice, 
		RESOURCETYPE eResourceType, 
		CVIBuffer::BUFFERTYPE eBufferType, 
		const wstring& wstrResourceKey,
		const float& wItvX = 1,
		const float& wItvY = 1,
		const float& wItvZ = 1,
		const DWORD& dwColor = D3DCOLOR_ARGB(255, 255, 255, 255));

	HRESULT	AddTexture(LPDIRECT3DDEVICE9 pDevice, 
		RESOURCETYPE eResourceType, 
		TEXTURETYPE eTextureType, 
		const wstring& wstrResourcesKey, 
		const wstring& wstrFilePath, 
		const WORD& wCnt);

	void	SetTerrainBuffer(LPDIRECT3DDEVICE9 pDevice, 
		RESOURCETYPE eResourceType, 
		CVIBuffer::BUFFERTYPE eBufferType, 
		const wstring& wstrResourceKey,
		int iCntX, 
		int iCntZ, 
		int iItv);

	void	Render(const wstring& wstrResourceKey);

	CComponent* CloneResource(RESOURCETYPE eResourveType, 
								const wstring& wstrResourceKey);

public:
	void		ResetDynamic(void);

	void		SetVtxInfo(RESOURCETYPE eResourceType, 
		const wstring& wstrResourceKey, 
		void* pVertex);

	void		GetVtxInfo(RESOURCETYPE eResourceType, 
		const wstring& wstrResourceKey, 
		void* pVertex);

private:
	void	Release(void);


};

END

#endif // ResourcesMgr_h__

