/*!
 * \file CollisionMgr.h
 * \date 2016/07/29 11:59
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
	enum COLLISIONID { COL_TERRAIN, COL_MOUSE };

public:
	HRESULT	AddColObject(COLLISIONID eCollisionID);
	ENGINE::CCollision*	CloneColObject(COLLISIONID eCollisionID);
private:
	void Release(void);

private:
	typedef	map<COLLISIONID, ENGINE::CCollision*>	MAPCOLLISION;
	MAPCOLLISION		m_mapCollision;

private:
	CCollisionMgr(void);
	~CCollisionMgr(void);

};

#endif // CollisionMgr_h__