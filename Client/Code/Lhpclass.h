#ifndef Lhpclass_h__
#define Lhpclass_h__


#include "RcObject.h"

class CLhpclass : public CRcObject
{

private:
	D3DXVECTOR3				m_vPos;
	ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;
	DWORD					m_dwVtxCnt;

	float					m_fX;
	float					m_fY;
private:
	HRESULT				Initialize(void);
	HRESULT				AddComponent(void);

public:
	static	CLhpclass*		Create(LPDIRECT3DDEVICE9 pDevice, float fX, float fY);
	void				Update(void);
	void				Render(void);
	void				Release(void);
	void				SetPos(float fX, float fY, float fZ);

private:
	explicit CLhpclass(LPDIRECT3DDEVICE9 pDevice, float fX, float fY);
public:
	~CLhpclass(void);


};


#endif // Lhpclass_h__
