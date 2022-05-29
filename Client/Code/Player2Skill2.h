#ifndef Player2Skill2_h__
#define Player2Skill2_h__

#include "RcObject.h"


class CPlayer2_Skill2 : public CRcObject
{

private:
	D3DXVECTOR3				m_vPos;
	ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;
	DWORD					m_dwVtxCnt;

	bool					m_bClick;
	
	float					m_fX;
	float					m_fY;
public:
	D3DXVECTOR3					fMetheoPos;
	D3DXVECTOR3					fMetheoDir;
private:
	HRESULT				Initialize(void);
	HRESULT				AddComponent(void);

public:
	static	CPlayer2_Skill2*		Create(LPDIRECT3DDEVICE9 pDevice, float fX, float fY);
	void					Update(void);
	void					Render(void);
	void					Release(void);
	void					SetPos(float fX, float fY, float fZ);

	bool&				GetClick(void)
	{
		return m_bClick;
	}

private:
	explicit CPlayer2_Skill2(LPDIRECT3DDEVICE9 pDevice, float fX, float fY);
public:
	~CPlayer2_Skill2(void);

};

#endif // Player2Skill2_h__
