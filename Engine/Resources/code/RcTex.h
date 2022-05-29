/*!
 * \file RcTex.h
 * \date 2016/07/19 12:05
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

#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CRcTex : public CVIBuffer
{
private:
	virtual HRESULT	CreateBuffer(void);

public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pDevice);
	void	Release(void);
	
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CRcTex();

};

END
#endif // RcTex_h__