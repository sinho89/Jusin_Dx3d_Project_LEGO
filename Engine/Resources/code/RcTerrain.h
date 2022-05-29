/*!
 * \file RcTerrain.h
 * \date 2016/07/19 10:37
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

#ifndef RcTerrain_h__
#define RcTerrain_h__

#include "VIBuffer.h"

BEGIN(ENGINE)

class ENGINE_DLL CRcTerrain : public CVIBuffer
{
public:
	virtual HRESULT CreateBuffer(const WORD& wCntX, const WORD& wCntZ, const WORD& wItv);
	virtual void Release(void);
	virtual void Render(void);

public:
	static CRcTerrain* Create(LPDIRECT3DDEVICE9	pDevice, 
								const WORD& wCntX, 
								const WORD& wCntZ, 
								const WORD& wItv);

private:
	explicit	CRcTerrain(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CRcTerrain(void);

};

END
#endif // RcTerrain_h__