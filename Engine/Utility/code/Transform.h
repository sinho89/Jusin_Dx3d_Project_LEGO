/*!
 * \file Transform.h
 * \date 2016/07/25 10:46
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

#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(ENGINE)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform(const D3DXVECTOR3& vLook);

public:
	virtual ~CTransform(void);

public:
	virtual void Update(void);
	static CTransform* Create(const D3DXVECTOR3& vLook);

public:
	float				m_fAngle[ANGLE_END];
	D3DXVECTOR3			m_vPos;
	D3DXVECTOR3			m_vDir;
	D3DXMATRIX			m_matWorld;
	D3DXMATRIX			m_matView;
	D3DXMATRIX			m_matProj;
	int					m_iHp;

};

END
#endif // Transform_h__