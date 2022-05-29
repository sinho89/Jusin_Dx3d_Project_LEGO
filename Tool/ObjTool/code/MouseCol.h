/*!
 * \file MouseCol.h
 * \date 2016/08/13 2:37
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

namespace ENGINE
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
	float rDist;
public:
	static POINT				GetMousePos(void);
	static CMouseCol*			Create(void);
	virtual ENGINE::CCollision* Clone(void);

public:
	float&	GetDist(void)
	{
		return rDist;		//큐브의 면과 레이와 화면과의 거리.
	}

public:
	void	PickObject(CPoint _pos,//D3DXVECTOR3* pOut,
						ENGINE::VTXCUBE* pVertex,
						const D3DXMATRIX*	pMatWorld,
						ENGINE::PICKINGID&	ePickID,
						float&				_fDist,
						D3DXVECTOR3&		_vecItv);

private:
	HRESULT	Initialize(void);
	void	Translation_ViewSpae(CPoint _pos);
	void	Translation_Local(const D3DXMATRIX*	pWorld);
	void	Release(void);

private:
	ENGINE::CInfoSubject*		m_pInfoSubject;

private:
	CCameraObserver*			m_pCamObserver;
	D3DXVECTOR3					m_vPivotPos;
	D3DXVECTOR3					m_vRayDir;
};

#endif // MouseCol_h__