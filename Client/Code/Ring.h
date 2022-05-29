/*!
 * \file Ring.h
 * \date 2016/08/31 5:23
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
#ifndef Ring_h__
#define Ring_h__

#include "Unit.h"

class CRing : public CUnit
{
private:
	explicit CRing(LPDIRECT3DDEVICE9 pDevice);
public:
	~CRing(void);

private:
	HRESULT			Initialize(void);
	HRESULT			AddComponent(void);

public:
	void			Update(void);
	void			Render(void);
	void			Release(void);

private:
	ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;
	DWORD					m_dwVtxCnt;

	float					m_fRotZ;

public:
	static CRing*	Create(LPDIRECT3DDEVICE9 pDevice);
};

#endif // Ring_h__
