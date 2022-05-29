/*!
 * \file Stage_2.h
 * \date 2016/08/11 10:29
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
#ifndef Stage_2_h__
#define Stage_2_h__

#include "ClientScene.h"

namespace ENGINE
{
	class CComponent;
	class CGameObject;
}
class CStage_2 : public CClientScene
{
private:
	explicit	CStage_2(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual		~CStage_2(void);
private:
	virtual HRESULT	Initialize(void);
private:
	POINT			m_pMouse;
	float			m_fTime;

	bool			m_bBoss_2_Appear;
	ENGINE::CGameObject* m_bBoss2;

private:
	ENGINE::CGameObject*	m_pRing;
	ENGINE::CGameObject*	m_pUnit;

	float					m_fDelayTime;
	bool					m_bRingThrow;
	bool					m_bRing2;
	bool					m_bReflectOn;

	bool					m_bGengi;
	bool					m_BossAttSound;
	bool					Stage2Att;
	bool					Stage2Crowl;
	bool					m_bDestroy;
	float					m_fGravityTime;
	float					m_fGravity;

public:
	virtual void	Update(void);
	virtual void	Render(void);
private:
	virtual void	Release(void);

public:
	virtual HRESULT	AddBuffer(void);
	virtual HRESULT	AddTexture(void);
	virtual HRESULT	AddObject(void);

public:
	void PlayerDelete();

public:
	void	CreateTerrain(void);
	void	SetCameraToPlayer(void);
public:
	virtual ENGINE::TERRAIN_INFO*	LoadTerrain(void);

public:
	static	CStage_2*	Create(LPDIRECT3DDEVICE9 pDevice);

};

#endif // Stage_2_h__