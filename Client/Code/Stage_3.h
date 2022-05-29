/*!
 * \file Stage_3.h
 * \date 2016/08/11 10:31
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
#ifndef Stage_3_h__
#define Stage_3_h__

#include "ClientScene.h"

class CStage_3 : public CClientScene
{

private:
	explicit	CStage_3(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual		~CStage_3(void);

private:
	virtual HRESULT	Initialize(void);

public:
	virtual void	Update(void);
	virtual void	Render(void);

private:
	virtual void	Release(void);

public:
	virtual HRESULT	AddBuffer(void);
	virtual HRESULT	AddTexture(void);
	virtual HRESULT	AddObject(void);

public:
	virtual ENGINE::TERRAIN_INFO*	LoadTerrain(void);

public:
	static	CStage_3*	Create(LPDIRECT3DDEVICE9 pDevice);

};

#endif // Stage_3_h__