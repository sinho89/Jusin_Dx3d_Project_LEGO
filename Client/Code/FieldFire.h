/*!
 * \file FieldFire.h
 * \date 2016/08/31 2:31
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

#ifndef FieldFire_h__
#define FieldFire_h__


#include "ParticleObject.h"

class CFieldFire : public CParticleObject
{

protected:
	explicit CFieldFire(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CFieldFire();

public:
	static CFieldFire* Create(LPDIRECT3DDEVICE9 pDevice);
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

	void SetPos(D3DXVECTOR3 _Pos) { m_vTargetPos = _Pos; }

private:
	virtual HRESULT	Initialize(void);
	void			Release(void);

public:
	virtual void	SetPart(int nIdx);
};
#endif // FieldFire_h__