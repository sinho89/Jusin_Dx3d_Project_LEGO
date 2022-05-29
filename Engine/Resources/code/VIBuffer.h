/*!
 * \file VIBuffer.h
 * \date 2016/07/14 10:42
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

#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(ENGINE)

class ENGINE_DLL CVIBuffer : public CResources
{
public:
	enum BUFFERTYPE { 
		BUFFER_TERRAIN, 
		BUFFER_RCTEX,
		BUFFER_CUBETEX, 
		BUFFER_CUBECOL,
		BUFFER_CUBETEX_CUSTOM,
		BUFFER_CUBECOL_CUSTOM};

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

	DWORD						m_dwVtxSize;
	DWORD						m_dwVtxCnt;
	DWORD						m_dwVtxFVF;
	DWORD						m_dwTriCnt;

	DWORD						m_dwIdxSize;
	D3DFORMAT					m_IdxFmt;

	ITVXYZ						m_tItv;

public:
	DWORD*	GetVtxSize();
	DWORD*	GetVtxCnt();
	DWORD*	GetVtxFVF();
	DWORD*	GetTriCnt();
public:
	virtual HRESULT	CreateBuffer(void);
	virtual CResources* CloneResource(void);

public:
	void			SetVtxInfo(void* pVertex);
	void			GetVtxInfo(void* pVertex);
	const ITVXYZ*	GetItv(void);


public:
	virtual void	Render(void);
	void	Release(void);

protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9	pDevice);
public:
	virtual ~CVIBuffer(void);

};

END
#endif // VI1Buffer_h__