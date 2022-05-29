#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"
#include "ResourcesMgr.h"
#include "TimeMgr.h"
#include "Management.h"

#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "InfoSubject.h"
#include "CollisionMgr.h"

#include "Stage_1.h"


CPlayer::CPlayer(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_pResourcesMgr(ENGINE::Get_ResourceMgr())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pManagement(ENGINE::Get_Management())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_fTime(0.f)
, m_dwVtxCnt(0)
, m_pConvertVertex(NULL)
, m_pVertex(NULL)
, m_pCameraObserver(NULL)
, m_pTerrainVertex(NULL)
, m_bMove(false)
{
	for(int i=0; i<BP_END; ++i)
	{
		m_pInfo[i] = ENGINE::CTransform::Create(ENGINE::g_vLook);
		m_pItvXYZ[i] = new ENGINE::ITVXYZ;
	}
}

CPlayer::~CPlayer()
{
	Release();
}
HRESULT CPlayer::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	m_fDistance = 10.f;
	m_fCameraAngle = D3DXToRadian(45.f);
	m_fSpeed = 10.f;

	m_dwVtxCnt			= 8;
	m_pVertex			= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXCUBE[m_dwVtxCnt];

	m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Normal_Cube", m_pVertex);

	
	TCHAR szFilePath[MAX_PATH] = L"../../Data/Object_Data/OriginBody.dat";

	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte = 0;

	while(true)
	{
		ENGINE::SAVECUBE* pSaveCube = new ENGINE::SAVECUBE;
		
		ReadFile(hFile, pSaveCube, sizeof(ENGINE::SAVECUBE), &dwByte, NULL);

		if(dwByte == 0)
		{
			delete pSaveCube;
			pSaveCube = NULL;
			break;
		}

		switch(pSaveCube->m_SaveType)
		{
		case ENGINE::SOBJ_HEAD:
			m_pInfo[BP_HEAD]->m_fAngle[ENGINE::ANGLE_X]	= pSaveCube->m_fAngle[ENGINE::ANGLE_X];
			m_pInfo[BP_HEAD]->m_fAngle[ENGINE::ANGLE_Y]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Y];
			m_pInfo[BP_HEAD]->m_fAngle[ENGINE::ANGLE_Z]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Z];
			m_pInfo[BP_HEAD]->m_matWorld	= pSaveCube->m_matWorld;
			m_pInfo[BP_HEAD]->m_vDir		= pSaveCube->m_vDir;
			m_pInfo[BP_HEAD]->m_vPos		= pSaveCube->m_vPos;

			m_pItvXYZ[BP_HEAD]->m_wItvX		= pSaveCube->m_fItv_X;
			m_pItvXYZ[BP_HEAD]->m_wItvY		= pSaveCube->m_fItv_Y;
			m_pItvXYZ[BP_HEAD]->m_wItvZ		= pSaveCube->m_fItv_Z;
			break;

		case ENGINE::SOBJ_BODY:
			m_pInfo[BP_BODY]->m_fAngle[ENGINE::ANGLE_X]	= pSaveCube->m_fAngle[ENGINE::ANGLE_X];
			m_pInfo[BP_BODY]->m_fAngle[ENGINE::ANGLE_Y]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Y];
			m_pInfo[BP_BODY]->m_fAngle[ENGINE::ANGLE_Z]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Z];
			m_pInfo[BP_BODY]->m_matWorld	= pSaveCube->m_matWorld;
			m_pInfo[BP_BODY]->m_vDir		= pSaveCube->m_vDir;
			m_pInfo[BP_BODY]->m_vPos		= pSaveCube->m_vPos;

			m_pItvXYZ[BP_BODY]->m_wItvX		= pSaveCube->m_fItv_X;
			m_pItvXYZ[BP_BODY]->m_wItvY		= pSaveCube->m_fItv_Y;
			m_pItvXYZ[BP_BODY]->m_wItvZ		= pSaveCube->m_fItv_Z;
			break;

		case ENGINE::SOBJ_L_ARM_UP:
			m_pInfo[BP_L_ARM_UP]->m_fAngle[ENGINE::ANGLE_X]	= pSaveCube->m_fAngle[ENGINE::ANGLE_X];
			m_pInfo[BP_L_ARM_UP]->m_fAngle[ENGINE::ANGLE_Y]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Y];
			m_pInfo[BP_L_ARM_UP]->m_fAngle[ENGINE::ANGLE_Z]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Z];
			m_pInfo[BP_L_ARM_UP]->m_matWorld	= pSaveCube->m_matWorld;
			m_pInfo[BP_L_ARM_UP]->m_vDir		= pSaveCube->m_vDir;
			m_pInfo[BP_L_ARM_UP]->m_vPos		= pSaveCube->m_vPos;

			m_pItvXYZ[BP_L_ARM_UP]->m_wItvX		= pSaveCube->m_fItv_X;
			m_pItvXYZ[BP_L_ARM_UP]->m_wItvY		= pSaveCube->m_fItv_Y;
			m_pItvXYZ[BP_L_ARM_UP]->m_wItvZ		= pSaveCube->m_fItv_Z;
			break;

		case ENGINE::SOBJ_L_ARM_DOWN:
			m_pInfo[BP_L_ARM_DOWN]->m_fAngle[ENGINE::ANGLE_X]	= pSaveCube->m_fAngle[ENGINE::ANGLE_X];
			m_pInfo[BP_L_ARM_DOWN]->m_fAngle[ENGINE::ANGLE_Y]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Y];
			m_pInfo[BP_L_ARM_DOWN]->m_fAngle[ENGINE::ANGLE_Z]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Z];
			m_pInfo[BP_L_ARM_DOWN]->m_matWorld	= pSaveCube->m_matWorld;
			m_pInfo[BP_L_ARM_DOWN]->m_vDir		= pSaveCube->m_vDir;
			m_pInfo[BP_L_ARM_DOWN]->m_vPos		= pSaveCube->m_vPos;

			m_pItvXYZ[BP_L_ARM_DOWN]->m_wItvX		= pSaveCube->m_fItv_X;
			m_pItvXYZ[BP_L_ARM_DOWN]->m_wItvY		= pSaveCube->m_fItv_Y;
			m_pItvXYZ[BP_L_ARM_DOWN]->m_wItvZ		= pSaveCube->m_fItv_Z;
			break;

		case ENGINE::SOBJ_R_ARM_UP:
			m_pInfo[BP_R_ARM_UP]->m_fAngle[ENGINE::ANGLE_X]	= pSaveCube->m_fAngle[ENGINE::ANGLE_X];
			m_pInfo[BP_R_ARM_UP]->m_fAngle[ENGINE::ANGLE_Y]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Y];
			m_pInfo[BP_R_ARM_UP]->m_fAngle[ENGINE::ANGLE_Z]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Z];
			m_pInfo[BP_R_ARM_UP]->m_matWorld	= pSaveCube->m_matWorld;
			m_pInfo[BP_R_ARM_UP]->m_vDir		= pSaveCube->m_vDir;
			m_pInfo[BP_R_ARM_UP]->m_vPos		= pSaveCube->m_vPos;

			m_pItvXYZ[BP_R_ARM_UP]->m_wItvX		= pSaveCube->m_fItv_X;
			m_pItvXYZ[BP_R_ARM_UP]->m_wItvY		= pSaveCube->m_fItv_Y;
			m_pItvXYZ[BP_R_ARM_UP]->m_wItvZ		= pSaveCube->m_fItv_Z;
			break;

		case ENGINE::SOBJ_R_ARM_DOWN:
			m_pInfo[BP_R_ARM_DOWN]->m_fAngle[ENGINE::ANGLE_X]	= pSaveCube->m_fAngle[ENGINE::ANGLE_X];
			m_pInfo[BP_R_ARM_DOWN]->m_fAngle[ENGINE::ANGLE_Y]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Y];
			m_pInfo[BP_R_ARM_DOWN]->m_fAngle[ENGINE::ANGLE_Z]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Z];
			m_pInfo[BP_R_ARM_DOWN]->m_matWorld	= pSaveCube->m_matWorld;
			m_pInfo[BP_R_ARM_DOWN]->m_vDir		= pSaveCube->m_vDir;
			m_pInfo[BP_R_ARM_DOWN]->m_vPos		= pSaveCube->m_vPos;

			m_pItvXYZ[BP_R_ARM_DOWN]->m_wItvX		= pSaveCube->m_fItv_X;
			m_pItvXYZ[BP_R_ARM_DOWN]->m_wItvY		= pSaveCube->m_fItv_Y;
			m_pItvXYZ[BP_R_ARM_DOWN]->m_wItvZ		= pSaveCube->m_fItv_Z;
			break;

		case ENGINE::SOBJ_L_LEG:
			m_pInfo[BP_L_LEG]->m_fAngle[ENGINE::ANGLE_X]	= pSaveCube->m_fAngle[ENGINE::ANGLE_X];
			m_pInfo[BP_L_LEG]->m_fAngle[ENGINE::ANGLE_Y]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Y];
			m_pInfo[BP_L_LEG]->m_fAngle[ENGINE::ANGLE_Z]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Z];
			m_pInfo[BP_L_LEG]->m_matWorld	= pSaveCube->m_matWorld;
			m_pInfo[BP_L_LEG]->m_vDir		= pSaveCube->m_vDir;
			m_pInfo[BP_L_LEG]->m_vPos		= pSaveCube->m_vPos;

			m_pItvXYZ[BP_L_LEG]->m_wItvX		= pSaveCube->m_fItv_X;
			m_pItvXYZ[BP_L_LEG]->m_wItvY		= pSaveCube->m_fItv_Y;
			m_pItvXYZ[BP_L_LEG]->m_wItvZ		= pSaveCube->m_fItv_Z;
			break;

		case ENGINE::SOBJ_R_LEG:
			m_pInfo[BP_R_LEG]->m_fAngle[ENGINE::ANGLE_X]	= pSaveCube->m_fAngle[ENGINE::ANGLE_X];
			m_pInfo[BP_R_LEG]->m_fAngle[ENGINE::ANGLE_Y]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Y];
			m_pInfo[BP_R_LEG]->m_fAngle[ENGINE::ANGLE_Z]	= pSaveCube->m_fAngle[ENGINE::ANGLE_Z];
			m_pInfo[BP_R_LEG]->m_matWorld	= pSaveCube->m_matWorld;
			m_pInfo[BP_R_LEG]->m_vDir		= pSaveCube->m_vDir;
			m_pInfo[BP_R_LEG]->m_vPos		= pSaveCube->m_vPos;

			m_pItvXYZ[BP_R_LEG]->m_wItvX		= pSaveCube->m_fItv_X;
			m_pItvXYZ[BP_R_LEG]->m_wItvY		= pSaveCube->m_fItv_Y;
			m_pItvXYZ[BP_R_LEG]->m_wItvZ		= pSaveCube->m_fItv_Z;
			break;
		}
	}

	FAILED_CHECK(AddComponent());

	return S_OK;
}

void CPlayer::Update(void)
{
	//KeyInput();
	SetDirection();

	/*if(m_bMove)
		Move();*/

	ENGINE::CGameObject::Update();	

	SetTransform();
}

void CPlayer::Render(void)
{
	m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Normal_Cube", m_pConvertVertex);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pTexture->Render(0);
	m_pBuffer->Render();
}

void CPlayer::Release(void)
{
	if(m_pVertex != NULL)
	{
		ENGINE::Safe_Delete_Array(m_pVertex);
	}

	if(m_pConvertVertex != NULL)
	{
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
	}

	ENGINE::Safe_Delete(m_pCameraObserver);
}


void CPlayer::KeyInput(void)
{
	m_fTime = m_pTimeMgr->GetTime();

	/*if(GetAsyncKeyState('W') & 0x8000)
	{
		m_pInfo->m_vPos += m_pInfo->m_vDir * m_fSpeed * m_fTime;
	}

	if(GetAsyncKeyState('S') & 0x8000)
	{
		m_pInfo->m_vPos -= m_pInfo->m_vDir * m_fSpeed * m_fTime;
	}

	if(GetAsyncKeyState('A') & 0x8000)
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_Y] += D3DXToRadian(90.f) * m_fTime;
	}

	if(GetAsyncKeyState('D') & 0x8000)
	{
		m_pInfo->m_fAngle[ENGINE::ANGLE_Y] -= D3DXToRadian(90.f) * m_fTime;
	}

	if(GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_fCameraAngle += D3DXToRadian(45.f) * m_fTime;
	}

	if(GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_fCameraAngle -= D3DXToRadian(45.f) * m_fTime;
	}

	if(GetAsyncKeyState('O') & 0x8000)
	{
		m_fDistance += m_fSpeed * m_fTime;
	}

	if(GetAsyncKeyState('P') & 0x8000)
	{
		m_fDistance -= m_fSpeed * m_fTime;
	}*/

}

void CPlayer::SetDirection(void)
{
	for(int i=0; i<BP_END; ++i)
	{
		D3DXVec3TransformNormal(&m_pInfo[i]->m_vDir, &ENGINE::g_vLook, &m_pInfo[i]->m_matWorld);
		D3DXVec3Normalize(&m_pInfo[i]->m_vDir, &m_pInfo[i]->m_vDir);
	}
}

HRESULT CPlayer::AddComponent(void)
{
	m_fAngle = 0.f;

	ENGINE::CComponent*		pComponent = NULL;

	// Transform
	//pComponent = m_pInfo = ENGINE::CTransform::Create(g_vLook);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	for(int i=0; i<BP_END; ++i)
	{
		pComponent = m_pInfo[i];
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));
	}


	// Buffer
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Normal_Cube");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));


	// Texture
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture SkyBox");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

void CPlayer::SetTransform(void)
{
	const D3DXMATRIX*		pMatView = m_pCameraObserver->GetView();
	NULL_CHECK(pMatView);

	const D3DXMATRIX*		pMatProj = m_pCameraObserver->GetProj();
	NULL_CHECK(pMatProj);

	for(int j=0; j<BP_END; ++j)
	{
		for(size_t i = 0; i < m_dwVtxCnt; ++i)
		{
			m_pConvertVertex[i] = m_pVertex[i];

			//X
			if(i == 0 || i == 4 || i == 7 || i == 3)
			{
				m_pConvertVertex[i].vPos.x -= m_pItvXYZ[j]->m_wItvX;
			}
			else
			{
				m_pConvertVertex[i].vPos.x += m_pItvXYZ[j]->m_wItvX;
			}

			//YÃà
			if(i == 0 || i == 4 || i == 5 || i == 1)
			{
				m_pConvertVertex[i].vPos.y += m_pItvXYZ[j]->m_wItvY;
			}
			else
			{
				m_pConvertVertex[i].vPos.y -= m_pItvXYZ[j]->m_wItvY;
			}

			//ZÃà
			if(i >= 0 && i < 4)
			{
				m_pConvertVertex[i].vPos.z -= m_pItvXYZ[j]->m_wItvZ;
			}
			else
			{
				m_pConvertVertex[i].vPos.z += m_pItvXYZ[j]->m_wItvZ;
			}

			ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo[j]->m_matWorld);
			ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pMatView);

			if(m_pConvertVertex[i].vPos.z < 1.f)
				m_pConvertVertex[i].vPos.z = 1.f;

			ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pMatProj);

		}
	}
}

void CPlayer::Move(void)
{
	D3DXVECTOR3	vDir;

	for(int i=0; i<BP_END; ++i)
	{
		vDir = m_vDestPos - m_pInfo[i]->m_vPos;
		float fDistance = D3DXVec3Length(&vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		m_pInfo[i]->m_vPos += vDir * m_fSpeed * m_pTimeMgr->GetTime();

		if(fDistance < 1.f)
			m_bMove = false;
	}
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer*		pPlayer = new CPlayer(pDevice);

	if(FAILED(pPlayer->Initialize()))
		ENGINE::Safe_Delete(pPlayer);

	return pPlayer;
}




