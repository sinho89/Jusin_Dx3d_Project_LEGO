/*!
 * \file Aim.h
 * \date 2016/08/31 5:35
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

#ifndef Aim_h__
#define Aim_h__

#include "RcObject.h"

class CAim : public CRcObject
{

private:
	D3DXVECTOR3				m_vPos;
	ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;
	DWORD					m_dwVtxCnt;
private:
	HRESULT				Initialize(void);
	HRESULT				AddComponent(void);

public:
	static	CAim*		Create(LPDIRECT3DDEVICE9 pDevice);
	void				Update(void);
	void				Render(void);
	void				Release(void);
	void				SetPos(float fX, float fY, float fZ);

private:
	explicit CAim(LPDIRECT3DDEVICE9 pDevice);
public:
	~CAim(void);
};

#endif // Aim_h__