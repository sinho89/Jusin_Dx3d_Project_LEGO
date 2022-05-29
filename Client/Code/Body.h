/*!
 * \file Body.h
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
#ifndef Body_h__
#define Body_h__

#include "CubeObject.h"

class CBody : public CCubeObject
{
public:
	enum UNITTYPE{ UT_PLAYER_ESILDUR, UT_PLAYER_ELOND, UT_ELF_SOLDIER, UT_ORC, UT_BOSS };

private:
	explicit	CBody(LPDIRECT3DDEVICE9 pDevice);
	explicit	CBody(LPDIRECT3DDEVICE9 pDevice, UNITTYPE m_eUnitType);
public:
	virtual		~CBody(void);

private:
	virtual	HRESULT		Invalidate(void);
	HRESULT				AddComponent(void);

public:
	virtual	void		Update(void);
	virtual	void		Render(void);

private:
	void		Release(void);

public:
	static CBody*	Create(LPDIRECT3DDEVICE9 pDevice);
	static CBody*	Create(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType);

private:
	UNITTYPE		m_eUnitType;
};

#endif // Body_h__