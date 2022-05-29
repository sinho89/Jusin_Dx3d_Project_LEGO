/*!
 * \file HitParticle.h
 * \date 2016/08/29 0:57
 *
 * \author sinho
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef HitParticle_h__
#define HitParticle_h__


#include "ParticleObject.h"

class CHitParticle : public CParticleObject
{
protected:
	explicit CHitParticle(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _Pos);

public:
	virtual ~CHitParticle();

public:
	static CHitParticle* Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _Pos);
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

	void SetPos (D3DXVECTOR3 _Pos);

private:
	virtual HRESULT	Initialize(void);
	void			Release(void);

public:
	virtual void	SetPart(int nIdx);
};

#endif // HitParticle_h__