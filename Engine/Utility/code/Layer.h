/*!
 * \file Layer.h
 * \date 2016/07/21 10:34
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

#ifndef Layer_h__
#define Layer_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CComponent;
class CGameObject;
class ENGINE_DLL CLayer
{
protected:
	typedef list<CGameObject*>		OBJLIST;
	typedef map<wstring, OBJLIST>	MAPOBJLIST;
	MAPOBJLIST						m_mapObjList;
	
private:
	LPDIRECT3DDEVICE9			m_pDevice;

public:
	const CComponent*	GetComponent(const wstring& wstrObjKey, 
									const wstring& wstrComponentKey);

public:
	void		Update(void);
	void		Render(void);
private:
	void		Release(void);
public:
	static CLayer*	Create(LPDIRECT3DDEVICE9 pDevice);

	HRESULT		AddObject(const wstring& wstrObjkey, CGameObject* pGameObject);

	const ENGINE::VTXTEX* GetTerrainVtx(const wstring& wstrObjKey);

private:
	explicit CLayer(LPDIRECT3DDEVICE9 pDevice);

public:
	~CLayer();
};


END
#endif // Layer_h__