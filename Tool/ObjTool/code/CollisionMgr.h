/*!
 * \file CollisionMgr.h
 * \date 2016/08/12 23:04
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
#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Include.h"

namespace ENGINE
{
	class CCollision;
}

class CCollisionMgr
{
public:
	DECLARE_SINGLETON(CCollisionMgr)

public:
	enum COLLISIONID{ COL_CUBE, COL_MOUSE };

private:
	CCollisionMgr(void);
	~CCollisionMgr(void);

public:
	HRESULT	AddColObject(COLLISIONID eCollisionID);
	ENGINE::CCollision*	CloneColObject(COLLISIONID	eCollisionID);

private:
	void	Release(void);

private:
	typedef unordered_map<COLLISIONID, ENGINE::CCollision*>	MAPCOLLISION;
	MAPCOLLISION	m_mapCollision;
};

#endif // CollisionMgr_h__