/*!
 * \file LeftArm_Down.h
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
#ifndef LeftArm_Down_h__
#define LeftArm_Down_h__

#include "CubeObject.h"

class CLeftArm_Down : public CCubeObject
{
public:
	enum UNITTYPE{ UT_PLAYER_ESILDUR, UT_PLAYER_ELOND, UT_ELF_SOLDIER, UT_ORC, UT_BOSS };

private:
	explicit	CLeftArm_Down(LPDIRECT3DDEVICE9 pDevice);
	explicit	CLeftArm_Down(LPDIRECT3DDEVICE9 pDevice, UNITTYPE m_eUnitType);
public:
	virtual		~CLeftArm_Down(void);

private:
	virtual	HRESULT		Invalidate(void);
	HRESULT				AddComponent(void);

public:
	virtual	void		Update(void);
	virtual	void		Render(void);

private:
	void		Release(void);

public:
	static CLeftArm_Down*	Create(LPDIRECT3DDEVICE9 pDevice);
	static CLeftArm_Down*	Create(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType);

private:
	UNITTYPE		m_eUnitType;
};

#endif // LeftArm_Down_h__