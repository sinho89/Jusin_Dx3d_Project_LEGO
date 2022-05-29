#ifndef Metheo_h__
#define Metheo_h__

#include "CubeObject.h"
class CPlayer_Elond;
class CMetheo : public CCubeObject
{
private:
	HRESULT				Initialize();
	HRESULT				AddComponent(void);
	void				SetTransform(void);

public:
	void				SetPos(float fX, float fY, float fZ);

public:
	static	CMetheo*	Create(LPDIRECT3DDEVICE9 pDevice);

	void				Update(void);
	void				Render(void);
	void				Release(void);
private:
	CPlayer_Elond*		m_pElond;
	ENGINE::CGameObject*	m_pMetheoParticle;	

	D3DXVECTOR3			m_DestPos;
	D3DXVECTOR3			m_DestDir;

	float				m_Gravity;
	float				m_fSkyFall;

	bool				m_bMakeStone;
	bool				m_bDamageCheck;
	bool				m_bShakeCam;

	float				m_fAngle;

private:
	void				Metheo_Damage(void);

public:
	bool&				GetStone(void){	return m_bMakeStone;}
	D3DXVECTOR3&		GetDestPos(void){return m_DestPos;}


private:
	explicit			CMetheo(LPDIRECT3DDEVICE9 pDevice);
public:
	~CMetheo(void);

};

#endif // Metheo_h__
