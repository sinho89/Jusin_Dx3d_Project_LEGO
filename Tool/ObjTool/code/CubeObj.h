/*!
 * \file CubeObj.h
 * \date 2016/08/12 10:33
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

class CObjToolView;
class CCollisionMgr;
class CMouseCol;
class CCameraObserver;
class CCubeObj : public ENGINE::CGameObject
{
public:
	ENGINE::CResourcesMgr*		m_pResourcesMgr;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CManagement*		m_pManagement;
	ENGINE::CVIBuffer*			m_pBuffer;
	ENGINE::CTexture*			m_pTexture;
	ENGINE::CTransform*			m_pInfo;
	ENGINE::CInfoSubject*		m_pInfoSubject;

	ENGINE::PICKINGID			m_ePickID;

	ENGINE::SAVEOBJID			m_eCubeSaveType;
	
	CCameraObserver*			m_pCamObserver;
	CCollisionMgr*				m_pCollisionMgr;
	CMouseCol*					m_pMouseCol;
	CObjToolView*				m_pObjToolView;

	wstring						m_wstrResourceKey;

	float						m_fDist;
	bool						m_bMove;

public:
	D3DXVECTOR3					m_pOriginPos[8];

	ENGINE::VTXCUBE*			m_pVertex;
	ENGINE::VTXCUBE*			m_pConvertVertex;
	
	ENGINE::VTXCUBECOL*			m_pVertex_Col;
	ENGINE::VTXCUBECOL*			m_pConvertVertex_Col;
	DWORD						m_dwVtxCnt;

	float						m_fItv_X;
	float						m_fItv_Y;
	float						m_fItv_Z;


private:
	float					m_fTime;
	float					m_fAngle;
	float					m_fSpeed;

	float					m_fDistance;
	float					m_fCameraAngle;
	int						m_iCount;
	bool					m_bSetTex;

private:
	explicit CCubeObj(LPDIRECT3DDEVICE9 pDevice, const wstring& wstrResourceKey);
public:
	~CCubeObj(void);

private:
	HRESULT				Initialize(void);
	void				SetDirection(void);
	HRESULT				AddComponent(void);
	void				SetTransform(void);

public:
	static	CCubeObj*	Create(LPDIRECT3DDEVICE9 pDevice, 
								const wstring& wstrResourceKey);
	void				SetInfo(ENGINE::CComponent* _pCube);

	void				Update(void);
	void				Render(void);
	void				Release(void);
	void				SetPos(const D3DXVECTOR3& vPos);
	void				PickCube(CPoint _pos);
	void				SetTexture(int _iSel);
	void				Move(void);
};

#endif // CubeObj_h__