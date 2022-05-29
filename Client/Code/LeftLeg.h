/*!
 * \file LeftLeg.h
 * \date 2016/08/20 14:33
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
#ifndef LeftLeg_h__
#define LeftLeg_h__

#include "CubeObject.h"

class CLeftLeg : public CCubeObject
{
public:
	enum UNITTYPE{ UT_PLAYER_ESILDUR, UT_PLAYER_ELOND, UT_ELF_SOLDIER, UT_ORC, UT_BOSS };

private:
	explicit	CLeftLeg(LPDIRECT3DDEVICE9 pDevice);
	explicit	CLeftLeg(LPDIRECT3DDEVICE9 pDevice, UNITTYPE m_eUnitType);
public:
	virtual		~CLeftLeg(void);

private:
	virtual	HRESULT		Invalidate(void);
	HRESULT				AddComponent(void);

public:
	virtual	void		Update(void);
	virtual	void		Render(void);

private:
	void		Release(void);

public:
	static CLeftLeg*	Create(LPDIRECT3DDEVICE9 pDevice);
	static CLeftLeg*	Create(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType);

private:
	UNITTYPE		m_eUnitType;
};

#endif // LeftLeg_h__