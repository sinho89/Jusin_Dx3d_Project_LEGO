/*!
 * \file Player.h
 * \date 2016/07/18 11:44
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

#ifndef Player_h__
#define Player_h__

#include "GameObject.h"

namespace ENGINE
{
	class CTimeMgr;
	class CResourcesMgr;

	class CTexture;
	class CVIBuffer;
	class CTransform;
	class CInfoSubject;	
}

class CCameraObserver;
class CCamTarget : public ENGINE::CGameObject
{
private:
	ENGINE::CResourcesMgr*	m_pResourcesMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::CVIBuffer*		m_pBuffer;
	ENGINE::CTexture*		m_pTexture;

	ENGINE::CInfoSubject*	m_pInfoSubject;
	CCameraObserver*		m_pCameraObserver;

private:
	ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;
	DWORD					m_dwVtxCnt;

private:
	float					m_fTime;
	float					m_fAngle;
	float					m_fSpeed;

	float					m_fDistance;
	float					m_fCameraAngle;

private:
	HRESULT				Initialize(void);
	void				KeyInput(void);
	void				SetDirection(void);
	HRESULT				AddComponent(void);
	void				StaticCamera(void);
	void				SetTransform(void);
	
public:
	static	CCamTarget*	Create(LPDIRECT3DDEVICE9 pDevice);
	void				Update(void);
	void				Render(void);
	void				Release(void);
	ENGINE::CTransform*		m_pInfo;

private:
	explicit CCamTarget(LPDIRECT3DDEVICE9		pDevice);
public:
	~CCamTarget();
};

#endif // Player_h__