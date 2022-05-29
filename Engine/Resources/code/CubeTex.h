/*!
 * \file CubeTex.h
 * \date 2016/08/12 11:01
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
#ifndef CubeTex_h__
#define CubeTex_h__

#include "VIBuffer.h"
#include "Engine_struct.h"

BEGIN(ENGINE)

class ENGINE_DLL CCubeTex : public CVIBuffer
{
//private:
//	ITVXYZ	m_tItv;

public:
	HRESULT	CreateBuffer(void);
	HRESULT CreateBuffer(const float& wItvX,
							const float& wItvY,
							const float& wItvZ);
	static ENGINE::CCubeTex* Create(LPDIRECT3DDEVICE9 pDevice);
	static ENGINE::CCubeTex* Create(LPDIRECT3DDEVICE9 pDevice, 
									const float& wItvX,
									const float& wItvY,
									const float& wItvZ);

//public:
//	const ITVXYZ* GetItv(void);

private:
	void	Release(void);

public:
	virtual	void Render(void);

private:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CCubeTex(void);
};

END
#endif // CubeTex_h__