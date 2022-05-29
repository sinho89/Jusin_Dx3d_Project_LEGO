/*!
 * \file LayerMgr.h
 * \date 2016/08/10 19:22
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
#ifndef LayerMgr_h__
#define LayerMgr_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CComponent;
class CGameObject;
class ENGINE_DLL CLayerMgr
{
	DECLARE_SINGLETON(CLayerMgr)

private:
	typedef list<CGameObject*>		OBJLIST;
	typedef unordered_map<const TCHAR*, OBJLIST>	MAPOBJLIST;

	MAPOBJLIST						m_mapObjlist[LAYER_END];

public:
	enum DivisionType { DIV_SINGLE, DIV_MULTI, DIV_LOGO, DIV_END };

private:
	DivisionType	m_eDivType;
public:
	void			SetDivisionType(DivisionType _eType);
	DivisionType	GetDivisionType();
public:
	void	Update(void);
	void	Render(void);
	void	Release(void);

public:
	HRESULT			AddObject(const TCHAR* szObjKey, 
							CGameObject* pGameObject, 
							LAYERID LayerType);

	CComponent*		GetComponent(const TCHAR*	szObjKey,
								const TCHAR*	szComponentKey,
								LAYERID LayerType);

	CGameObject*	GetGameObject(const TCHAR* szObjKey,
								LAYERID LayerType);

	OBJLIST*		GetObjList(const TCHAR* szObjKey,
								LAYERID LayerType);


private:
	explicit CLayerMgr(void);
public:
	~CLayerMgr(void);
};

END

#endif // LayerMgr_h__