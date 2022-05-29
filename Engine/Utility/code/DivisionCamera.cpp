#include "DivisionCamera.h"

ENGINE::CDivisionCamera::CDivisionCamera()
{
	m_pDev	= NULL;

	D3DXMatrixIdentity(&m_mtViw);
	D3DXMatrixIdentity(&m_mtPrj);


	m_vcEye		= D3DXVECTOR3(0,0,-1);													// Camera position
	m_vcLook	= D3DXVECTOR3(0,0, 0);													// Look vector
	m_vcUp		= D3DXVECTOR3(0,1, 0);														// up vector

	m_fFov		= D3DX_PI/4.f;
	m_fAsp		= 800.f/600.f;
	m_fNr		= 1.f;
	m_fFr		= 10000.f;

}

ENGINE::CDivisionCamera::~CDivisionCamera()
{

}

INT ENGINE::CDivisionCamera::Create(LPDIRECT3DDEVICE9 pDev)
{
	m_pDev = pDev;

	SetParamView(D3DXVECTOR3(1000,500,-1000), D3DXVECTOR3( 1000, 300, 0), D3DXVECTOR3(0,1,0));

	return 0;
}

INT ENGINE::CDivisionCamera::FrameMove()
{
	if(GetAsyncKeyState('W'))
		MoveForward( 4.f, 1.f);
	if(GetAsyncKeyState('S'))
		MoveForward(-4.f, 1.f);
	if(GetAsyncKeyState('A'))
		MoveSideward(-4.f);
	if(GetAsyncKeyState('D'))
		MoveSideward(4.f);

	UpdateViewProj();

	return 0;
}

void ENGINE::CDivisionCamera::SetEye(D3DXVECTOR3 v)
{
	m_vcEye = v;
}

void ENGINE::CDivisionCamera::SetLook(D3DXVECTOR3 v)
{
	m_vcLook = v;
}

void ENGINE::CDivisionCamera::SetUP(D3DXVECTOR3 v)
{
	m_vcUp = v;
}

void ENGINE::CDivisionCamera::SetParamView(D3DXVECTOR3 vcEye, D3DXVECTOR3 vcLook, D3DXVECTOR3 vcUp)
{
	m_vcEye		= vcEye;
	m_vcLook	= vcLook;
	m_vcUp		= vcUp;
}

void ENGINE::CDivisionCamera::SetParamProj(FLOAT fFov, FLOAT fAsp, FLOAT fNear, FLOAT fFar)
{
	m_fFov	= fFov;
	m_fAsp	= fAsp;
	m_fNr	= fNear;
	m_fFr	= fFar;
}

void ENGINE::CDivisionCamera::UpdateViewProj()
{
	D3DXMatrixPerspectiveFovLH(&m_mtPrj, m_fFov, m_fAsp, m_fNr, m_fFr);
	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);

	D3DXMatrixInverse(&m_mtViwI, NULL, &m_mtViw);
	m_mtBill = m_mtViwI;

	m_mtBill._41 = 0.f;
	m_mtBill._42 = 0.f;
	m_mtBill._43 = 0.f;

	m_vcAxisX.x = m_mtViw._11;
	m_vcAxisX.y = m_mtViw._21;
	m_vcAxisX.z = m_mtViw._31;

	m_vcAxisY.x = m_mtViw._12;
	m_vcAxisY.y = m_mtViw._22;
	m_vcAxisY.z = m_mtViw._32;

	m_vcAxisZ.x = m_mtViw._13;
	m_vcAxisZ.y = m_mtViw._23;
	m_vcAxisZ.z = m_mtViw._33;
}	

void ENGINE::CDivisionCamera::SetTransformViw()
{
	m_pDev->SetTransform(D3DTS_VIEW, &m_mtViw);
}

void ENGINE::CDivisionCamera::SetTransformPrj()
{
	m_pDev->SetTransform(D3DTS_PROJECTION, &m_mtPrj);	
}

void ENGINE::CDivisionCamera::MoveSideward(FLOAT fSpeed)
{
	D3DXVECTOR3 tmp(m_mtViw._11, 0, m_mtViw._31);
	D3DXVec3Normalize(&tmp,&tmp);

	m_vcEye  += tmp * fSpeed;
	m_vcLook += tmp * fSpeed;
}

void ENGINE::CDivisionCamera::MoveForward(FLOAT fSpeed, FLOAT fY/*=0*/)
{
	D3DXVECTOR3 tmp(m_mtViw._13, m_mtViw._23*fY, m_mtViw._33);
	D3DXVec3Normalize(&tmp,&tmp);

	m_vcEye  += tmp * fSpeed;
	m_vcLook += tmp * fSpeed;
}

void ENGINE::CDivisionCamera::Rotate(const D3DXVECTOR3* vcDelta, float fSpeed)
{
	D3DXVECTOR3 vcEps = *vcDelta;

	FLOAT	fYaw	;
	FLOAT	fPitch	;

	D3DXVECTOR3 vcZ;
	D3DXVECTOR3 vcY;
	D3DXVECTOR3 vcX;

	D3DXMATRIX rtY;
	D3DXMATRIX rtX;

	// ���� ��ǥ y �࿡ ���� ȸ��
	fYaw	= D3DXToRadian(vcEps.x * fSpeed);
	D3DXMatrixRotationY(&rtY, fYaw);

	vcZ = m_vcLook-m_vcEye;
	vcY = D3DXVECTOR3(m_mtViw._12, m_mtViw._22, m_mtViw._32);

	D3DXVec3TransformCoord(&vcZ, &vcZ, &rtY);
	D3DXVec3TransformCoord(&vcY, &vcY, &rtY);

	m_vcLook= vcZ + m_vcEye;
	m_vcUp	= vcY;
	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);


	// ī�޶��� x �࿡ ���� ȸ��
	fPitch	= D3DXToRadian(vcEps.y * fSpeed);
	vcX	= D3DXVECTOR3(m_mtViw._11, m_mtViw._21, m_mtViw._31);
	vcY = D3DXVECTOR3(m_mtViw._12, m_mtViw._22, m_mtViw._32);
	vcZ = m_vcLook-m_vcEye;

	D3DXMatrixRotationAxis(&rtX, &vcX, fPitch);
	D3DXVec3TransformCoord(&vcZ, &vcZ, &rtX);
	D3DXVec3TransformCoord(&vcY, &vcY, &rtX);

	m_vcLook= vcZ + m_vcEye;
	m_vcUp	= vcY;
	D3DXMatrixLookAtLH(&m_mtViw, &m_vcEye, &m_vcLook, &m_vcUp);

}

