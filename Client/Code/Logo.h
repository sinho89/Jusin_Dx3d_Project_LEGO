/*!
 * \file Logo.h
 * \date 2016/08/11 8:59
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 로고 화면
 *	
 * TODO: long description
 *
 * \note
*/
#ifndef Logo_h__
#define Logo_h__

#include "ClientScene.h"

class CLogo : public CClientScene
{

private:
	explicit	CLogo(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual		~CLogo(void);

private:
	virtual	HRESULT	Initialize(void);

public:
	virtual void	Update(void);
	virtual	void	Render(void);

public:
	virtual HRESULT	AddBuffer(void);
	virtual HRESULT	AddTexture(void);
	virtual HRESULT	AddObject(void);

public:
	virtual ENGINE::TERRAIN_INFO*	LoadTerrain(void);

private:
	virtual void	Release(void);


public:
	static	CLogo*	Create(LPDIRECT3DDEVICE9 pDevice);
};

#endif // Logo_h__