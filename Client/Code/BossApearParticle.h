/*!
 * \file BossApearParticle.h
 * \date 2016/08/29 19:52
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

#ifndef BossApearParticle_h__
#define BossApearParticle_h__


#include "ParticleObject.h"

class CBossApearParticle : public CParticleObject
{
protected:
	explicit CBossApearParticle(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _Pos);
public:
	virtual ~CBossApearParticle();

private:
	D3DXVECTOR3		m_vDestPos;

public:
	static CBossApearParticle* Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _Pos);
public:
	virtual void Update(void);
	virtual void Render(void);
	virtual D3DXMATRIX*	GetWorldMatrix(void);
	virtual D3DXMATRIX* GetViewMatrix(void);
	virtual D3DXMATRIX* GetProjMatrix(void);
	virtual LPDIRECT3DDEVICE9* GetDevice(void);

	virtual void	SetAni(BOOL bAni=TRUE);		// �ִϸ��̼� Ȱ��ȭ/ ��Ȱ��ȭ
	virtual void	SetAvgTime(FLOAT fTime);	// ��� �ð� ����
	//virtual int		SortFnc(const ENGINE::PARTICLE* p1, const ENGINE::PARTICLE* p2);

private:
	float		m_fColorRed, m_fColorGreen, m_fColorBlue, m_fColorAlpha;
public:
	void	SetColor(float fColorRed, float fColorGreen, float fColorBlue, float fColorAlpha);
	void	SetTarget(ENGINE::CComponent* _Target);
	void	SetPos(D3DXVECTOR3 _vTargetPos);
private:
	virtual HRESULT	Initialize(void);
	void			Release(void);

public:
	virtual void	SetPart(int nIdx);
};
#endif // BossApearParticle_h__