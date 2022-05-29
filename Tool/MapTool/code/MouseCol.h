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
	static CMouseCol* Create(void);
	virtual ENGINE::CCollision* Clone(void);

public:
	void PickTerrain(CPoint	_Pos,
					D3DXVECTOR3* pOut, 
					ENGINE::VTXTEX* pTerrainVtx,
					int _VtxCntX,
					int _VtxCntZ,
					int	_VtxItv,
					float _VtxHeight);

	void PickObject(D3DXVECTOR3* pOut, 
					const ENGINE::VTXTEX* pVertex,
					const D3DXMATRIX* pMatWorld);

private:
	HRESULT	Initialize(void);
	void	Translation_ViewSpace(CPoint _Pos);
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