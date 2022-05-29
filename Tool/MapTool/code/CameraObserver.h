/*!
 * \file CameraObserver.h
 * \date 2016/08/11 17:54
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
#ifndef CameraObserver_h__
#define CameraObserver_h__

#include "Observer.h"

namespace ENGINE
{
	class CInfoSubject;
}

class CCameraObserver : public ENGINE::CObserver
{
private:
	CCameraObserver(void);
public:
	virtual	~CCameraObserver(void);

public:
	const D3DXMATRIX*			GetView(void);
	const D3DXMATRIX*			GetProj(void);
	static	CCameraObserver*	Create(void);

public:
	virtual	void	Update(int iMessage);

private:
	ENGINE::CInfoSubject*	m_pInfoSubject;

	D3DXMATRIX				m_matView;
	D3DXMATRIX				m_matProj;
};

#endif // CameraObserver_h__