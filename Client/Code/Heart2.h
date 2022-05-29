#ifndef Heart2_h__
#define Heart2_h__

#include "RcObject.h"

class CHeart2 : public CRcObject
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
	static	CHeart2*		Create(LPDIRECT3DDEVICE9 pDevice, float fX, float fY);
	void				Update(void);
	void				Render(void);
	void				Release(void);
	void				SetPos(float fX, float fY, float fZ);

private:
	explicit CHeart2(LPDIRECT3DDEVICE9 pDevice, float fX, float fY);
public:
	~CHeart2(void);

};
#endif // Heart2_h__
