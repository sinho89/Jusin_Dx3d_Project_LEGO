/*!
 * \file MeteoParticle.h
 * \date 2016/08/29 14:19
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

#ifndef MeteoParticle_h__
#define MeteoParticle_h__

#include "ParticleObject.h"

class CMeteoParticle: public CParticleObject
{
protected:
	explicit CMeteoParticle(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _Pos);

public:
	virtual ~CMeteoParticle();

public:
	static CMeteoParticle* Create(LPDIRECT3DDEVICE9 pDevice, D3DXVECTOR3 _Pos);
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

	void SetPos (D3DXVECTOR3 _Pos);

private:
	virtual HRESULT	Initialize(void);
	void			Release(void);

public:
	virtual void	SetPart(int nIdx);
};


#endif // MeteoParticle_h__