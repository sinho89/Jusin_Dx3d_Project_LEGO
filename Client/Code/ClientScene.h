/*!
 * \file ClientScene.h
 * \date 2016/08/20 15:48
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

#ifndef ClientScene_h__
#define ClientScene_h__

#include "Scene.h"

namespace ENGINE
{
	class CManagement;
	class CResourcesMgr;
	class CLayerMgr;
	class CCameraMgr;
	class CTimeMgr;
	class CSoundMgr;
}

class CClientScene : public ENGINE::CScene
{
protected:
	ENGINE::CManagement*	m_pManagement;
	ENGINE::CResourcesMgr*	m_pResourcesMgr;
	ENGINE::CLayerMgr*		m_pLayerMgr;
	ENGINE::CCameraMgr*		m_pCameraMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CSoundMgr*		m_pSoundMgr;
	

	float					m_fTime;
	float					m_fOriTime;
	float					m_fCount;

protected:
	explicit	CClientScene(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual		~CClientScene(void);

private:
	virtual	HRESULT	Initialize(void);

public:
	virtual void	Update(void);
	virtual	void	Render(void);

public:
	virtual ENGINE::TERRAIN_INFO*	LoadTerrain(void);
public:
	virtual HRESULT	AddBuffer(void);
	virtual HRESULT	AddTexture(void);
	virtual HRESULT	AddObject(void);

private:
	virtual void	Release(void);
};

#endif // ClientScene_h__