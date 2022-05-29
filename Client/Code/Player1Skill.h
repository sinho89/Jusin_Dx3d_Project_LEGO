#ifndef Player1Skill_h__
#define Player1Skill_h__


#include "RcObject.h"

class CPlayer1_Skill : public CRcObject
{

private:
	D3DXVECTOR3				m_vPos;
	ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;
	DWORD					m_dwVtxCnt;

	bool					m_bClick;
	float					m_fX;
	float					m_fY;
private:
	HRESULT				Initialize(void);
	HRESULT				AddComponent(void);

public:
	static	CPlayer1_Skill*		Create(LPDIRECT3DDEVICE9 pDevice, float fX, float fY);
	void				Update(void);
	void				Render(void);
	void				Release(void);
	void				SetPos(float fX, float fY, float fZ);
	bool&				GetClick(void)
	{
		return m_bClick;
	}

private:
	explicit CPlayer1_Skill(LPDIRECT3DDEVICE9 pDevice, float fX, float fY);
public:
	~CPlayer1_Skill(void);

};

#endif // Player1Skill_h__
