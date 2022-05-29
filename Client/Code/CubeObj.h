#ifndef CubeObj_h__
#define CubeObj_h__

#include "GameObject.h"

namespace ENGINE
{
	class CTimeMgr;
	class CResourcesMgr;
	class CManagement;
	class CTexture;
	class CVIBuffer;
	class CTransform;
	class CInfoSubject;
}

class CCameraObserver;
class CCollisionMgr;

class CCubeObj : public ENGINE::CGameObject
{
private:
	ENGINE::CResourcesMgr*	m_pResourcesMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CManagement*	m_pManagement;

	ENGINE::CVIBuffer*		m_pBuffer;
	ENGINE::CTexture*		m_pTexture;
	ENGINE::CTransform*		m_pInfo;

	ENGINE::CInfoSubject*	m_pInfoSubject;
	CCameraObserver*		m_pCameraObserver;
	CCollisionMgr*			m_pCollisionMgr;

private:
	float					m_fTime;
	float					m_fSpeed;

private:
	
	ENGINE::VTXCUBE*		m_pVertex;
	ENGINE::VTXCUBE*		m_pConvertVertex;

	DWORD					m_dwVtxCnt;


public:
	HRESULT				Initialize(void);
	void				SetDirection(void);
	HRESULT				AddComponent(void);

public:
	static	CCubeObj*	Create(LPDIRECT3DDEVICE9 pDevice);
	void				Update(void);
	void				Render(void);
	void				Release(void);
	void				SetPos(const D3DXVECTOR3& vPos);
	void				SetTransform(void);

private:
	explicit CCubeObj(LPDIRECT3DDEVICE9		pDevice);
public:
	~CCubeObj();


};



#endif // CubeObj_h__
