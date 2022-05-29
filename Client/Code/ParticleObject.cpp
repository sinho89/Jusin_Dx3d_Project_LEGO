#include "stdafx.h"
#include "ParticleObject.h"

CParticleObject::CParticleObject(LPDIRECT3DDEVICE9 pDevice)
: CClientObj(pDevice)
, m_pTargetInfo(NULL)
, m_fTime(0.f)
{

}

CParticleObject::~CParticleObject()
{

}
HRESULT CParticleObject::Initialize(void)
{
	return S_OK;
}
void CParticleObject::Update(void)
{
	
}

void CParticleObject::Render(void)
{

}
/*int CParticleObject::SortFnc(const ENGINE::PARTICLE* p1, const ENGINE::PARTICLE* p2)
{
	FLOAT	v1, v2;

	v1 = p1->PrsZ;
	v2 = p2->PrsZ;

	if(v1 < v2)
		return 1;

	else if(v1 == v2)
		return 0;

	else 
		return -1;
}*/
void CParticleObject::SetAni(BOOL bAni/*=TRUE*/)
{

}

void CParticleObject::SetAvgTime(FLOAT fTime)
{

}

void CParticleObject::SetPart(int nIdx)
{

}

D3DXMATRIX* CParticleObject::GetWorldMatrix(void)
{
	return NULL;
}

D3DXMATRIX* CParticleObject::GetViewMatrix(void)
{
	return NULL;
}

D3DXMATRIX* CParticleObject::GetProjMatrix(void)
{
	return NULL;
}

LPDIRECT3DDEVICE9* CParticleObject::GetDevice(void)
{
	return NULL;
}

void CParticleObject::Release(void)
{

}
