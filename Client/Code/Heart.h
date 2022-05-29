#ifndef Heart_h__
#define Heart_h__

#include "RcObject.h"

class CHeart : public CRcObject
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
	static	CHeart*		Create(LPDIRECT3DDEVICE9 pDevice, float fX, float fY);
	void				Update(void);
	void				Render(void);
	void				Release(void);
	void				SetPos(float fX, float fY, float fZ);

private:
	explicit CHeart(LPDIRECT3DDEVICE9 pDevice, float fX, float fY);
public:
	~CHeart(void);

};



#endif // Heart_h__
