/*!
 * \file MouseCol.h
 * \date 2016/08/03 10:15
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

#ifndef MouseCol_h__
#define MouseCol_h__

#include "Collision.h"

namespace	ENGINE
{
	class CInfoSubject;
}

class CCameraObserver;
class CMouseCol : public ENGINE::CCollision
{
private:
	CMouseCol(void);
public:
	virtual ~CMouseCol(void);

public:
	static POINT GetMousePos(void);
	static CMouseCol* Create(void);
	virtual ENGINE::CCollision* Clone(void);

public:
	void PickTerrain(D3DXVECTOR3* pOut,
					const ENGINE::VTXTEX* pTerrainVtx);

	void PickObject(D3DXVECTOR3* pOut, 
					const ENGINE::VTXTEX* pVertex,
					const D3DXMATRIX* pMatWorld);

private:
	HRESULT	Initialize(void);
	void	Translation_ViewSpace(void);
	void	Translation_Local(const D3DXMATRIX* pWorld);
	void	Release(void);

private:
	ENGINE::CInfoSubject*			m_pInfoSubject;

private:
	CCameraObserver*				m_pCameraObserver;
	D3DXVECTOR3						m_vPivotPos;
	D3DXVECTOR3						m_vRayDir;

};

#endif // MouseCol_h__