/*!
 * \file DivisionMgr.h
 * \date 2016/08/21 8:39
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

#ifndef DivisionMgr_h__
#define DivisionMgr_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CDivisionCamera;
class ENGINE_DLL CDivisionMgr
{
	DECLARE_SINGLETON(CDivisionMgr)
public:
	CDivisionCamera*	m_pCam1P;
	CDivisionCamera*	m_pCam2P;
public:
	INT		Create(LPDIRECT3DDEVICE9);
	INT		FrameMove();

private:
	explicit CDivisionMgr(void);
public:
	~CDivisionMgr(void);
};

END

#endif // DivisionMgr_h__