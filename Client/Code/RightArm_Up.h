/*!
 * \file RightArm_Up.h
 * \date 2016/08/20 14:28
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
#ifndef RightArm_Up_h__
#define RightArm_Up_h__

#include "CubeObject.h"

class CRightArm_Up : public CCubeObject
{
public:
	enum UNITTYPE{ UT_PLAYER_ESILDUR, UT_PLAYER_ELOND, UT_ELF_SOLDIER, UT_ORC, UT_BOSS };

private:
	explicit	CRightArm_Up(LPDIRECT3DDEVICE9 pDevice);
	explicit	CRightArm_Up(LPDIRECT3DDEVICE9 pDevice, UNITTYPE m_eUnitType);
public:
	virtual		~CRightArm_Up(void);

private:
	virtual	HRESULT		Invalidate(void);
	HRESULT				AddComponent(void);

public:
	virtual	void		Update(void);
	virtual	void		Render(void);

private:
	void		Release(void);

public:
	static CRightArm_Up*	Create(LPDIRECT3DDEVICE9 pDevice);
	static CRightArm_Up*	Create(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType);

private:
	UNITTYPE		m_eUnitType;
};

#endif // RightArm_Up_h__