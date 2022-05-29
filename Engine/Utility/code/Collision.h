/*!
 * \file Collision.h
 * \date 2016/07/29 11:54
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

#ifndef Collision_h__
#define Collision_h__

#include "Component.h"

BEGIN(ENGINE)

class ENGINE_DLL CCollision : public CComponent
{
protected:
	CCollision(void);

public:
	virtual ~CCollision(void);

public:
	virtual CCollision* Clone(void)	PURE;

protected:
	void Release(void);

protected:
	WORD*		m_pwRefCnt;

};

END
#endif // Collision_h__