/*!
 * \file Stage_1.cpp
 * \date 2016/08/11 9:56
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
#ifndef Stage_1_h__
#define Stage_1_h__

#include "ClientScene.h"

namespace ENGINE
{
	class CGameObject;
	class CSoundMgr;
}


class CStage_1 : public CClientScene
{

private:
	explicit	CStage_1(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual		~CStage_1(void);

private:
	float					m_fTime;
	WORD					m_wFrameCnt;
	bool					m_bIsildurSound;
	bool					m_bCrowl;
	float					fCrowTime;
	bool					m_bFieldFire;
	float					m_fFieldFire;

	ENGINE::CGameObject*	m_pIsildur;
	ENGINE::CGameObject*	m_pElond;

private:
	ENGINE::CGameObject*			m_pBoss;
	bool							m_pBossAppear;

private:
	virtual HRESULT	Initialize(void);

public:
	virtual void	Update(void);
	virtual void	Render(void);

public:
	virtual HRESULT	AddBuffer(void);
	virtual HRESULT	AddTexture(void);
	virtual HRESULT	AddObject(void);

	void SetCameraToPlayer();
public:
	void	CreateTerrain(void);

private:
	virtual void	Release(void);

public:
	virtual ENGINE::TERRAIN_INFO*	LoadTerrain(void);

public:
	static	CStage_1*	Create(LPDIRECT3DDEVICE9 pDevice);
};


#endif // Stage_1_h__