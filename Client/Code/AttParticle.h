/*!
 * \file AttParticle.h
 * \date 2016/08/25 19:57
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

#ifndef AttParticle_h__
#define AttParticle_h__

#include "ParticleObject.h"

class CAttParticle : public CParticleObject
{
protected:
	explicit CAttParticle(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CAttParticle();

public:
	static CAttParticle* Create(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual void Update(void);
	virtual void Render(void);
	virtual D3DXMATRIX*	GetWorldMatrix(void);
	virtual D3DXMATRIX* GetViewMatrix(void);
	virtual D3DXMATRIX* GetProjMatrix(void);
	virtual LPDIRECT3DDEVICE9* GetDevice(void);

	virtual void	SetAni(BOOL bAni=TRUE);		// 애니메이션 활성화/ 비활성화
	virtual void	SetAvgTime(FLOAT fTime);	// 평균 시간 설정
	//virtual int		SortFnc(const ENGINE::PARTICLE* p1, const ENGINE::PARTICLE* p2);

private:
	float		m_fColorRed, m_fColorGreen, m_fColorBlue, m_fColorAlpha;

public:
	void			SetColor(float fColorRed, float fColorGreen, float fColorBlue, float fColorAlpha);
	void			SetTarget(ENGINE::CComponent* _Target);
	void			SetPos(D3DXVECTOR3 _vTargetPos);

private:
	virtual HRESULT	Initialize(void);
	void			Release(void);

public:
	virtual void	SetPart(int nIdx);
};

#endif // AttParticle_h__