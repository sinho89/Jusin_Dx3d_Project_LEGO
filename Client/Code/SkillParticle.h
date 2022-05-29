/*!
 * \file SkillParticle.h
 * \date 2016/08/26 8:35
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

#ifndef SkillParticle_h__
#define SkillParticle_h__

#include "ParticleObject.h"

class CSkillParticle : public CParticleObject
{

protected:
	explicit CSkillParticle(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CSkillParticle();

public:
	static CSkillParticle* Create(LPDIRECT3DDEVICE9 pDevice);
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
	float		m_fSizeW, m_fSizeH;
public:
	void	SetColor(float fColorRed, float fColorGreen, float fColorBlue, float fColorAlpha);
	void	SetTarget(ENGINE::CComponent* _Target);
	void	SetPos(D3DXVECTOR3 _vTargetPos);
	void	SetSize(float fSizeW, float fSizeH);
private:
	virtual HRESULT	Initialize(void);
	void			Release(void);

public:
	virtual void	SetPart(int nIdx);
};
#endif // SkillParticle_h__