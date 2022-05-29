/*!
 * \file Scene.h
 * \date 2016/07/13 10:35
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

#ifndef Scene_h__
#define Scene_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CLayer;
class ENGINE_DLL CScene
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9	pDevice);
public:
	virtual ~CScene(void);

public:
	virtual HRESULT		Initialize(void);
	virtual void		Update(void);
	virtual void		Render(void);

public:
	virtual HRESULT	AddBuffer(void);
	virtual HRESULT	AddTexture(void);
	virtual HRESULT	AddObject(void);

public:
	virtual ENGINE::TERRAIN_INFO*	LoadTerrain(void)PURE;

private:
	virtual	void		Release(void);
	

protected:
	LPDIRECT3DDEVICE9				m_pDevice;
};

END
#endif // Scene_h__
