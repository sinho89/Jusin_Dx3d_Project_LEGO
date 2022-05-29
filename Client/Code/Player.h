#ifndef Player_h__
#define Player_h__

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
class CTerrainCol;

class CPlayer : public ENGINE::CGameObject
{
public:
	enum BODYPART
	{
		BP_HEAD, 
		BP_BODY, 
		BP_R_ARM_UP, 
		BP_R_ARM_DOWN,
		BP_L_ARM_UP,
		BP_L_ARM_DOWN,
		BP_R_LEG,
		BP_L_LEG,
		BP_END
	};

private:
	ENGINE::CResourcesMgr*	m_pResourcesMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;
	ENGINE::CManagement*	m_pManagement;

	ENGINE::CVIBuffer*		m_pBuffer;
	ENGINE::CTexture*		m_pTexture;

	ENGINE::CTransform*		m_pInfo[BP_END];
	ENGINE::ITVXYZ*			m_pItvXYZ[BP_END];

	ENGINE::CInfoSubject*	m_pInfoSubject;

	CCameraObserver*		m_pCameraObserver;
	CCollisionMgr*			m_pCollisionMgr;

	D3DXVECTOR3				m_vDestPos;
	bool					m_bMove;
	const ENGINE::VTXTEX*	m_pTerrainVertex;

private:
	ENGINE::VTXCUBE*		m_pVertex;
	ENGINE::VTXCUBE*		m_pConvertVertex;
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
	void				SetTransform(void);
	void				Move(void);

public:
	static	CPlayer*	Create(LPDIRECT3DDEVICE9 pDevice);
	void				Update(void);
	void				Render(void);
	void				Release(void);

private:
	explicit CPlayer(LPDIRECT3DDEVICE9		pDevice);
public:
	~CPlayer();
};





#endif // Player_h__