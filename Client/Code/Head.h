/*!
 * \file Head.h
 * \date 2016/08/20 14:22
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
#ifndef Head_h__
#define Head_h__

#include "CubeObject.h"

class CHead : public CCubeObject
{
public:
	enum UNITTYPE{ UT_PLAYER_ESILDUR, UT_PLAYER_ELOND, UT_ELF_SOLDIER, UT_ORC, UT_BOSS };
private:
	explicit	CHead(LPDIRECT3DDEVICE9 pDevice);
	explicit	CHead(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType);
public:
	virtual		~CHead(void);

private:
	virtual	HRESULT		Initialize(void);
	HRESULT				AddComponent(void);

public:
	virtual	void		Update(void);
	virtual	void		Render(void);

private:
	void		Release(void);

public:
	static CHead*	Create(LPDIRECT3DDEVICE9 pDevice);
	static CHead*	Create(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType);

private:
	UNITTYPE		m_eUnitType;
};

#endif // Head_h__