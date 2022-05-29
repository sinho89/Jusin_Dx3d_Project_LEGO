/*!
 * \file LeftArm_Up.h
 * \date 2016/08/20 14:31
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
#ifndef LeftArm_Up_h__
#define LeftArm_Up_h__

#include "CubeObject.h"

class CLeftArm_Up : public CCubeObject
{
public:
	enum UNITTYPE{ UT_PLAYER_ESILDUR, UT_PLAYER_ELOND, UT_ELF_SOLDIER, UT_ORC, UT_BOSS };

private:
	explicit	CLeftArm_Up(LPDIRECT3DDEVICE9 pDevice);
	explicit	CLeftArm_Up(LPDIRECT3DDEVICE9 pDevice, UNITTYPE m_eUnitType);
public:
	virtual		~CLeftArm_Up(void);

private:
	virtual	HRESULT		Invalidate(void);
	HRESULT				AddComponent(void);

public:
	virtual	void		Update(void);
	virtual	void		Render(void);

private:
	void		Release(void);

public:
	static CLeftArm_Up*	Create(LPDIRECT3DDEVICE9 pDevice);
	static CLeftArm_Up*	Create(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType);

private:
	UNITTYPE		m_eUnitType;
};

#endif // LeftArm_Up_h__