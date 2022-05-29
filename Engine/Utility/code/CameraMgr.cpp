#include "CameraMgr.h"
#include "Component.h"
#include "LayerMgr.h"
#include "Transform.h"

IMPLEMENT_SINGLETON(ENGINE::CCameraMgr)

ENGINE::CCameraMgr::CCameraMgr(void)
: m_bStage1_ViewBossAction(false)
{
	
}

ENGINE::CCameraMgr::~CCameraMgr(void)
{

}

void ENGINE::CCameraMgr::Strafe(float _fUnits)
{
	switch(m_eCamType)
	{
	case CAM_PLAY:
		m_tInfo.m_vPos += m_tInfo.m_vRight * _fUnits;
	case CAM_DIR_1:
		break;
	case CAM_DIR_2:
		break;
	case CAM_DIR_3:
		break;
	}
}

void ENGINE::CCameraMgr::Fly(float _fUnits)
{
	switch(m_eCamType)
	{
	case CAM_PLAY:
		m_tInfo.m_vPos += m_tInfo.m_vUp * _fUnits;
	case CAM_DIR_1:
		break;
	case CAM_DIR_2:
		break;
	case CAM_DIR_3:
		break;
	}
}

void ENGINE::CCameraMgr::Walk(float _fUnits)
{
	if(_fUnits == 0)
		return;

	switch(m_eCamType)
	{
	case CAM_PLAY:
		m_tInfo.m_vPos += m_tInfo.m_vLook * _fUnits;
		break;
	case CAM_DIR_1:
		break;
	case CAM_DIR_2:
		break;
	case CAM_DIR_3:
		break;
	}
}
void ENGINE::CCameraMgr::pitch(float angle)
{
	// Calculate new forward
	rotateAroundAxis(m_tInfo.m_vLook, m_tInfo.m_vRight, angle);
	// Calculate new camera up vector
	D3DXVec3Cross(&m_tInfo.m_vUp, &m_tInfo.m_vLook, &m_tInfo.m_vRight);

}

// Rotates camera around the world Y-axis
void ENGINE::CCameraMgr::rotateY(float angle)
{
	float xxx, zzz;
	float sinAng, cosAng;

	// Get sin()/cos() of angle
	sinAng = sinf(angle);
	cosAng = cosf(angle);

	// Save off forward components for computation
	xxx = m_tInfo.m_vLook.x;
	zzz = m_tInfo.m_vLook.z;

	// Rotate forward vector
	m_tInfo.m_vLook.x = xxx * cosAng + zzz * sinAng;
	m_tInfo.m_vLook.z = xxx * -sinAng + zzz * cosAng;

	// Save off up components for computation
	xxx = m_tInfo.m_vUp.x;
	zzz = m_tInfo.m_vUp.z;

	// Rotate up vector
	m_tInfo.m_vUp.x = xxx * cosAng + zzz * sinAng;
	m_tInfo.m_vUp.z = xxx * -sinAng + zzz * cosAng;

	// Save off right components for computation
	xxx = m_tInfo.m_vRight.x;
	zzz = m_tInfo.m_vRight.z;

	// Rotate right vector
	m_tInfo.m_vRight.x = xxx * cosAng + zzz * sinAng;
	m_tInfo.m_vRight.z = xxx * -sinAng + zzz * cosAng;
}
D3DXVECTOR3 ENGINE::CCameraMgr::rotateAroundAxis(const D3DXVECTOR3& vec, const D3DXVECTOR3& axis, float angle)
{
	float aX = axis.x; // Axis X
	float aY = axis.y; // Axis Y
	float aZ = axis.z; // Axis Z

	// Compute the sin and cos of the angle 
	float sinAng = sinf(angle);
	float cosAng = cosf(angle);

	// Calculate X component
	float xxx = (aX * aX * (1.0f - cosAng) + cosAng) * vec.x +
		(aX * aY * (1.0f - cosAng) + aZ * sinAng) * vec.y +
		(aX * aZ * (1.0f - cosAng) - aY * sinAng) * vec.z;

	// Calculate Y component	   
	float yyy = (aX * aY * (1.0f - cosAng) - aZ * sinAng) * vec.x +
		(aY * aY * (1.0f - cosAng) + cosAng) * vec.y +
		(aY * aZ * (1.0f - cosAng) + aX * sinAng) * vec.z;

	// Calculate Z component
	float zzz = (aX * aZ * (1.0f - cosAng) + aY * sinAng) * vec.x +
		(aY * aZ * (1.0f - cosAng) - aX * sinAng) * vec.y +
		(aZ * aZ * (1.0f - cosAng) + cosAng) * vec.z;

	return D3DXVECTOR3(xxx, yyy, zzz); // Return result
}
void ENGINE::CCameraMgr::Pitch(float _fAngle)
{
	D3DXMATRIX	vTemp;
	D3DXMatrixRotationAxis(&vTemp, &m_tInfo.m_vRight, _fAngle * -1);

	// "Right" Base _ "Up" & "Look" Rotation 
	D3DXVec3TransformCoord(&m_tInfo.m_vUp, &m_tInfo.m_vUp, &vTemp);
	D3DXVec3TransformCoord(&m_tInfo.m_vLook, &m_tInfo.m_vLook, &vTemp);

	D3DXVec3Normalize(&m_tInfo.m_vUp, &m_tInfo.m_vUp);
	D3DXVec3Normalize(&m_tInfo.m_vLook, &m_tInfo.m_vLook);
}

void ENGINE::CCameraMgr::Yaw(float _fAngle)
{
	D3DXMATRIX	vTemp;

	if(m_eCamType == CAM_END)
		return;

	switch(m_eCamType)
	{
	case CAM_PLAY:
		//D3DXMatrixRotationY(&vTemp, _fAngle); // Up Vector Base
		D3DXMatrixRotationAxis(&vTemp, &m_tInfo.m_vUp, _fAngle);
		break;
	case CAM_DIR_1:
		break;
	case CAM_DIR_2:
		break;
	case CAM_DIR_3:
		break;
	}

	// Rotate Right and Look Around Up or Y-Axis

	D3DXVec3TransformCoord(&m_tInfo.m_vRight, &m_tInfo.m_vRight, &vTemp);
	D3DXVec3TransformCoord(&m_tInfo.m_vLook, &m_tInfo.m_vLook, &vTemp);

	D3DXVec3Normalize(&m_tInfo.m_vRight, &m_tInfo.m_vRight);
	D3DXVec3Normalize(&m_tInfo.m_vLook, &m_tInfo.m_vLook);
	
}

void ENGINE::CCameraMgr::Roll(float _fAngle)
{
	D3DXMATRIX T;

	switch(m_eCamType) // 필요없긴함 일단 보류
	{
	case CAM_PLAY:		
		D3DXMatrixRotationAxis(&T, &m_tInfo.m_vLook, _fAngle);

		D3DXVec3TransformCoord(&m_tInfo.m_vRight, &m_tInfo.m_vRight, &T);
		D3DXVec3TransformCoord(&m_tInfo.m_vUp, &m_tInfo.m_vUp, &T);
		break;
	case CAM_DIR_1:
		break;
	case CAM_DIR_2:
		break;
	case CAM_DIR_3:
		break; 
	}

	D3DXVec3Normalize(&m_tInfo.m_vRight, &m_tInfo.m_vRight);
	D3DXVec3Normalize(&m_tInfo.m_vUp, &m_tInfo.m_vUp);
}

void ENGINE::CCameraMgr::GetViewMatrix(D3DXMATRIX* _pMat, D3DXVECTOR3*	_pAt)
{

	//D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(_pMat, &m_tInfo.m_vPos, _pAt, &m_tInfo.m_vUp);

	m_vView = *(_pMat);
}
void ENGINE::CCameraMgr::Player1BossView(D3DXMATRIX* _pMat)
{
	CLayerMgr::GetInstance()->SetDivisionType(ENGINE::CLayerMgr::DIV_SINGLE);

	//D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(_pMat, &m_tInfo.m_vPos, &((ENGINE::CTransform*)m_pPlayer1)->m_vPos, &m_tInfo.m_vUp);

	m_vView = (*_pMat);
}

void ENGINE::CCameraMgr::Player2BossView(D3DXMATRIX* _pMat)
{
	//D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(_pMat, &m_tInfo.m_vPos, &((ENGINE::CTransform*)m_pPlayer2)->m_vPos, &m_tInfo.m_vUp);

	m_vView = (*_pMat);
}
void ENGINE::CCameraMgr::Player1ViewMatrix(D3DXMATRIX* _pMat)
{
	//D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	m_tInfo.m_vPos.x = ((ENGINE::CTransform*)m_pPlayer1)->m_vPos.x;

	D3DXMatrixLookAtLH(_pMat, &m_tInfo.m_vPos, &((ENGINE::CTransform*)m_pPlayer1)->m_vPos, &m_tInfo.m_vUp);
}

void ENGINE::CCameraMgr::Player2ViewMatrix(D3DXMATRIX* _pMat)
{
	//D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	m_tInfo.m_vPos.x = ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.x;

	D3DXMatrixLookAtLH(_pMat, &m_tInfo.m_vPos, &((ENGINE::CTransform*)m_pPlayer2)->m_vPos, &m_tInfo.m_vUp);
}
void ENGINE::CCameraMgr::SetCameraType(CameraType _Type)
{
	m_eCamType = _Type;
}

void ENGINE::CCameraMgr::GetPos(D3DXVECTOR3* _pPos)
{
	*_pPos = m_tInfo.m_vPos;
}

void ENGINE::CCameraMgr::SetPos(D3DXVECTOR3* _pPos)
{
	m_tInfo.m_vPos = *_pPos;
}

void ENGINE::CCameraMgr::GetRight(D3DXVECTOR3* _pRight)
{
	*_pRight = m_tInfo.m_vRight;
}

void ENGINE::CCameraMgr::GetUp(D3DXVECTOR3* _pUp)
{
	*_pUp = m_tInfo.m_vUp;
}

void ENGINE::CCameraMgr::GetLook(D3DXVECTOR3* _pLook)
{
	*_pLook = m_tInfo.m_vLook;
}

void ENGINE::CCameraMgr::CamInit(LPDIRECT3DDEVICE9 pDevice)
{
	m_tInfo.m_vPos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_tInfo.m_vRight	= D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_tInfo.m_vUp		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_tInfo.m_vLook		= D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_OriPos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_pDevice			= pDevice;
	m_eCamType			= CAM_PLAY;
	m_eType				= DIS_NORMAL;
	m_eDirectType		= DIR_OPENING;

	m_Gravity			= 3.7f;
	m_fShakeTimer		= 0.f;

	for(int i = 0; i < DIR_END; ++i)
		m_bDirect[i] = false;

	for(int i = 0; i < OP_END; ++i)
		m_bOverlapProtect[i] = false;

	m_bShake = false;
	m_fShakeCount = 0;
	m_bStage1_View_Volcano = false;
	m_bStage1_View_Boss_Ready = false;
	m_bPlayer1Delete = false;
	m_bOzoom = false;
}
void ENGINE::CCameraMgr::Stage1InitCam(void)
{
	m_tInfo.m_vPos				 = D3DXVECTOR3(0.f, 100.f, 100.f);

	m_bDirect[DIR_OPENING]		 = true;

	m_vDirectTarget[DIR_NONE].x = abs((((ENGINE::CTransform*)m_pPlayer1)->m_vPos.x + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.x) / 2.f);
	m_vDirectTarget[DIR_NONE].y = abs((((ENGINE::CTransform*)m_pPlayer1)->m_vPos.y + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.y) / 2.f);
	m_vDirectTarget[DIR_NONE].z = abs((((ENGINE::CTransform*)m_pPlayer1)->m_vPos.z + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.z) / 2.f);

	m_vDirectTarget[DIR_OPENING] = D3DXVECTOR3(60.f, 10.f, 50.f);
	m_vDirectTarget[DIR_STAGE1_BOSS_START] = D3DXVECTOR3(40.f, 2.f, 50.f); 

}

void ENGINE::CCameraMgr::Stage2InitCam(void)
{
	m_tInfo.m_vPos				 = D3DXVECTOR3(0.f, 40.f, -20.f);

	m_vDirectTarget[DIR_NONE].x = abs((((ENGINE::CTransform*)m_pPlayer1)->m_vPos.x + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.x) / 2.f);
	m_vDirectTarget[DIR_NONE].y = abs((((ENGINE::CTransform*)m_pPlayer1)->m_vPos.y + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.y) / 2.f);
	m_vDirectTarget[DIR_NONE].z = abs((((ENGINE::CTransform*)m_pPlayer1)->m_vPos.z + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.z) / 2.f);

	for(int i = 0; i < DIR_END; ++i)
		m_bDirect[i] = false;

	for(int i = 0; i < OP_END; ++i)
		m_bOverlapProtect[i] = false;

	m_bOverlapProtect[DIR_OPENING] = true;
	m_bOverlapProtect[DIR_STAGE1_BOSS_START] = true;

	m_Gravity = 0.f;

	m_bShake = false;
	m_bBoss1Shake = false;

	m_bStage1_View_Volcano = false;
	m_bStage1_View_Boss_Ready = false;

	m_bDirect[DIR_NONE] = true;
	m_eType	= DIS_STAGE2;
}
ENGINE::CAM_INFO* ENGINE::CCameraMgr::GetInfo()
{
	return &m_tInfo;
}


void ENGINE::CCameraMgr::SetPlayer1(CComponent* _pPlayer1)
{
	m_pPlayer1 = _pPlayer1;
}

void ENGINE::CCameraMgr::SetPlayer2(CComponent* _pPlayer2)
{
	m_pPlayer2 = _pPlayer2;
}

D3DXVECTOR3* ENGINE::CCameraMgr::GetAt()
{
	return m_pAt;
}

ENGINE::CCameraMgr::DisplayType ENGINE::CCameraMgr::GetDisType()
{
	return m_eType;
}

void ENGINE::CCameraMgr::SetDisType(DisplayType _eType)
{
	m_eType = _eType;
}

D3DXMATRIX* ENGINE::CCameraMgr::GetView()
{
	return &m_vView;
}

ENGINE::CCameraMgr::DirectType ENGINE::CCameraMgr::GetDirectType()
{
	return m_eDirectType;
}

void ENGINE::CCameraMgr::SetDirectType(DirectType _eType)
{
	m_eDirectType = _eType;
}

bool* ENGINE::CCameraMgr::GetDirectState()
{
	return m_bDirect;
}

void ENGINE::CCameraMgr::OpeningAction(float _fTime)
{
	if(m_bDirect[DIR_OPENING] == false)
		return;
	if(m_bOverlapProtect[OP_OPENING] == true)
		return;

	m_OriPos = m_tInfo.m_vPos;

	if(m_tInfo.m_vPos.y < 40)
	{
		m_bDirect[DIR_OPENING] = false;
		m_bDirect[DIR_NONE] = true;
		m_bOverlapProtect[OP_OPENING] = true;
	}
	if(m_tInfo.m_vPos.z < 50)
	{
		m_tInfo.m_vPos.x += 0.002f / _fTime;
		m_tInfo.m_vPos.y -= 0.001f / _fTime;
		m_tInfo.m_vPos.z -= 0.002f / _fTime;
	}
	else
	{
		m_tInfo.m_vPos.x -= 0.002f / _fTime;
		m_tInfo.m_vPos.y -= 0.001f / _fTime;
		m_tInfo.m_vPos.z -= 0.002f / _fTime;
	}

	D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&m_vView, &m_tInfo.m_vPos, &m_vDirectTarget[DIR_OPENING], &vUp);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_vView);
}

void ENGINE::CCameraMgr::DivisionCamera()
{
	if(m_bDirect[DIR_NONE] == false)
		return;

	if(m_eType == DIS_STAGE1_BOSS)
	{
		CLayerMgr::GetInstance()->SetDivisionType(ENGINE::CLayerMgr::DIV_SINGLE);
		return;
	}
	float fX = ((ENGINE::CTransform*)m_pPlayer1)->m_vPos.x - ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.x;
	float fY = ((ENGINE::CTransform*)m_pPlayer1)->m_vPos.y - ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.y;
	float fZ = ((ENGINE::CTransform*)m_pPlayer1)->m_vPos.z - ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.z;

	float fDistance = sqrtf(fX * fX + fY * fY + fZ * fZ);

	if(fDistance < 30)
	{
		CLayerMgr::GetInstance()->SetDivisionType(ENGINE::CLayerMgr::DIV_SINGLE);
	}
	else
	{
		CLayerMgr::GetInstance()->SetDivisionType(ENGINE::CLayerMgr::DIV_MULTI);
	}
}

void ENGINE::CCameraMgr::Stage1BasicView()
{
	if(m_bDirect[DIR_NONE] == false)
		return;

	if(CLayerMgr::GetInstance()->GetDivisionType() == ENGINE::CLayerMgr::DIV_MULTI)
		return;	
	
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	if(m_eType == DIS_NORMAL)
	{
		m_vDirectTarget[DIR_NONE].x = abs((((ENGINE::CTransform*)m_pPlayer1)->m_vPos.x + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.x) / 2.f);
		m_vDirectTarget[DIR_NONE].z = abs((((ENGINE::CTransform*)m_pPlayer1)->m_vPos.z + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.z) / 2.f);

		m_tInfo.m_vPos.x = m_vDirectTarget[DIR_NONE].x - 0.5f;

	}
	else if(m_eType == DIS_STAGE1_BOSS)
	{
		m_vDirectTarget[DIR_NONE].x = abs((((ENGINE::CTransform*)m_pPlayer1)->m_vPos.x + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.x) / 2.f);
		m_vDirectTarget[DIR_NONE].z = abs((((ENGINE::CTransform*)m_pPlayer1)->m_vPos.z + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.z) / 2.f);

		m_tInfo.m_vPos.z = m_vDirectTarget[DIR_NONE].z - 0.5f;
	}
	
	D3DXMatrixLookAtLH(&m_vView, &m_tInfo.m_vPos, &m_vDirectTarget[DIR_NONE], &vUp);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_vView);
}

void ENGINE::CCameraMgr::Stage2BasicView()
{
	if(m_bDirect[DIR_NONE] == false)
		return;

	if(CLayerMgr::GetInstance()->GetDivisionType() == ENGINE::CLayerMgr::DIV_MULTI)
		return;	

	D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	if(m_eType == DIS_STAGE2)
	{
		m_vDirectTarget[DIR_NONE].x = abs((((ENGINE::CTransform*)m_pPlayer1)->m_vPos.x + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.x) / 2.f);
		m_vDirectTarget[DIR_NONE].z = abs((((ENGINE::CTransform*)m_pPlayer1)->m_vPos.z + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.z) / 2.f);

		float fX = abs(m_vDirectTarget[DIR_NONE].x - m_tInfo.m_vPos.x);
		float fZ = abs(m_vDirectTarget[DIR_NONE].z - m_tInfo.m_vPos.z);

		float fDist = sqrtf(fX * fX + fZ * fZ);

		m_tInfo.m_vPos.y = fDist;
	}

	D3DXMatrixLookAtLH(&m_vView, &m_tInfo.m_vPos, &m_vDirectTarget[DIR_NONE], &vUp);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_vView);
}

bool ENGINE::CCameraMgr::Stage1BossAction(float _fTime)
{
	if(((ENGINE::CTransform*)m_pPlayer1)->m_vPos.x > 150.f
		&& ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.x > 150.f
		&& m_bDirect[DIR_STAGE1_BOSS_START] == false
		&& m_bOverlapProtect[OP_STAGE1] == false)
	//if(m_bStage1_ViewBossAction == true)
	{
		CLayerMgr::GetInstance()->SetDivisionType(ENGINE::CLayerMgr::DIV_SINGLE);
		m_bDirect[DIR_NONE]	= false;
		m_bDirect[DIR_STAGE1_BOSS_START] = true;

		m_tInfo.m_vPos = D3DXVECTOR3(0.f, 20.f, 50.f);
		m_vDirectTarget[DIR_STAGE1_BOSS_START] = D3DXVECTOR3(200.f, 20.f, 50.f);

	}

	if(m_bDirect[DIR_STAGE1_BOSS_START] == false)
		return false;

	if(m_tInfo. m_vPos.x > 100 && m_bStage1_View_Volcano == false)
	{
		m_bStage1_View_Volcano = true;
		m_Gravity = 0.f;
		return true;
	}
	
	if(m_bStage1_View_Volcano == true)
	{
		if(m_bStage1_View_Boss_Ready == false)
		{
			m_Gravity += _fTime; // 여기서 화산쪽으로 화면 꺾임

			m_vDirectTarget[DIR_STAGE1_BOSS_START].y += 0.098 * m_Gravity * m_Gravity * 0.5f;
			m_vDirectTarget[DIR_STAGE1_BOSS_START].z -= 0.098 * m_Gravity * m_Gravity * 0.5f;

			ShakeCamera(m_vDirectTarget);

			if(m_vDirectTarget[DIR_STAGE1_BOSS_START].y > 45.f)
			{
				m_bStage1_View_Boss_Ready = true; // 화산이 화면 정중앙으로 도착
				m_Gravity = 0.f;
			}
		}
		else 
		{
			if(m_tInfo.m_vPos.x < 20.f)
			{
				if(m_tInfo.m_vPos.y > 30.f) // 보스뷰시작
				{
					m_Gravity = 0.f;
					m_bOverlapProtect[OP_STAGE1] = true;
					m_bDirect[DIR_NONE]	= true;
					m_bDirect[DIR_STAGE1_BOSS_START] = false;
					m_eType = DIS_STAGE1_BOSS;
					
					return false;
				}
				else
				{
					m_tInfo.m_vPos.y += 0.098 * m_Gravity * m_Gravity * 0.2f;
				}
				
			}
			else // 화산을 바라본상태로 뒤로 물러남
			{
				m_Gravity += _fTime;
				m_vDirectTarget[DIR_STAGE1_BOSS_START].y -= 0.098 * m_Gravity * m_Gravity * 0.2f;
				m_vDirectTarget[DIR_STAGE1_BOSS_START].z += 0.098 * m_Gravity * m_Gravity * 0.1f;
				m_tInfo.m_vPos.x -= 0.098f * m_Gravity * m_Gravity * 0.5f;
			}
		}
	}

	else
	{
		m_Gravity -= _fTime;
		m_tInfo.m_vPos.x += 0.098 * m_Gravity * m_Gravity * 0.5f;
	}

	D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&m_vView, &m_tInfo.m_vPos, &m_vDirectTarget[DIR_STAGE1_BOSS_START], &vUp);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_vView);

	return false;
}

void ENGINE::CCameraMgr::Stage2BossAction(float _fTime)
{
	if( (((ENGINE::CTransform*)m_pPlayer1)->m_vPos.x > 70.f)
		&& (((ENGINE::CTransform*)m_pPlayer2)->m_vPos.x > 70.f)
		&& m_bDirect[DIR_STAGE2_BOSS_START] == false) // 연출시작
	{
		m_tInfo.m_vPos = D3DXVECTOR3(-50.f, 30.f, 10.f);
		m_vDirectTarget[DIR_STAGE2_BOSS_START] = D3DXVECTOR3(200.f, 5.f, 30.f);
		m_bDirect[DIR_NONE]	= false;
		m_bDirect[DIR_STAGE2_BOSS_START] = true;
	}

	if( (((ENGINE::CTransform*)m_pPlayer1)->m_vPos.x <= 70.f)
		&& (((ENGINE::CTransform*)m_pPlayer2)->m_vPos.x <= 70.f) )
		return;

	if(m_eType == DIS_STAGE2_BOSS)
	{
		m_tInfo.m_vPos.x = ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.x;
		m_tInfo.m_vPos.y = ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.y;
		m_tInfo.m_vPos.z = ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.z;
	}


	if(m_eType == DIS_STAGE2_ONE && ((ENGINE::CTransform*)m_pPlayer1)->m_vPos.x > 40.f && m_bPlayer1Delete == false)
	{
		if(!m_bOzoom)
			m_bOzoom = true;
		m_bPlayer1Delete = true;
	}
	if(m_bPlayer1Delete == false
		&& m_eType == DIS_STAGE2_ONE
		&& ((ENGINE::CTransform*)m_pPlayer1)->m_vPos.x < 40.f)
	{
		D3DXVECTOR3 vGoalPos = ((ENGINE::CTransform*)m_pPlayer2)->m_vPos;
		D3DXVECTOR3	vActionDir = vGoalPos - m_tInfo.m_vPos;

		m_vDirectTarget[DIR_STAGE2_BOSS_START] = ((ENGINE::CTransform*)m_pPlayer2)->m_vPos;

		D3DXVec3Normalize(&vActionDir, &vActionDir);
		m_tInfo.m_vPos += vActionDir * 50.f * _fTime;

		float fX = abs(vGoalPos.x - m_tInfo.m_vPos.x);
		float fY = abs(vGoalPos.y - m_tInfo.m_vPos.y);
		float fZ = abs(vGoalPos.z - m_tInfo.m_vPos.z);

		float fDist = sqrtf(fX * fX + fY * fY + fZ * fZ);

		if(fDist < 1.f)
		{
			m_tInfo.m_vPos.x = ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.x - 2.f;
			m_tInfo.m_vPos.y = ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.y + 2.f;
			m_tInfo.m_vPos.z = ((ENGINE::CTransform*)m_pPlayer2)->m_vPos.z;

			m_vDirectTarget[DIR_STAGE2_BOSS_START] = D3DXVECTOR3(200.f, 5.f, 30.f);
			m_bDirect[DIR_NONE]	= false;
			m_bDirect[DIR_STAGE2_BOSS_START] = false;
			m_eType = DIS_STAGE2_BOSS;
		}

	}
	if(m_tInfo.m_vPos.x > 70 
		&& m_eType == DIS_STAGE2)
	{
		D3DXVECTOR3	vCenter = ((((ENGINE::CTransform*)m_pPlayer1)->m_vPos + ((ENGINE::CTransform*)m_pPlayer2)->m_vPos)) / 2.f;
		D3DXVECTOR3	vActionDir = vCenter - m_vDirectTarget[DIR_STAGE2_BOSS_START];
		
		D3DXVec3Normalize(&vActionDir, &vActionDir);

		float fX = abs(vCenter.x - m_vDirectTarget[DIR_STAGE2_BOSS_START].x);
		float fY = abs(vCenter.y - m_vDirectTarget[DIR_STAGE2_BOSS_START].y);
		float fZ = abs(vCenter.z - m_vDirectTarget[DIR_STAGE2_BOSS_START].z);

		float fDist = sqrtf(fX * fX + fY * fY + fZ * fZ);

		if(fDist < 1.f)
		{
			m_eType = DIS_STAGE2_ONE;
		}
		else
		{
			if(m_eType == DIS_STAGE2_ONE)
				return;
			m_vDirectTarget[DIR_STAGE2_BOSS_START] += vActionDir * 50.f * _fTime;
		}
	}
	else
	{
		if(m_eType == DIS_STAGE2)
		{
			m_Gravity += _fTime;
			m_tInfo.m_vPos.x += 0.098 * m_Gravity * m_Gravity * 0.5f;
		}
	}
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&m_vView, &m_tInfo.m_vPos, &m_vDirectTarget[DIR_STAGE2_BOSS_START], &vUp);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_vView);

}
bool ENGINE::CCameraMgr::ShakeCamera(D3DXVECTOR3*	_ViewTarget)
{
	/*if(!m_bShake)
		return;*/

	m_fShakeTimer += 8.f * m_fTime;

	if(m_fShakeTimer > 0.f && m_fShakeTimer <= 1.f)
	{
		m_tInfo.m_vPos.z += 0.2f;
		_ViewTarget[DIR_STAGE1_BOSS_START].z += 0.2f;
		m_tInfo.m_vPos.y += 0.2f;
		_ViewTarget[DIR_STAGE1_BOSS_START].y += 0.2f;
	}
	if(m_fShakeTimer > 1.f && m_fShakeTimer <= 2.f)
	{
		m_tInfo.m_vPos.z -= 0.2f;
		_ViewTarget[DIR_STAGE1_BOSS_START].z -= 0.2f;
		m_tInfo.m_vPos.y -= 0.2f;
		_ViewTarget[DIR_STAGE1_BOSS_START].y -= 0.2f;
	}
	if(m_fShakeTimer > 2.f && m_fShakeTimer <= 3.f)
	{
		m_tInfo.m_vPos.z += 0.4f;
		_ViewTarget[DIR_STAGE1_BOSS_START].z += 0.4f;
		m_tInfo.m_vPos.y += 0.4f;
		_ViewTarget[DIR_STAGE1_BOSS_START].y += 0.4f;
	}
	if(m_fShakeTimer > 3.f && m_fShakeTimer <= 4.f)
	{
		m_tInfo.m_vPos.z -= 0.4f;
		_ViewTarget[DIR_STAGE1_BOSS_START].z -= 0.4f;
		m_tInfo.m_vPos.y -= 0.4f;
		_ViewTarget[DIR_STAGE1_BOSS_START].y -= 0.4f;
	}
	if(m_fShakeTimer > 4.f && m_fShakeTimer <= 5.f)
	{
		m_tInfo.m_vPos.z += 0.6f;
		_ViewTarget[DIR_STAGE1_BOSS_START].z += 0.6f;
		m_tInfo.m_vPos.y += 0.6f;
		_ViewTarget[DIR_STAGE1_BOSS_START].y += 0.6f;
	}
	if(m_fShakeTimer > 5.f && m_fShakeTimer <= 6.f)
	{
		m_tInfo.m_vPos.z -= 0.6f;
		_ViewTarget[DIR_STAGE1_BOSS_START].z -= 0.6f;
		m_tInfo.m_vPos.y -= 0.6f;
		_ViewTarget[DIR_STAGE1_BOSS_START].y -= 0.6f;
	}
	if(m_fShakeTimer > 6.f && m_fShakeTimer <= 7.f)
	{
		m_tInfo.m_vPos.z += 0.8f;
		_ViewTarget[DIR_STAGE1_BOSS_START].z += 0.8f;
		m_tInfo.m_vPos.y += 0.8f;
		_ViewTarget[DIR_STAGE1_BOSS_START].y += 0.8f;
	}
	if(m_fShakeTimer > 7.f && m_fShakeTimer <= 8.f)
	{
		m_tInfo.m_vPos.z -= 0.8f;
		_ViewTarget[DIR_STAGE1_BOSS_START].z -= 0.8f;
		m_tInfo.m_vPos.y -= 0.8f;
		_ViewTarget[DIR_STAGE1_BOSS_START].y -= 0.8f;
	}
	if(m_fShakeTimer > 8.f && m_fShakeTimer <= 9.f)
	{
		m_tInfo.m_vPos.z += 1.f;
		_ViewTarget[DIR_STAGE1_BOSS_START].z += 1.f;
		m_tInfo.m_vPos.y += 1.f;
		_ViewTarget[DIR_STAGE1_BOSS_START].y += 1.f;
	}
	if(m_fShakeTimer > 9.f && m_fShakeTimer <= 10.f)
	{
		m_tInfo.m_vPos.z -= 1.f;
		_ViewTarget[DIR_STAGE1_BOSS_START].z -= 1.f;
		m_tInfo.m_vPos.y -= 1.f;
		_ViewTarget[DIR_STAGE1_BOSS_START].y -= 1.f;
	}
	if(m_fShakeTimer > 10.f)
	{
		m_fShakeTimer = 0;
		m_fTime = 0;
		++m_fShakeCount;

		if(m_fShakeCount > 2)
			return true;
	}

	return false;
}

void ENGINE::CCameraMgr::Stage1BossView()
{

}

void ENGINE::CCameraMgr::Stage2BossView()
{

}

void ENGINE::CCameraMgr::SetTime(float _fTime)
{
	m_fTime = _fTime;
}

bool ENGINE::CCameraMgr::Stage1BossShakeCamera()
{
	if(!m_bBoss1Shake)
		return false;
	m_fShakeTimer += 8.f * m_fTime;

	if(m_fShakeTimer > 0.f && m_fShakeTimer <= 1.f)
	{
		m_tInfo.m_vPos.y += 0.2f;
		m_vDirectTarget[DIR_STAGE1_BOSS_START].y += 0.2f;
	}
	if(m_fShakeTimer > 1.f && m_fShakeTimer <= 2.f)
	{
		m_tInfo.m_vPos.y -= 0.2f;
		m_vDirectTarget[DIR_STAGE1_BOSS_START].y -= 0.2f;
	}
	if(m_fShakeTimer > 2.f && m_fShakeTimer <= 3.f)
	{
		m_tInfo.m_vPos.y += 0.4f;
		m_vDirectTarget[DIR_STAGE1_BOSS_START].y += 0.4f;
	}
	if(m_fShakeTimer > 3.f && m_fShakeTimer <= 4.f)
	{
		m_tInfo.m_vPos.y -= 0.4f;
		m_vDirectTarget[DIR_STAGE1_BOSS_START].y -= 0.4f;
	}
	if(m_fShakeTimer > 4.f && m_fShakeTimer <= 5.f)
	{
		m_tInfo.m_vPos.y += 0.6f;
		m_vDirectTarget[DIR_STAGE1_BOSS_START].y += 0.6f;
	}
	if(m_fShakeTimer > 5.f && m_fShakeTimer <= 6.f)
	{
		m_tInfo.m_vPos.y -= 0.6f;
		m_vDirectTarget[DIR_STAGE1_BOSS_START].y -= 0.6f;
	}
	if(m_fShakeTimer > 6.f && m_fShakeTimer <= 7.f)
	{
		m_tInfo.m_vPos.y += 0.8f;
		m_vDirectTarget[DIR_STAGE1_BOSS_START].y += 0.8f;
	}
	if(m_fShakeTimer > 7.f && m_fShakeTimer <= 8.f)
	{
		m_tInfo.m_vPos.y -= 0.8f;
		m_vDirectTarget[DIR_STAGE1_BOSS_START].y -= 0.8f;
	}
	if(m_fShakeTimer > 8.f && m_fShakeTimer <= 9.f)
	{
		m_tInfo.m_vPos.y += 1.f;
		m_vDirectTarget[DIR_STAGE1_BOSS_START].y += 1.f;
	}
	if(m_fShakeTimer > 9.f && m_fShakeTimer <= 10.f)
	{
		m_tInfo.m_vPos.y -= 1.f;
		m_vDirectTarget[DIR_STAGE1_BOSS_START].y -= 1.f;
	}
	if(m_fShakeTimer > 10.f)
	{
		m_bBoss1Shake = false;
		m_fShakeTimer = 0;
		m_fTime = 0;
		++m_fShakeCount;
	}

	return false;
}

bool* ENGINE::CCameraMgr::GetBossShake()
{
	return &m_bBoss1Shake;
}

void ENGINE::CCameraMgr::SetStage2BossView()
{
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

	D3DXVECTOR3 vTarget = m_tInfo.m_vPos + m_tInfo.m_vLook;

	D3DXMatrixLookAtLH(&m_vView, &m_tInfo.m_vPos, &vTarget, &vUp);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_vView);
}



