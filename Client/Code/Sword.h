/*!
 * \file Sword.h
 * \date 2016/08/25 22:02
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
#ifndef Sword_h__
#define Sword_h__

#include "CubeObject.h"

class CSword : public CCubeObject
{
public:
	enum UNITTYPE{ UT_PLAYER, UT_ORC, UT_BOSS };

private:
	explicit	CSword(LPDIRECT3DDEVICE9 pDevice);
	explicit	CSword(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType);
public:
	virtual		~CSword(void);

private:
	virtual	HRESULT	Initialize(void);
	HRESULT			AddComponent(void);

public:
	virtual	void	Update(void);
	virtual	void	Render(void);

private:
	void			Release(void);

public:
	static	CSword*	Create(LPDIRECT3DDEVICE9 pDevice);
	static	CSword*	Create(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType);

public:
	D3DXVECTOR3&		GetSwordPos(int);
	float*				GetSwordAngle(int);

private:
	UNITTYPE			m_eUnitType;
	
	ENGINE::VTXCUBE*	m_pHandGuard_Vertex;
	ENGINE::VTXCUBE*	m_pHandGuard_ConverterVertex;
	ENGINE::VTXCUBE*	m_pSword_Vertex;
	ENGINE::VTXCUBE*	m_pSword_ConverterVertex;

	ENGINE::CVIBuffer*	m_pGuardBuffer;
	ENGINE::CVIBuffer*	m_pSwordBuffer;

	float				m_fItv[3][ENGINE::ITV_END];

	float				m_fAngle[3][ENGINE::ANGLE_END];

	D3DXVECTOR3			m_vSwordDir[3];
	D3DXVECTOR3			m_vSwordPos[3];
};

#endif // Sword_h__