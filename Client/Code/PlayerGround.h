#ifndef PlayerGround_h__
#define PlayerGround_h__

#include "RcObject.h"

class CPlayerGround : public CRcObject
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
	static	CPlayerGround*		Create(LPDIRECT3DDEVICE9 pDevice);
	void				Update(void);
	void				Render(void);
	void				Release(void);
	void				SetPos(float fX, float fY, float fZ);

private:
	explicit CPlayerGround(LPDIRECT3DDEVICE9 pDevice);
public:
	~CPlayerGround(void);
};


#endif // PlayerGround_h__
