#ifndef Destroy_h__
#define Destroy_h__

#include "RcObject.h"

class CDestroy : public CRcObject
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
	static	CDestroy*		Create(LPDIRECT3DDEVICE9 pDevice, float fX, float fY);
	void				Update(void);
	void				Render(void);
	void				Release(void);
	void				SetPos(float fX, float fY, float fZ);

private:
	explicit CDestroy(LPDIRECT3DDEVICE9 pDevice, float fX, float fY);
public:
	~CDestroy(void);

};
#endif // Destroy_h__
