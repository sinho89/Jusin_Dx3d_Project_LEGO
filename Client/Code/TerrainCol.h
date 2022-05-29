/*!
 * \file TerrainCol.h
 * \date 2016/08/23 8:44
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
#ifndef TerrainCol_h__
#define TerrainCol_h__

#include "Collision.h"

namespace ENGINE
{
	class CTimeMgr;
}

class CTerrain;

class CTerrainCol : public ENGINE::CCollision
{
private:
	CTerrainCol(void);
public:
	virtual ~CTerrainCol(void);

public:
	virtual void				Update(void);
	virtual ENGINE::CCollision*	Clone(void);

public:
	static	CTerrainCol*		Create(void);
	void						SetColInfo(D3DXVECTOR3* pPos, 
										const ENGINE::VTXTEX* pTerrainVtx,
										CTerrain* pTerrain);
	void						Release(void);

	

public:
	bool						m_bCollision;
	bool						m_JumpAble;

public:
	bool&						GetCollision(void);
	bool&						GetJumpAble(void);

private:
	D3DXVECTOR3*				m_pPos;
	CTerrain*					m_pTerrain;
	const ENGINE::VTXTEX*		m_pTerrainVtx;
	//float						m_Gravity;
	//bool						m_bJumpAble;
	//ENGINE::CTimeMgr*			m_pTimeMgr;
};


#endif // TerrainCol_h__