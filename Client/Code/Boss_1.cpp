#include "stdafx.h"
#include "Boss_1.h"

#include "Head.h"
#include "RightLeg.h"
#include "LeftLeg.h"
#include "RightArm_Down.h"
#include "RightArm_Up.h"
#include "LeftArm_Down.h"
#include "LeftArm_Up.h"
#include "Body.h"
#include "CollisionMgr.h"
#include "TerrainCol.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Sword.h"
#include "BossApearParticle.h"
#include "Player_Elond.h"
#include "Player_Isildur.h"
#include "HitParticle.h"
#include "UnitCol.h"
#include "SoundMgr.h"

CBoss_1::CBoss_1(LPDIRECT3DDEVICE9 pDevice)
: CUnit(pDevice)
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL), m_pTerrainVertex(NULL), m_pSword(NULL), m_pUnitCol(NULL)
, m_bArmIdle(false), m_bHeadIdle(false), m_bBodyIdle(false), m_bLegIdle(false), m_bSwordIdle(false)
, m_bLeg_MoveUp(false), m_bMove_HandShake(false), m_bMove_LegWalk(false), m_bSwordAttack(false)
, m_bCollision(true), m_bAttackBtn(false), m_bSkillUse(false), m_bSwordMove(false), m_bCrush(false)
, m_bAppear(false), m_bAppearScene(false), m_bFrontWalk(true)
, m_fLeftLeg_AngleX(0), m_fRightLeg_AngleX(0), m_fLeftArm_AngleX(0.f), m_fRightArm_AngleX(0.f)
, m_fAttackArmAngle(0.f), m_fSkillTime(0.f), m_fSwordMove(0.f), m_fCrushTime(0.f), m_Gravity(0.f)
, m_fActionMove(50.f)
, MonDestination(false), m_fTime(0.f), m_ElondAtt(false), m_iHp(30), m_fAttTime(0.f), m_fBossJumpTime(0.f)
, m_bJumpTiming(false), m_bSpeedSkill(false), m_IsildurAtt(false)
{
	m_pDyingBodyPart[ENGINE::BP_BODY] = CBody::Create(pDevice, CBody::UT_BOSS);
	m_pDyingBodyPart[ENGINE::BP_HEAD] = CHead::Create(pDevice, CHead::UT_BOSS);
	m_pDyingBodyPart[ENGINE::BP_R_ARM_UP] = CRightArm_Up::Create(pDevice, CRightArm_Up::UT_BOSS);
	m_pDyingBodyPart[ENGINE::BP_R_ARM_DOWN] = CRightArm_Down::Create(pDevice, CRightArm_Down::UT_BOSS);
	m_pDyingBodyPart[ENGINE::BP_L_ARM_UP] = CLeftArm_Up::Create(pDevice, CLeftArm_Up::UT_BOSS);
	m_pDyingBodyPart[ENGINE::BP_L_ARM_DOWN] = CLeftArm_Down::Create(pDevice, CLeftArm_Down::UT_BOSS);
	m_pDyingBodyPart[ENGINE::BP_R_LEG] = CRightLeg::Create(pDevice, CRightLeg::UT_BOSS);
	m_pDyingBodyPart[ENGINE::BP_L_LEG] = CLeftLeg::Create(pDevice, CLeftLeg::UT_BOSS);
}

CBoss_1::~CBoss_1(void)
{
	Release();
}

HRESULT CBoss_1::Initialize(void)
{
	m_fSpeed = 10.f;
	TCHAR szFilePath[MAX_PATH] = L"../../Data/Object_Data/OriginBody.dat";
	HANDLE	hFile	= CreateFile(szFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwByte	= 0;

	m_iHp = 10;

	while(true)
	{
		ENGINE::SAVECUBE* pSaveCube = new ENGINE::SAVECUBE;
		ReadFile(hFile, pSaveCube, sizeof(ENGINE::SAVECUBE), &dwByte, NULL);

		if(dwByte == 0)
		{
			ENGINE::Safe_Delete(pSaveCube);
			break;
		}
		CreateBodyPart(pSaveCube);
		ENGINE::Safe_Delete(pSaveCube);
	}

	m_pSword = CSword::Create(m_pDevice, CSword::UT_BOSS);

	CloseHandle(hFile);

	//Ani_SkillUse();

	FAILED_CHECK(AddComponent());

	D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], D3DXToRadian(90.f));

	return S_OK;
}

HRESULT CBoss_1::AddComponent(void)
{
	CUnit::AddComponent();

	ENGINE::CComponent*		pComponent = NULL;

	pComponent = m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_TERRAIN);
	m_pTerrainCol = dynamic_cast<CTerrainCol*>(pComponent);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));

	return S_OK;
}

void CBoss_1::Update(void)
{
	if(m_bAppear)
	{
		CUnit::Update();

		float m_fTime = m_pTimeMgr->GetTime();

		if(m_iHp <= 0)
		{
			m_pSoundMgr->MonDie(L"Mondie.wav");

			m_bCrush = true;
			m_fCrushTime = 0.f;

			for(int i = 0; i < ENGINE::BP_END; ++i)
			{
				m_pDyingBodyPart[i]->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = m_pBodyPart[i]->GetInfo()->m_fAngle[ENGINE::ANGLE_X];
				m_pDyingBodyPart[i]->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = m_pBodyPart[i]->GetInfo()->m_fAngle[ENGINE::ANGLE_Y];
				m_pDyingBodyPart[i]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = m_pBodyPart[i]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z];

				m_pDyingBodyPart[i]->GetInfo()->m_matProj = m_pBodyPart[i]->GetInfo()->m_matProj;
				m_pDyingBodyPart[i]->GetInfo()->m_matView = m_pBodyPart[i]->GetInfo()->m_matView;
				m_pDyingBodyPart[i]->GetInfo()->m_vPos = m_pBodyPart[i]->GetInfo()->m_vPos;
				m_pDyingBodyPart[i]->GetInfo()->m_vDir = m_pBodyPart[i]->GetInfo()->m_vDir;
				m_pDyingBodyPart[i]->GetInfo()->m_matWorld = m_pBodyPart[i]->GetInfo()->m_matWorld;

				m_matParent = m_mMat[ENGINE::MAT_PARENT];
				m_vecDir	= m_pInfo->m_vDir;
			}

			m_pInfo->m_vPos = D3DXVECTOR3(9999.f, 9999.f, 9999.f);
			m_iHp = 100;
		}

		if(!m_pTerrainCol->GetCollision())
		{
			m_Gravity += m_fTime;

			m_pInfo->m_vPos.y -= 0.098f * m_Gravity * m_Gravity * 5.f;
		}

		ENGINE::CGameObject* pTerrain = m_pLayerMgr->GetGameObject(L"Terrain", ENGINE::LAYER_ENVIRONMENT);
		m_pTerrainVertex = dynamic_cast<CTerrain*>(pTerrain)->GetTerrainVertex();
		m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex, dynamic_cast<CTerrain*>(pTerrain));

		if(m_bAppearScene == false)
		{
			m_Gravity += m_fTime;

			m_fActionMove -= 0.098f * m_Gravity * m_Gravity * 4.f;

			m_pInfo->m_vPos.y += m_fActionMove;
			m_bJumpTiming = true;
			//m_bSpeedSkill = true;


			if(m_fActionMove < 10.f)
			{
				m_pSoundMgr->BossSound(L"BossSound.wav");
				ENGINE::CGameObject* pGameObject = CBossApearParticle::Create(m_pDevice, D3DXVECTOR3(200.f, 0.f, 30.f));
				m_pLayerMgr->AddObject(L"BossApearParticle",pGameObject, ENGINE::LAYER_GAMELOGIC);
				pGameObject = CBossApearParticle::Create(m_pDevice, D3DXVECTOR3(200.f, 0.f, 70.f));
				m_pLayerMgr->AddObject(L"BossApearParticle",pGameObject, ENGINE::LAYER_GAMELOGIC);

				*(m_pCameraMgr->GetBossShake()) = true;
				m_bAppearScene = true;
				m_bFrontWalk = false;
				m_Gravity = 0.f;
			}
		}

		if(m_bFrontWalk == false)
		{
			m_eUnitStateType = ENGINE::US_MOVE;

			m_Gravity += m_fTime;			 

			m_pInfo->m_vPos.x -= 0.098f * m_Gravity * m_Gravity * 2.f;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(90.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();

			if(m_pInfo->m_vPos.x < 130.f)
			{
				m_bFrontWalk = true;
				m_bLeg_MoveUp = false;
				m_eUnitStateType = ENGINE::US_IDLE;
				Ani_Idle();
			}
		}

		if(m_bAppearScene)
			m_pInfo->m_vPos.y += 10.f;

		if(m_bAppearScene && m_bFrontWalk)
		{
			AI_01();
		}

		Jump();		
		BodyAnimation();

		if(m_IsildurAtt)
			IsildurAtt();		

		if(m_ElondAtt)	
			ElondAtt();

		if(m_bAttackBtn)
			Ani_Attack();

		if(m_bCrush)
		{
			m_pTerrainCol->GetJumpAble() = true;
			m_pTerrainCol->GetCollision() = false;
			BoomCrush();
		}

		D3DXMatrixTranslation(&m_mMat[ENGINE::MAT_TRANS], m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

		m_mMat[ENGINE::MAT_PARENT] =	m_mMat[ENGINE::MAT_SCALE] *  
										m_mMat[ENGINE::MAT_ROT_X] * 
										m_mMat[ENGINE::MAT_ROT_Y] * 
										m_mMat[ENGINE::MAT_ROT_Z] * 
										m_mMat[ENGINE::MAT_TRANS] * 
										m_mMat[ENGINE::MAT_AXIS];

		for(int i=0; i<ENGINE::BP_END; ++i)
		{
			(dynamic_cast<CCubeObject*>(m_pBodyPart[i])->GetMatrix())[ENGINE::MAT_PARENT] = m_mMat[ENGINE::MAT_PARENT];
		}

		(dynamic_cast<CCubeObject*>(m_pSword)->GetMatrix())[ENGINE::MAT_PARENT] = m_mMat[ENGINE::MAT_PARENT];

		for(int i=0; i<ENGINE::BP_END; ++i)
			m_pBodyPart[i]->Update();

		if(m_bCrush)
		{
			for(int i=0; i<ENGINE::BP_END; ++i)
			{
				(dynamic_cast<CCubeObject*>(m_pDyingBodyPart[i])->GetMatrix())[ENGINE::MAT_PARENT] = m_mMat[ENGINE::MAT_PARENT];
			}

			for(int i=0; i<ENGINE::BP_END; ++i)
				m_pDyingBodyPart[i]->Update();
		}

		if(m_bAttackBtn == false && m_bCrush == false)
			Ani_Sword();

		m_pSword->Update();

		ENGINE::CGameObject::Update();

		m_eUnitStateType = ENGINE::US_IDLE;
	}
}

void CBoss_1::Render(void)
{
	if(m_bAppear)
	{
		for(int i=0; i<ENGINE::BP_END; ++i)
			m_pBodyPart[i]->Render();

		if(m_bCrush)
		{
			for(int i=0; i<ENGINE::BP_END; ++i)
				m_pDyingBodyPart[i]->Render();
		}

		m_pSword->Render();
	}	
}


void CBoss_1::Release(void)
{

}

void CBoss_1::CreateBodyPart(ENGINE::SAVECUBE* pSaveCube)
{
	switch(pSaveCube->m_SaveType)
	{
	case ENGINE::SOBJ_HEAD:
		m_pBodyPart[ENGINE::BP_HEAD] = CHead::Create(m_pDevice, CHead::UT_BOSS);
		m_pBodyPart[ENGINE::BP_HEAD]->SetInfo(pSaveCube);
		dynamic_cast<CHead*>(m_pBodyPart[ENGINE::BP_HEAD])->SetInterver(pSaveCube);
		dynamic_cast<CHead*>(m_pDyingBodyPart[ENGINE::BP_HEAD])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_BODY:
		m_pBodyPart[ENGINE::BP_BODY] = CBody::Create(m_pDevice, CBody::UT_BOSS);
		m_pBodyPart[ENGINE::BP_BODY]->SetInfo(pSaveCube);
		dynamic_cast<CBody*>(m_pBodyPart[ENGINE::BP_BODY])->SetInterver(pSaveCube);
		dynamic_cast<CBody*>(m_pDyingBodyPart[ENGINE::BP_BODY])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_L_ARM_UP:
		m_pBodyPart[ENGINE::BP_L_ARM_UP] = CLeftArm_Up::Create(m_pDevice, CLeftArm_Up::UT_BOSS);
		m_pBodyPart[ENGINE::BP_L_ARM_UP]->SetInfo(pSaveCube);
		dynamic_cast<CLeftArm_Up*>(m_pBodyPart[ENGINE::BP_L_ARM_UP])->SetInterver(pSaveCube);
		dynamic_cast<CLeftArm_Up*>(m_pDyingBodyPart[ENGINE::BP_L_ARM_UP])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_L_ARM_DOWN:
		m_pBodyPart[ENGINE::BP_L_ARM_DOWN] = CLeftArm_Down::Create(m_pDevice, CLeftArm_Down::UT_BOSS);		
		m_pBodyPart[ENGINE::BP_L_ARM_DOWN]->SetInfo(pSaveCube);
		dynamic_cast<CLeftArm_Down*>(m_pBodyPart[ENGINE::BP_L_ARM_DOWN])->SetInterver(pSaveCube);		
		dynamic_cast<CLeftArm_Down*>(m_pDyingBodyPart[ENGINE::BP_L_ARM_DOWN])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_R_ARM_UP:
		m_pBodyPart[ENGINE::BP_R_ARM_UP] = CRightArm_Up::Create(m_pDevice, CRightArm_Up::UT_BOSS);
		m_pBodyPart[ENGINE::BP_R_ARM_UP]->SetInfo(pSaveCube);
		dynamic_cast<CRightArm_Up*>(m_pBodyPart[ENGINE::BP_R_ARM_UP])->SetInterver(pSaveCube);
		dynamic_cast<CRightArm_Up*>(m_pDyingBodyPart[ENGINE::BP_R_ARM_UP])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_R_ARM_DOWN:
		m_pBodyPart[ENGINE::BP_R_ARM_DOWN] = CRightArm_Down::Create(m_pDevice, CRightArm_Down::UT_BOSS);
		m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->SetInfo(pSaveCube);
		dynamic_cast<CRightArm_Down*>(m_pBodyPart[ENGINE::BP_R_ARM_DOWN])->SetInterver(pSaveCube);
		dynamic_cast<CRightArm_Down*>(m_pDyingBodyPart[ENGINE::BP_R_ARM_DOWN])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_L_LEG:
		m_pBodyPart[ENGINE::BP_L_LEG] = CLeftLeg::Create(m_pDevice, CLeftLeg::UT_BOSS);
		m_pBodyPart[ENGINE::BP_L_LEG]->SetInfo(pSaveCube);
		dynamic_cast<CLeftLeg*>(m_pBodyPart[ENGINE::BP_L_LEG])->SetInterver(pSaveCube);
		dynamic_cast<CLeftLeg*>(m_pDyingBodyPart[ENGINE::BP_L_LEG])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_R_LEG:
		m_pBodyPart[ENGINE::BP_R_LEG] = CRightLeg::Create(m_pDevice, CRightLeg::UT_BOSS);
		m_pBodyPart[ENGINE::BP_R_LEG]->SetInfo(pSaveCube);
		dynamic_cast<CRightLeg*>(m_pBodyPart[ENGINE::BP_R_LEG])->SetInterver(pSaveCube);
		dynamic_cast<CRightLeg*>(m_pDyingBodyPart[ENGINE::BP_R_LEG])->SetInterver(pSaveCube);
		break;
	}
}

void CBoss_1::Ani_Idle(void)
{
	//오른, 왼팔 들썩.
	if(m_bArmIdle == false)
	{
		m_pBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_vPos.y += 0.0001f;
		m_pBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_vPos.y += 0.0001f;

		m_pBodyPart[ENGINE::BP_R_ARM_UP]->GetInfo()->m_vPos.y += 0.0001f;
		m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.y += 0.0001f;

		if((m_pBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_vPos.y + 12.f) >= (m_pInfo->m_vPos.y + 0.014f))
			m_bArmIdle = true;
	}
	else
	{
		m_pBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_vPos.y -= 0.0001f;
		m_pBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_vPos.y -= 0.0001f;

		m_pBodyPart[ENGINE::BP_R_ARM_UP]->GetInfo()->m_vPos.y -= 0.0001f;
		m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.y -= 0.0001f;

		if((m_pBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_vPos.y + 12.f) <= (m_pInfo->m_vPos.y))
			m_bArmIdle = false;
	}

	//오른, 왼팔 몸을 중심으로 공전
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(45.f));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(45.f));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(45.f));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(45.f));

	//머리
	if(m_bHeadIdle == false)
	{	
		m_pBodyPart[ENGINE::BP_HEAD]->GetInfo()->m_fAngle[ENGINE::ANGLE_X] += 0.001f;

		if(m_pBodyPart[ENGINE::BP_HEAD]->GetInfo()->m_fAngle[ENGINE::ANGLE_X] >= 0.1f)
			m_bHeadIdle = true;
	}
	else
	{
		m_pBodyPart[ENGINE::BP_HEAD]->GetInfo()->m_fAngle[ENGINE::ANGLE_X] -= 0.001f;

		if(m_pBodyPart[ENGINE::BP_HEAD]->GetInfo()->m_fAngle[ENGINE::ANGLE_X] <= -0.1f)
			m_bHeadIdle = false;
	}

	//몸통 회전, 들썩
	m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_fAngle[ENGINE::ANGLE_Y] = -45.f;

	if(m_bBodyIdle == false)
	{
		m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos.y += 0.00007f;
		m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos.y += 0.00007f;

		if((m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos.y + 12.f) >= (m_pInfo->m_vPos.y + 0.008f))
			m_bBodyIdle = true;
	}
	else
	{
		m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos.y -= 0.00007f;
		m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos.y -= 0.00007f;

		if((m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos.y + 12.f) <= (m_pInfo->m_vPos.y - 0.008f))
			m_bBodyIdle = false;
	}

	//왼, 오른 다리 벌리기.
	m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(-40.f);
	m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(40.f);

	if(m_bLegIdle == false)
	{		
		m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x -= 0.08f;
		m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x += 0.08f;

		m_bLegIdle = true;
		m_bLeg_MoveUp = false;
	}

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_LEG])->GetMatrix())[ENGINE::MAT_AXIS], &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(40.f));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_LEG])->GetMatrix())[ENGINE::MAT_AXIS], &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(40.f));

	if(m_bSwordIdle == false)
	{
		m_pSword->GetInfo()->m_vPos.y += 0.0001f;

		if((m_pSword->GetInfo()->m_vPos.y - 1.f) >= (m_pInfo->m_vPos.y + 0.05f))
			m_bSwordIdle = true;
	}
	else
	{
		m_pSword->GetInfo()->m_vPos.y -= 0.0001f;

		if((m_pSword->GetInfo()->m_vPos.y - 1.f) <= (m_pInfo->m_vPos.y - 0.05f))
			m_bSwordIdle = false;
	}	
}

void CBoss_1::Ani_Attack(void)
{
	D3DXVECTOR3 X_Cross;
	D3DXVec3Cross(&X_Cross, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);

	X_Cross.y += 0.5f;
	X_Cross.x += 1.5f;

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fAttackArmAngle));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fAttackArmAngle));

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pSword)->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fAttackArmAngle-55.f));

	m_fAttackArmAngle -= 10.f;

	if(m_fAttackArmAngle <= -90.f)
	{
		m_fAttackArmAngle = 180.f;
		m_bAttackBtn = false;
	}
}

void CBoss_1::Ani_LegWalk(void)
{
	D3DXVECTOR3 X_Cross;
	D3DXVec3Cross(&X_Cross, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir );

	if(m_bMove_LegWalk)
	{
		m_fLeftLeg_AngleX += 4.f;
		m_fRightLeg_AngleX -= 4.f;

		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_LEG])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fLeftLeg_AngleX)); 
		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_LEG])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fRightLeg_AngleX));

		if(m_fLeftLeg_AngleX >= 50.f)
			m_bMove_LegWalk = false;
	}
	else
	{
		m_fLeftLeg_AngleX -= 4.f;
		m_fRightLeg_AngleX += 4.f;

		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_LEG])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fLeftLeg_AngleX)); 
		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_LEG])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fRightLeg_AngleX));

		if(m_fLeftLeg_AngleX <= -50.f)
			m_bMove_LegWalk = true;
	}
}

void CBoss_1::Ani_WalkHandShake(void)
{
	D3DXVECTOR3 X_Cross;
	D3DXVec3Cross(&X_Cross, &D3DXVECTOR3(0.f, -1.f, 0.f), &m_pInfo->m_vDir);

	if(m_bMove_HandShake)
	{
		m_fLeftArm_AngleX += 4.f;
		m_fRightArm_AngleX -= 4.f;

		X_Cross.y += 0.5f;
		X_Cross.x -= 1.5f;

		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fLeftArm_AngleX)); 
		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fLeftArm_AngleX)); 

		X_Cross.y -= 0.5f;
		X_Cross.x += 1.5f;

		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fRightArm_AngleX));
		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fRightArm_AngleX));

		if(m_fLeftArm_AngleX >= 50.f)
			m_bMove_HandShake = false;
	}
	else
	{		
		m_fLeftArm_AngleX -= 4.f;
		m_fRightArm_AngleX += 4.f;

		X_Cross.y += 0.5f;
		X_Cross.x -= 1.5f;

		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fLeftArm_AngleX)); 
		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fLeftArm_AngleX)); 

		X_Cross.y -= 0.5f;
		X_Cross.x += 1.5f;

		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fRightArm_AngleX));
		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fRightArm_AngleX));

		if(m_fLeftArm_AngleX <= -50.f)
			m_bMove_HandShake = true;
	}
}

void CBoss_1::Ani_Sword(void)
{
	switch(m_eUnitStateType)
	{
	case ENGINE::US_IDLE:
		{
			D3DXVECTOR3 X_Cross;
			D3DXVec3Cross(&X_Cross, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, -1.f, 0.f));

			D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pSword)->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(-55.f));

			m_pSword->GetInfo()->m_vPos = D3DXVECTOR3(m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.x + 0.4f,
				m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.y + 0.5f,
				m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.z - 0.5f);
		}
		break;

	case ENGINE::US_MOVE:
		{
			D3DXVECTOR3 X_Cross;
			D3DXVec3Cross(&X_Cross, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, -1.f, 0.f));

			D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pSword)->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(-85.f));

			m_pSword->GetInfo()->m_vPos = D3DXVECTOR3(m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.x + 1.5f,
														m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.y + 0.2f,
														m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.z - 0.5f);

			if(m_bMove_HandShake)
			{
				m_pSword->GetInfo()->m_vPos.y += (m_fSwordMove += 0.5f);
			}
			else
			{
				m_pSword->GetInfo()->m_vPos.y += (m_fSwordMove -= 0.5f);
			}
		}
		break;
	}
}

CBoss_1* CBoss_1::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CBoss_1* pBoss_1 = new CBoss_1(pDevice);

	if(FAILED(pBoss_1->Initialize()))
		ENGINE::Safe_Delete(pBoss_1);

	return pBoss_1;
}

void CBoss_1::BodyAnimation(void)
{
	switch(m_eUnitStateType)
	{
	case ENGINE::US_IDLE:
		Ani_Idle();
		break;

	case ENGINE::US_END:
		break;
	}
}

void CBoss_1::BoomCrush(void)
{
	m_fCrushTime += 1.f * m_pTimeMgr->GetTime();

	for(int i = 0; i < ENGINE::BP_END; ++i)
	{
		if(m_pTerrainCol->GetJumpAble())
		{
			m_pDyingBodyPart[i]->GetInfo()->m_vPos.y -= 0.098f * m_Gravity * 0.5f;
		}
	}

	//오른팔 어깨
	m_pDyingBodyPart[ENGINE::BP_R_ARM_UP]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = 85.f;
	D3DXVec3Cross(&m_pDyingBodyPart[ENGINE::BP_R_ARM_UP]->GetInfo()->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
	//오른팔 손
	m_pDyingBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = 85.f;
	//왼팔 어깨
	m_pDyingBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = -85.f;
	D3DXVec3Cross(&m_pDyingBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_vDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
	//왼팔 손
	m_pDyingBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = -85.f;
	D3DXVECTOR3 vRight;
	D3DXVec3Cross(&vRight, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
	D3DXVec3Cross(&m_pDyingBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &vRight);
	//머리
	D3DXVec3Cross(&vRight, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
	D3DXVec3Cross(&m_pDyingBodyPart[ENGINE::BP_HEAD]->GetInfo()->m_vDir, &m_pInfo->m_vDir, &vRight);
	//몸통
	//오른다리
	m_pDyingBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = -85.f;
	D3DXVec3Cross(&m_pDyingBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
	//왼다리
	m_pDyingBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = 85.f;
	D3DXVec3Cross(&m_pDyingBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);

	for(int i = 0; i < ENGINE::BP_END; ++i)
	{
		D3DXVec3Normalize(&m_pDyingBodyPart[i]->GetInfo()->m_vDir, &m_pDyingBodyPart[i]->GetInfo()->m_vDir);
	}

	m_pDyingBodyPart[ENGINE::BP_R_ARM_UP]->GetInfo()->m_vPos = m_pDyingBodyPart[ENGINE::BP_R_ARM_UP]->GetInfo()->m_vDir * m_Gravity * 1.5f;
	m_pDyingBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos = m_pInfo->m_vDir * m_Gravity * 1.5f;
	m_pDyingBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_vPos = m_pDyingBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_vDir * m_Gravity * 1.5f;
	m_pDyingBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_vPos = m_pDyingBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_vDir * m_Gravity * 1.5f;
	m_pDyingBodyPart[ENGINE::BP_HEAD]->GetInfo()->m_vPos = m_pDyingBodyPart[ENGINE::BP_HEAD]->GetInfo()->m_vDir * m_Gravity * 0.5f;
	m_pDyingBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos = m_pInfo->m_vDir * m_Gravity * 1.2f;
	m_pDyingBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos = m_pDyingBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vDir * m_Gravity * 1.2f;
	m_pDyingBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos = m_pDyingBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vDir * m_Gravity * 1.2f;

	if(m_fCrushTime > 1.f)
	{
		m_bCrush = false;
		m_pTerrainCol->GetJumpAble() = false;
	}
}

void CBoss_1::AI_01(void)
{
	float fX = ((CPlayer_Isildur*)(m_pLayerMgr->GetGameObject(L"Player_Isildur", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos.x - m_pInfo->m_vPos.x;
	float fY = ((CPlayer_Isildur*)(m_pLayerMgr->GetGameObject(L"Player_Isildur", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos.y - m_pInfo->m_vPos.y;
	float fZ = ((CPlayer_Isildur*)(m_pLayerMgr->GetGameObject(L"Player_Isildur", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos.z - m_pInfo->m_vPos.z;

	float fDistance = sqrtf(fX * fX + fY * fY + fZ * fZ);

	float fX2 = ((CPlayer_Isildur*)(m_pLayerMgr->GetGameObject(L"Player_Elond", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos.x - m_pInfo->m_vPos.x;
	float fY2 = ((CPlayer_Isildur*)(m_pLayerMgr->GetGameObject(L"Player_Elond", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos.y - m_pInfo->m_vPos.y;
	float fZ2 = ((CPlayer_Isildur*)(m_pLayerMgr->GetGameObject(L"Player_Elond", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos.z - m_pInfo->m_vPos.z;

	float fDistance2 = sqrtf(fX2 * fX2 + fY2 * fY2 + fZ2 * fZ2);

	if(fDistance < fDistance2)
	{
		//이실두르쫓아가느넉 구현
		if(MonDestination == false)
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			D3DXVECTOR3 vDirection = ((CPlayer_Isildur*)(m_pLayerMgr->GetGameObject(L"Player_Isildur", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos - m_pInfo->m_vPos;

			D3DXVec3Normalize(&vDirection, &vDirection);

			float fAngle = D3DXVec3Dot(&m_pInfo->m_vDir,  &vDirection);

			float fCosAngle = acosf(-fAngle);

			if(m_pInfo->m_vPos.x < ((CPlayer_Isildur*)(m_pLayerMgr->GetGameObject(L"Player_Isildur", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos.x)
				fCosAngle = 2 * D3DX_PI - fCosAngle;

			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], fCosAngle);

			if(fDistance < 50.f)
			{
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);

				if(m_bLeg_MoveUp == false)
				{
					m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
					m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

					m_bLeg_MoveUp = true;
					m_bLegIdle = false;
				}

				Ani_LegWalk();

				if(m_bAttackBtn == false)
					Ani_WalkHandShake();

				//if(!m_bSpeedSkill)
				//{
					m_pInfo->m_vPos.x +=  fX * 0.005f;
					m_pInfo->m_vPos.y +=  fY * 0.005f;
					m_pInfo->m_vPos.z +=  fZ * 0.005f;

					m_pInfo->m_vDir = vDirection;
				//}

				if(fDistance <= 15.f)
				{
					MonDestination = true;
				}

				if(m_bJumpTiming)
				{
					m_fBossJumpTime += 1.f * m_pTimeMgr->GetTime();

					if(m_fBossJumpTime > 5.f)
					{
						m_pSoundMgr->BossSound(L"BossSkillSound.wav");
						m_bSpeedSkill = true;
					}

					//cout << "점멸 시간 : " << m_fBossJumpTime << endl;
				}

				if(m_bSpeedSkill)
				{
					m_pInfo->m_vPos.x +=  fX * 0.1f;
					m_pInfo->m_vPos.y +=  fY * 0.1f;
					m_pInfo->m_vPos.z +=  fZ * 0.1f;

					m_pInfo->m_vDir = vDirection;

					m_bSpeedSkill = false;
					m_fBossJumpTime = 0.f;
				}
			}
			//else
			//{
			//	//보스가 나오고 나서 타임이 돌아가야함.
			//	if(m_bJumpTiming)
			//	{
			//		m_fBossJumpTime += 1.f * m_pTimeMgr->GetTime();

			//		if(m_fBossJumpTime > 5.f)
			//		{
			//			m_bSpeedSkill = true;
			//		}
			//		
			//		cout << "점멸 시간 : " << m_fBossJumpTime << endl;

			//	}
			//	
			//	if(m_bSpeedSkill)
			//	{
			//		m_pInfo->m_vPos.x +=  fX * 0.4f;
			//		m_pInfo->m_vPos.y +=  fY * 0.4f;
			//		m_pInfo->m_vPos.z +=  fZ * 0.4f;

			//		m_pInfo->m_vDir = vDirection;

			//		m_bSpeedSkill = false;
			//		m_fBossJumpTime = 0.f;
			//	}
			//	
			//}		
		}
		else
		{	
			D3DXVECTOR3 vDirection = ((CPlayer_Isildur*)(m_pLayerMgr->GetGameObject(L"Player_Isildur", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos - m_pInfo->m_vPos;

			D3DXVec3Normalize(&vDirection, &vDirection);

			float fAngle = D3DXVec3Dot(&m_pInfo->m_vDir,  &vDirection);

			float fCosAngle = acosf(-fAngle);

			if(m_pInfo->m_vPos.x < ((CPlayer_Isildur*)(m_pLayerMgr->GetGameObject(L"Player_Isildur", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos.x)
				fCosAngle = 2 * D3DX_PI - fCosAngle;

			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], fCosAngle);

			if(fDistance > 15.f)
				MonDestination = false;

			m_fTime += 1.f * m_pTimeMgr->GetTime();

			if(m_fTime > 1.5f)
			{
				m_IsildurAtt = true;
				m_bAttackBtn = true;
				m_fTime = 0.f;
			}
		}
	}
	else
	{
		//엘론드 쫓아가는거 구현
		if(MonDestination == false)
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			D3DXVECTOR3 vDirection = ((CPlayer_Elond*)(m_pLayerMgr->GetGameObject(L"Player_Elond", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos - m_pInfo->m_vPos;

			D3DXVec3Normalize(&vDirection, &vDirection);

			float fAngle = D3DXVec3Dot(&m_pInfo->m_vDir,  &vDirection);

			float fCosAngle = acosf(-fAngle);

			if(m_pInfo->m_vPos.x < ((CPlayer_Elond*)(m_pLayerMgr->GetGameObject(L"Player_Elond", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos.x)
				fCosAngle = 2 * D3DX_PI - fCosAngle;

			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], fCosAngle);			

			if(fDistance2 < 50.f)
			{
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);

				if(m_bLeg_MoveUp == false)
				{
					m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
					m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

					m_bLeg_MoveUp = true;
					m_bLegIdle = false;
				}

				Ani_LegWalk();

				if(m_bAttackBtn == false)
					Ani_WalkHandShake();

				m_pInfo->m_vPos.x +=  fX2 * 0.005f;
				m_pInfo->m_vPos.y +=  fY2 * 0.005f;
				m_pInfo->m_vPos.z +=  fZ2 * 0.005f;

				m_pInfo->m_vDir = vDirection;

				if(fDistance2 <= 15.f)
				{
					MonDestination = true;
				}
			}
			else
			{
				//보스가 나오고 나서 타임이 돌아가야함.
				if(m_bJumpTiming)
				{
					m_fBossJumpTime += 1.f * m_pTimeMgr->GetTime();

					if(m_fBossJumpTime > 5.f)
					{
						m_pSoundMgr->BossSound(L"BossSkillSound.wav");
						m_bSpeedSkill = true;
					}

					//fDistance = 300.f;
					//cout << "점멸 시간 : " << m_fBossJumpTime << endl;

				}
				//손머리위로 올리면... 
				if(m_bSpeedSkill)
				{
					m_pInfo->m_vPos.x +=  fX * 0.6f;
					m_pInfo->m_vPos.y +=  fY * 0.6f;
					m_pInfo->m_vPos.z +=  fZ * 0.6f;

					m_pInfo->m_vDir = vDirection;
					//여기서 파티클 생성하시면 될 거 같습니당.
					m_bSpeedSkill = false;
					m_fBossJumpTime = 0.f;
				}
			}
		}
		else
		{	
			D3DXVECTOR3 vDirection = ((CPlayer_Elond*)(m_pLayerMgr->GetGameObject(L"Player_Elond", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos - m_pInfo->m_vPos;

			D3DXVec3Normalize(&vDirection, &vDirection);

			float fAngle = D3DXVec3Dot(&m_pInfo->m_vDir,  &vDirection);

			float fCosAngle = acosf(-fAngle);

			if(m_pInfo->m_vPos.x < ((CPlayer_Elond*)(m_pLayerMgr->GetGameObject(L"Player_Elond", ENGINE::LAYER_PLAYER)))->GetInfo()->m_vPos.x)
				fCosAngle = 2 * D3DX_PI - fCosAngle;

			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], fCosAngle);

			if(fDistance2 > 15.f)
				MonDestination = false;

			m_fTime += 1.f * m_pTimeMgr->GetTime();

			if(m_fTime > 1.5f)
			{				
				m_ElondAtt = true;
				m_bAttackBtn = true;
				m_fTime = 0.f;
			}
		}
	}

}

void CBoss_1::ElondAtt(void)
{
	list<ENGINE::CGameObject*>::iterator iter_Elond = m_pLayerMgr->GetObjList(L"Player_Elond", ENGINE::LAYER_PLAYER)->begin();

	m_pUnitCol->SetColInfo(&m_pInfo->m_vPos, &((CPlayer_Elond*)(*iter_Elond))->GetInfo()->m_vPos/*공격하는 상대의 포스*/);

	m_pSoundMgr->BossAtt(L"HyunChul.wav");
	ENGINE::CGameObject* pGameObject = CHitParticle::Create(m_pDevice, ((CPlayer_Elond*)(*iter_Elond))->GetInfo()->m_vPos);
	ENGINE::CLayerMgr::GetInstance()->AddObject(L"HitParticle", pGameObject, ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CHitParticle*>(pGameObject)->SetAni();

	m_ElondAtt = false;

}

void CBoss_1::IsildurAtt(void)
{
	list<ENGINE::CGameObject*>::iterator iter_Isildur = m_pLayerMgr->GetObjList(L"Player_Isildur", ENGINE::LAYER_PLAYER)->begin();

	m_pUnitCol->SetColInfo(&m_pInfo->m_vPos, &((CPlayer_Isildur*)(*iter_Isildur))->GetInfo()->m_vPos/*공격하는 상대의 포스*/);

	m_pSoundMgr->BossAtt(L"HyunChul.wav");
	ENGINE::CGameObject* pGameObject = CHitParticle::Create(m_pDevice, ((CPlayer_Isildur*)(*iter_Isildur))->GetInfo()->m_vPos);
	ENGINE::CLayerMgr::GetInstance()->AddObject(L"HitParticle", pGameObject, ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CHitParticle*>(pGameObject)->SetAni();

	m_IsildurAtt = false;
}

void CBoss_1::Ani_SkillUse(void)
{	
	D3DXVECTOR3 X_Cross;
	D3DXVec3Cross(&X_Cross, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);

	X_Cross.y += 0.5f;
	X_Cross.x += 2.f;

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(160.f));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(160.f));

	D3DXVECTOR3 Y_Cross;
	D3DXVec3Cross(&Y_Cross, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);

	Y_Cross.y -= 0.5f;
	Y_Cross.x += 2.f;

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &Y_Cross, D3DXToRadian(160.f));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &Y_Cross, D3DXToRadian(160.f));

	m_fSkillTime += 1.f * m_pTimeMgr->GetTime();

	if(m_fSkillTime > 1.f)
	{
		m_bSkillUse = true;
		m_fSkillTime = 0.f;
		//m_fSkillUseTime = 0.f;
	}

}

void CBoss_1::Jump(void)
{
	if(m_pTerrainCol->GetJumpAble() == true)
	{
		m_pTerrainCol->GetCollision() = false;
		m_pInfo->m_vPos.y += 0.098 * m_Gravity * 2.f;
	}

	if(m_pInfo->m_vPos.y < 0.f)
	{
		m_pTerrainCol->GetCollision() = true;
		m_pInfo->m_vPos.y = 0.f;
		m_Gravity = 0;
		m_pTerrainCol->GetJumpAble() = false;
	}
}

