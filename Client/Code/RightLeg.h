/*!
 * \file RightLeg.h
 * \date 2016/08/20 14:32
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
#ifndef RightLeg_h__
#define RightLeg_h__

#include "CubeObject.h"

class CRightLeg : public CCubeObject
{
public:
	enum UNITTYPE{ UT_PLAYER_ESILDUR, UT_PLAYER_ELOND, UT_ELF_SOLDIER, UT_ORC, UT_BOSS };

private:
	explicit	CRightLeg(LPDIRECT3DDEVICE9 pDevice);
	explicit	CRightLeg(LPDIRECT3DDEVICE9 pDevice, UNITTYPE m_eUnitType);
public:
	virtual		~CRightLeg(void);

private:
	virtual	HRESULT		Invalidate(void);
	HRESULT				AddComponent(void);

public:
	virtual	void		Update(void);
	virtual	void		Render(void);

private:
	void		Release(void);

public:
	static CRightLeg*	Create(LPDIRECT3DDEVICE9 pDevice);
	static CRightLeg*	Create(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType);

private:
	UNITTYPE		m_eUnitType;
};

#endif // RightLeg_h__