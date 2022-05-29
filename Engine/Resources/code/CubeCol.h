/*!
 * \file CubeCol.h
 * \date 2016/08/12 19:48
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
#ifndef CubeCol_h__
#define CubeCol_h__

#include "VIBuffer.h"
#include "Engine_struct.h"

BEGIN(ENGINE)

class ENGINE_DLL CCubeCol : public CVIBuffer
{
private:
	ITVXYZ	m_tItv;

private:
	explicit	CCubeCol(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CCubeCol(void);

public:
	HRESULT	CreateBuffer(void);
	HRESULT	CreateBuffer(const float& wItvX,
						const float& wItvY,
						const float& wItvZ,
						const DWORD&	dwColor);

public:
	virtual void Render(void);

public:
	static CCubeCol* Create(LPDIRECT3DDEVICE9 pDevice);
	static CCubeCol* Create(LPDIRECT3DDEVICE9 pDevice, 
							const float& wItvX,
							const float& wItvY,
							const float& wItvZ,
							const DWORD& dwColor);
protected:
	void Release(void);
};

END
#endif // CubeCol_h__