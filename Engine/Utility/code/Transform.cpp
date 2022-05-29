#include "Transform.h"
#include "Pipeline.h"

ENGINE::CTransform::CTransform(const D3DXVECTOR3& vLook)
: m_vPos(0.f, 0.f, 0.f)
, m_vDir(vLook)
, m_iHp(0)
{
	ZeroMemory(m_fAngle, sizeof(float) * ANGLE_END);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

ENGINE::CTransform::~CTransform(void)
{

}

void ENGINE::CTransform::Update(void)
{	
	//ENGINE::CPipeline::MakeWorldMatrix(&m_matWorld, 
	//	&D3DXVECTOR3(1.f, 1.f, 1.f), m_fAngle, &m_vPos);
}

ENGINE::CTransform* ENGINE::CTransform::Create(const D3DXVECTOR3& vLook)
{
	return new CTransform(vLook);
}

