/*!
 * \file GameObject.h
 * \date 2016/07/21 10:55
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

#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Include.h"

BEGIN(ENGINE)


class CComponent;
class CTransform;
class ENGINE_DLL CGameObject
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CGameObject();

protected:
	CTransform*			m_pInfo;
public:
	virtual HRESULT		Initialize(void);
	virtual void		Update(void);
	virtual void		Render(void);
	virtual D3DXMATRIX*	GetWorldMatrix(void);
	virtual D3DXMATRIX*	GetViewMatrix(void);
	virtual D3DXMATRIX* GetProjMatrix(void);
	virtual LPDIRECT3DDEVICE9* GetDevice(void);

public:
	CComponent*	GetComponent(const wstring& wstrComponentKey);
	bool&		GetAlive(void);
	void		SetAlive(bool bAlive);

private:
	void	Release(void);

protected:
	LPDIRECT3DDEVICE9			m_pDevice;
	bool						m_bAlive;


protected:
	typedef	unordered_map<wstring, CComponent*>			MAPCOMPONENT;
	MAPCOMPONENT								m_mapComponent;

};
END
#endif // GameObject_h__