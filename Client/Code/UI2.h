#ifndef UI2_h__
#define UI2_h__

#include "RcObject.h"

class CUI2 : public CRcObject
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
	static	CUI2*		Create(LPDIRECT3DDEVICE9 pDevice);
	void				Update(void);
	void				Render(void);
	void				Release(void);
	void				SetPos(float fX, float fY, float fZ);

private:
	explicit CUI2(LPDIRECT3DDEVICE9 pDevice);
public:
	~CUI2(void);
};
#endif // UI2_h__
