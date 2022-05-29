/*!
 * \file Terrain.h
 * \date 2016/07/31 14:53
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

#ifndef Terrain_h__
#define Terrain_h__

#include "GameObject.h"

namespace ENGINE
{
	class CResourcesMgr;
	class CTimeMgr;

	class CVIBuffer;
	class CTexture;
	class CTransform;
	class CInfoSubject;
}

class CMouseCol;
class CCollisionMgr;
class CCameraObserver;
class CTerrain : public ENGINE::CGameObject
{
private:
	ENGINE::CResourcesMgr*	m_pResourcesMgr;
	ENGINE::CTimeMgr*		m_pTimeMgr;

	ENGINE::CTexture*		m_pTexture;
	ENGINE::CVIBuffer*		m_pBuffer;
public:
	ENGINE::CTransform*		m_pInfo;
public:
	ENGINE::CInfoSubject*	m_pInfoSubject;
	CCameraObserver*		m_pCameraObserver;
	D3DXVECTOR3				m_vDestPos;
	CMouseCol*				m_pMouseCol;
	CCollisionMgr*			m_pCollisionMgr;

	ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;
	DWORD					m_dwVtxCnt;

	wstring					m_wstrResourceKey;

	int			m_VtxCntX;
	int			m_VtxCntZ;
	int			m_Itv;

private:
	HRESULT		Initialize(void);
	void		KeyInput(void);
	void		SetDirection(void);
	HRESULT		AddComponent(void);
	void		SetTransform(void);

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv);
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv, const wstring& wstrResourceKey);
	void			Update(void);
	void			Render(void);
	void			Release(void);
	void			SetVtx(int iCntX, int iCntZ, int iItv);
	bool			PickTerrain(CPoint	_Point, float _fHeight);
public:
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv);
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv, const wstring& wstrResourceKey);
public:
	~CTerrain();

};

#endif // Terrain_h__