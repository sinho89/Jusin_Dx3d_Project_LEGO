/*!
 * \file MagmaParticle.h
 * \date 2016/08/27 23:17
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

#ifndef MagmaParticle_h__
#define MagmaParticle_h__


#include "ParticleObject.h"

class CMagmaParticle : public CParticleObject
{

protected:
	explicit CMagmaParticle(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CMagmaParticle();

public:
	static CMagmaParticle* Create(LPDIRECT3DDEVICE9 pDevice);
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
	virtual HRESULT	Initialize(void);
	void			Release(void);

public:
	virtual void	SetPart(int nIdx);
};

#endif // MagmaParticle_h__