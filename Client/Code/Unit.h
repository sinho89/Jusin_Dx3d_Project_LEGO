/*!
 * \file Unit.h
 * \date 2016/08/22 10:19
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
#ifndef Unit_h__
#define Unit_h__

#include "ClientObj.h"
#include "SkillParticle.h"
class CUnit : public CClientObj
{
protected:
	explicit CUnit(LPDIRECT3DDEVICE9 pDevice);
public:
	~CUnit(void);

private:
	virtual HRESULT	Initialize(void);

public:
	virtual	void	Update(void);
	virtual	void	Render(void);
	
public:
	virtual HRESULT	AddComponent(void);
	void			SetDirection(void);
	int&			GetHp(void)
	{
		return m_iHp;
	}

private:
	void			Release(void);

public:
	static CUnit*	Create(LPDIRECT3DDEVICE9 pDevice);

protected:
	CClientObj*				m_pBodyPart[ENGINE::BP_END];
	ENGINE::UNITSTATEID		m_eUnitStateType;
	int						m_iHp;
	float					m_fSpeed;
	
	D3DXMATRIX				m_mMat[ENGINE::MAT_END];

private:
};

#endif // Unit_h__