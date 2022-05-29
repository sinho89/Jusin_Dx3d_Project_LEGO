#include "Pipeline.h"
#include "MathMgr.h"

ENGINE::CPipeline::CPipeline(void)
{

}

ENGINE::CPipeline::~CPipeline(void)
{

}

void ENGINE::CPipeline::MakeWorldMatrix(D3DXMATRIX* pMatrix, 
										const D3DXVECTOR3* pScale, 
										const float* pAngle, 
										const D3DXVECTOR3* pPos)
{
	D3DXMatrixIdentity(pMatrix);

	D3DXVECTOR3	vRight	= D3DXVECTOR3(1.f, 0.f, 0.f);
	D3DXVECTOR3	vUp		= D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXVECTOR3	vLook	= D3DXVECTOR3(0.f, 0.f, 1.f);
	D3DXVECTOR3	vPos	= D3DXVECTOR3(0.f, 0.f, 0.f);

	vRight	*= pScale->x;
	vUp     *= pScale->y;
	vLook   *= pScale->z;

	CMathMgr::MyRotaionX(&vRight, &vRight, pAngle[ANGLE_X]);
	CMathMgr::MyRotaionX(&vUp, &vUp, pAngle[ANGLE_X]);
	CMathMgr::MyRotaionX(&vLook, &vLook, pAngle[ANGLE_X]);

	CMathMgr::MyRotaionY(&vRight, &vRight, pAngle[ANGLE_Y]);
	CMathMgr::MyRotaionY(&vUp, &vUp, pAngle[ANGLE_Y]);
	CMathMgr::MyRotaionY(&vLook, &vLook, pAngle[ANGLE_Y]);

	CMathMgr::MyRotaionZ(&vRight, &vRight, pAngle[ANGLE_Z]);
	CMathMgr::MyRotaionZ(&vUp, &vUp, pAngle[ANGLE_Z]);
	CMathMgr::MyRotaionZ(&vLook, &vLook, pAngle[ANGLE_Z]);

	vPos = *pPos;

	MakeTransformMatrix(pMatrix, &vRight, &vUp, &vLook, &vPos);
}

void ENGINE::CPipeline::MakeTransformMatrix(D3DXMATRIX* pMatrix, 
											const D3DXVECTOR3* pRight, 
											const D3DXVECTOR3* pUp, 
											const D3DXVECTOR3* pLook, 
											const D3DXVECTOR3* pPos)
{
	memcpy(&pMatrix->m[0][0], pRight, sizeof(float) * 3);
	memcpy(&pMatrix->m[1][0], pUp, sizeof(float) * 3);
	memcpy(&pMatrix->m[2][0], pLook, sizeof(float) * 3);
	memcpy(&pMatrix->m[3][0], pPos, sizeof(float) * 3);
}

void ENGINE::CPipeline::MakeViewSpaceMatrix(D3DXMATRIX* pMatrix, 
											const D3DXVECTOR3* pEye, 
											const D3DXVECTOR3* pAt, 
											const D3DXVECTOR3* pUp)
{
	D3DXMatrixIdentity(pMatrix);

	D3DXVECTOR3	vRight	= D3DXVECTOR3(1.f, 0.f, 0.f);
	D3DXVECTOR3	vUp		= D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXVECTOR3	vLook	= D3DXVECTOR3(0.f, 0.f, 1.f);
	D3DXVECTOR3	vPos	= D3DXVECTOR3(0.f, 0.f, 0.f);

	// Look 완성
	vLook = *pAt - *pEye;
	D3DXVec3Normalize(&vLook, &vLook);

	// Right 완성
	D3DXVec3Cross(&vRight, pUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	// Up 완성
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	vPos = *pEye;

	MakeTransformMatrix(pMatrix, &vRight, &vUp, &vLook, &vPos);

	// 카메라의 월드 행렬
	// 카메라 월드 행렬의 역행렬을 구하자

	D3DXMatrixInverse(pMatrix, 0, pMatrix);
}

void ENGINE::CPipeline::MakeProjectionMatrix(D3DXMATRIX* pMatrix, 
											 const float& fFovY, 
											 const float& fAspect, 
											 const float& fNear, 
											 const float& fFar)
{
	D3DXMatrixIdentity(pMatrix);

	pMatrix->_11 = (1.f / tanf(fFovY / 2.f)) / fAspect;
	pMatrix->_22 = 1.f / tanf(fFovY / 2.f);

	pMatrix->_33 =  fFar / (fFar - fNear);
	pMatrix->_43 = -fNear * fFar / (fFar - fNear);

	pMatrix->_34 = 1.f;
	pMatrix->_44 = 0.f;
}

void ENGINE::CPipeline::MyTransformCoord(D3DXVECTOR3* pOut, 
										 const D3DXVECTOR3* pVector, 
										 const D3DXMATRIX* pMatrix)
{
	D3DXVECTOR4		vVector(pVector->x, pVector->y, pVector->z, 1.f);

	pOut->x =	vVector.x * pMatrix->_11 + 
				vVector.y * pMatrix->_21 + 
				vVector.z * pMatrix->_31 + 
				vVector.w * pMatrix->_41;

	pOut->y =	vVector.x * pMatrix->_12 + 
				vVector.y * pMatrix->_22 + 
				vVector.z * pMatrix->_32 + 
				vVector.w * pMatrix->_42;


	pOut->z =	vVector.x * pMatrix->_13 + 
				vVector.y * pMatrix->_23 + 
				vVector.z * pMatrix->_33 + 
				vVector.w * pMatrix->_43;

	float fZ =	vVector.x * pMatrix->_14 + 
				vVector.y * pMatrix->_24 + 
				vVector.z * pMatrix->_34 + 
				vVector.w * pMatrix->_44;

	pOut->x /= fZ;
	pOut->y /= fZ;
	pOut->z /= fZ;
}

