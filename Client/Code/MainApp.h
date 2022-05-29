/*!
 * \file MainApp.h
 * \date 2016/08/10 14:17
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
#ifndef MainApp_h__
#define MainApp_h__

#include "Engine_Include.h"

namespace ENGINE
{
	class CGraphicDev;
	class CManagement;
	class CCameraMgr;
	class CTimeMgr;
}

class CMainApp
{
private:
	CMainApp(void);

public:
	~CMainApp(void);

private:
	ENGINE::CGraphicDev*		m_pGraphicDev;
	ENGINE::CManagement*		m_pManagement;
	ENGINE::CTimeMgr*			m_pTimeMgr;
	ENGINE::CCameraMgr*			m_pCameraMgr;
	LPDIRECT3DDEVICE9			m_pDevice;

public:
	void	Update(void);
	void	Render(void);
private:
	HRESULT		Initialize(void);
	void		Release(void);

public:
	static		CMainApp*		Create(void);
};

#endif // MainApp_h__