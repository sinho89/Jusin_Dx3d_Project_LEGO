/*!
 * \file ParticleObject.h
 * \date 2016/08/25 17:00
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

#ifndef ParticleObject_h__
#define ParticleObject_h__

#include "ClientObj.h"

class CParticleObject : public CClientObj
{
protected:
	FLOAT						m_fTimeAvg;		// Frame ��� �ð�
	INT							m_PrtN;			// ������ ��
	ENGINE::PARTICLE*			m_PrtD;			// Particle Data
	D3DXVECTOR3					m_vcWind;		// �ٶ� ����

	// ȭ�� ��¿�
	INT							m_iVtx;			// Vertex Number
	ENGINE::VTXPTC*				m_pVtx;			// Vertex
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DTEXTURE9			m_pTx;
	BOOL						m_bAni;

	DWORD				m_dTimeCur;
	DWORD				m_dTimeInt;	// TimeInterval

	float				m_fBrightness;
	bool				m_bBrightCheck;
	float				m_fMagmaY;

	float				m_fTime;

	ENGINE::CComponent* m_pTargetInfo;
	D3DXVECTOR3			m_vTargetPos;

	ID3DXMesh*			m_pMsh;
	D3DXMATRIX			m_mtMsh;

	D3DLIGHT9			m_d3Lgt;
	D3DMATERIAL9		m_d3Mtl;

protected:
	explicit CParticleObject(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CParticleObject();

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
	virtual HRESULT	Initialize(void);
	void			Release(void);

public:
	virtual void	SetPart(int nIdx);
};

#endif // ParticleObject_h__