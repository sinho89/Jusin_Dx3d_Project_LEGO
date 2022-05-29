#include "Camera.h"
#include "Pipeline.h"

ENGINE::CCamera::CCamera(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_vEye(0.f, 0.f, 0.f)
, m_vAt(0.f, 0.f, 0.f)
, m_vUp(0.f, 1.f, 0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

ENGINE::CCamera::~CCamera(void)
{

}

void ENGINE::CCamera::SetViewSpaceMatrix(const D3DXVECTOR3* pEye, 
										 const D3DXVECTOR3* pAt, 
										 const D3DXVECTOR3* pUp)
{
	CPipeline::MakeViewSpaceMatrix(&m_matView, pEye, pAt, pUp);

}

void ENGINE::CCamera::SetProjectionMatrix(const float& fFovY, 
										  const float& fAspect, 
										  const float& fNear, 
										  const float& fFar)
{
	//D3DXMatrixPerspectiveFovLH(&m_matProj, fFovY, fAspect, fNear, fFar);
	//m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	CPipeline::MakeProjectionMatrix(&m_matProj, fFovY, fAspect, fNear, fFar);
}

