/*!
 * \file Resouces.h
 * \date 2016/07/15 11:33
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

#ifndef Resources_h__
#define Resources_h__

#include "Component.h"

BEGIN(ENGINE)

class ENGINE_DLL CResources : public CComponent
{
protected:
	WORD*			m_pwRefCnt;

protected:
	explicit CResources(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CResources(void);

public:
	virtual void Render(void)PURE;
	virtual CResources* CloneResource(void) PURE;

	void Release(void);

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

};

END

#endif // Resources_h__

