#include "stdafx.h"
#include "Player_Elond.h"
#include "Transform.h"

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
#include "Player2Skill.h"
#include "Player2Skill2.h"
#include "..\Engine\Utility\code\GameObject.h"
#include "LayerMgr.h"
#include "Metheo.h"
#include "Player2Skill2.h"
#include "Heart.h"
#include "Lhpclass.h"
#include "Heart2.h"
#include "Sword.h"
#include "UnitCol.h"
#include "Monster_Orc.h"
#include "HitParticle.h"
#include "Boss_1.h"



//장풍스킬, 점프스킬
CPlayer_Elond::CPlayer_Elond(LPDIRECT3DDEVICE9 pDevice)
: CUnit(pDevice)
, m_pLayerMgr(ENGINE::Get_LayerMgr())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL), m_pTerrainVertex(NULL), m_pMetheo(NULL), m_pPlayerSkill2(NULL)
, m_bArmIdle(false), m_bHeadIdle(false), m_bBodyIdle(false), m_bLegIdle(false)
, m_bLeg_MoveUp(false)
, m_fLeftLeg_AngleX(0.f), m_fRightLeg_AngleX(0.f)
, m_fLeftArm_AngleX(0.f), m_fRightArm_AngleX(0.f)
, m_bMove_LegWalk(false), m_bMove_HandShake(false), m_bSwordIdle(false), m_bSwordMove(false)
, m_Gravity(0.f), m_fSwordMove(0.f), m_fAttackArmAngle(180.f)
, m_bAttackBtn(false), m_bReflectAni(false), m_bSkillUse(false), m_bCrush(false)
, m_fReflectAngle(0.f), m_fSkillTime(0.f), m_fCrushTime(0), m_pUnitCol(NULL), m_fAttTime(0.f)
, m_fMetheoTime(0.f), m_bMetheo_Use(false), m_iCountMetheo(0), m_fSkill_2_Size(0)
, m_bUseAdonbbagadon(false), m_bSkill_2_fase_1(false), m_bSkill_2_fase_2(false), m_bSkill_2_fase_3(false)
, m_fSkill_2_Time(0.f), m_pAdonbbagadon_IN(NULL), m_pAdonbbagadon_OUT(NULL)
{

}

CPlayer_Elond::~CPlayer_Elond(void)
{
	Release();
}

HRESULT CPlayer_Elond::Initialize(void)
{
	m_iHp = 3;
	m_fSpeed = 10.f;

	TCHAR szFilePath[MAX_PATH] = L"../../Data/Object_Data/OriginBody.dat";

	HANDLE	hFile	= CreateFile(szFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwByte	= 0;

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

	CloseHandle(hFile);

	m_pSword = CSword::Create(m_pDevice, CSword::UT_PLAYER);

	ENGINE::CGameObject* pGameObject = NULL;

	pGameObject = CPlayer2_Skill::Create(m_pDevice, 500.f, 350.f);
	ENGINE::CLayerMgr::GetInstance()->AddObject(L"Player2 Skill", pGameObject, ENGINE::LAYER_UI);

	pGameObject = CPlayer2_Skill2::Create(m_pDevice, 430.f, 350.f);
	ENGINE::CLayerMgr::GetInstance()->AddObject(L"Player2 Skill2", pGameObject, ENGINE::LAYER_UI);


	for(int i = 0; i < 3; ++i)
	{
		pGameObject = CHeart::Create(m_pDevice, -520.f + (i*30.f), 300.f);
		m_pLayerMgr->AddObject(L"Player Heart", pGameObject, ENGINE::LAYER_UI);
	}


	m_Heartlist = m_pLayerMgr->GetObjList(L"Player Heart", ENGINE::LAYER_UI);

	FAILED_CHECK(AddComponent());

	m_bMeteo = false;
	m_bBuff  = false;

	return S_OK;
}

HRESULT CPlayer_Elond::AddComponent(void)
{
	CUnit::AddComponent();

	ENGINE::CComponent*		pComponent = NULL;


	pComponent = m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_TERRAIN);
	m_pTerrainCol = dynamic_cast<CTerrainCol*>(pComponent);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));
	
	return S_OK;
}

void CPlayer_Elond::Update(void)
{
	CUnit::Update();
	
	((CPlayer2_Skill2*)ENGINE::CLayerMgr::GetInstance()->GetGameObject(L"Player2 Skill2", ENGINE::LAYER_UI))->fMetheoPos = m_pInfo->m_vPos;
	((CPlayer2_Skill2*)ENGINE::CLayerMgr::GetInstance()->GetGameObject(L"Player2 Skill2", ENGINE::LAYER_UI))->fMetheoDir = m_pInfo->m_vDir;

	//cout << "size : " << m_Heartlist->size() << endl;

	float m_fTime = m_pTimeMgr->GetTime();

	if(!m_pTerrainCol->GetCollision())
	{
		m_Gravity += m_fTime;

		m_pInfo->m_vPos.y -= 0.098 * m_Gravity * m_Gravity * 4.f;
	}

	ENGINE::CGameObject* pTerrain = m_pLayerMgr->GetGameObject(L"Terrain", ENGINE::LAYER_ENVIRONMENT);
	m_pTerrainVertex = dynamic_cast<CTerrain*>(pTerrain)->GetTerrainVertex();
	m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex, dynamic_cast<CTerrain*>(pTerrain));

	KeyDown();
	Jump();
	BodyAnimation();

	if(m_bMetheo_Use)
		Skill_1_Metheo();

	if(m_bUseAdonbbagadon)
		Skill_2_Adonbbagadon();

	if(m_bAttackBtn)
	{
		Ani_Attack();
		AttMode();
		AttBoss();
	}

	if(m_bReflectAni)
		Ani_Reflection();

	if(m_bSkillUse)
		Ani_SkillUse();

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

	if(m_bAttackBtn == false && m_bReflectAni == false && m_bSkillUse == false && m_bCrush == false)
		Ani_Sword();

	m_pSword->Update();
	ENGINE::CGameObject::Update();

	m_eUnitStateType = ENGINE::US_IDLE;
}

void CPlayer_Elond::Render(void)
{
	for(int i=0; i<ENGINE::BP_END; ++i)
		m_pBodyPart[i]->Render();

	m_pSword->Render();
}

void CPlayer_Elond::Release(void)
{

}

CPlayer_Elond* CPlayer_Elond::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer_Elond* pElond = new CPlayer_Elond(pDevice);

	if(FAILED(pElond->Initialize()))
		ENGINE::Safe_Delete(pElond);

	return pElond;
}

void CPlayer_Elond::CreateBodyPart(ENGINE::SAVECUBE* pSaveCube)
{
	switch(pSaveCube->m_SaveType)
	{
	case ENGINE::SOBJ_HEAD:
		m_pBodyPart[ENGINE::BP_HEAD] = CHead::Create(m_pDevice, CHead::UT_PLAYER_ELOND);
		m_pBodyPart[ENGINE::BP_HEAD]->SetInfo(pSaveCube);
		dynamic_cast<CHead*>(m_pBodyPart[ENGINE::BP_HEAD])->SetInterver(pSaveCube);		
		//m_pLayerMgr->AddObject(L"Isildur_Head", m_pBodyPart[ENGINE::BP_HEAD], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_BODY:
		m_pBodyPart[ENGINE::BP_BODY] = CBody::Create(m_pDevice, CBody::UT_PLAYER_ELOND);
		m_pBodyPart[ENGINE::BP_BODY]->SetInfo(pSaveCube);
		dynamic_cast<CBody*>(m_pBodyPart[ENGINE::BP_BODY])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_Body", m_pBodyPart[ENGINE::BP_BODY], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_L_ARM_UP:
		m_pBodyPart[ENGINE::BP_L_ARM_UP] = CLeftArm_Up::Create(m_pDevice, CLeftArm_Up::UT_PLAYER_ELOND);
		m_pBodyPart[ENGINE::BP_L_ARM_UP]->SetInfo(pSaveCube);
		dynamic_cast<CLeftArm_Up*>(m_pBodyPart[ENGINE::BP_L_ARM_UP])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_L_Arm_Up", m_pBodyPart[ENGINE::BP_L_ARM_UP], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_L_ARM_DOWN:
		m_pBodyPart[ENGINE::BP_L_ARM_DOWN] = CLeftArm_Down::Create(m_pDevice, CLeftArm_Down::UT_PLAYER_ELOND);		
		m_pBodyPart[ENGINE::BP_L_ARM_DOWN]->SetInfo(pSaveCube);
		dynamic_cast<CLeftArm_Down*>(m_pBodyPart[ENGINE::BP_L_ARM_DOWN])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_L_Arm_Down", m_pBodyPart[ENGINE::BP_L_ARM_DOWN], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_R_ARM_UP:
		m_pBodyPart[ENGINE::BP_R_ARM_UP] = CRightArm_Up::Create(m_pDevice, CRightArm_Up::UT_PLAYER_ELOND);
		m_pBodyPart[ENGINE::BP_R_ARM_UP]->SetInfo(pSaveCube);
		dynamic_cast<CRightArm_Up*>(m_pBodyPart[ENGINE::BP_R_ARM_UP])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_R_Arm_Up", m_pBodyPart[ENGINE::BP_R_ARM_UP], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_R_ARM_DOWN:
		m_pBodyPart[ENGINE::BP_R_ARM_DOWN] = CRightArm_Down::Create(m_pDevice, CRightArm_Down::UT_PLAYER_ELOND);
		m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->SetInfo(pSaveCube);
		dynamic_cast<CRightArm_Down*>(m_pBodyPart[ENGINE::BP_R_ARM_DOWN])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_R_Arm_Down", m_pBodyPart[ENGINE::BP_R_ARM_DOWN], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_L_LEG:
		m_pBodyPart[ENGINE::BP_L_LEG] = CLeftLeg::Create(m_pDevice, CLeftLeg::UT_PLAYER_ELOND);
		m_pBodyPart[ENGINE::BP_L_LEG]->SetInfo(pSaveCube);
		dynamic_cast<CLeftLeg*>(m_pBodyPart[ENGINE::BP_L_LEG])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_L_Leg", m_pBodyPart[ENGINE::BP_L_LEG], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_R_LEG:
		m_pBodyPart[ENGINE::BP_R_LEG] = CRightLeg::Create(m_pDevice, CRightLeg::UT_PLAYER_ELOND);
		m_pBodyPart[ENGINE::BP_R_LEG]->SetInfo(pSaveCube);
		dynamic_cast<CRightLeg*>(m_pBodyPart[ENGINE::BP_R_LEG])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_R_Leg", m_pBodyPart[ENGINE::BP_R_LEG], ENGINE::LAYER_GAMELOGIC);
		break;
	}
}

void CPlayer_Elond::KeyDown(void)
{
	float m_fTime = m_pTimeMgr->GetTime();

	if(m_pCameraMgr->GetDisType() == m_pCameraMgr->DIS_NORMAL)
	{
		//오른쪽 위 대각선
		if(GetAsyncKeyState('W') && GetAsyncKeyState('D'))
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.z += 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x += 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(225.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//왼쪽 위 대각선
		else if(GetAsyncKeyState('W') && GetAsyncKeyState('A'))
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.z += 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x -= 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(135.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//오른쪽 아래 대각선
		else if(GetAsyncKeyState('S') && GetAsyncKeyState('D'))
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.z -= 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x += 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(315.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//왼쪽 아래 대각선
		else if(GetAsyncKeyState('S') && GetAsyncKeyState('A'))
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.z -= 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x -= 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(45.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//위
		else if(GetAsyncKeyState('W') & 0x8000)
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.z += 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(180.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//아래
		else if(GetAsyncKeyState('S') & 0x8000)
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.z -= 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(0.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//왼쪽
		else if(GetAsyncKeyState('A') & 0x8000)
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.x -= 1.f * m_fSpeed * m_fTime;

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

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//오른쪽
		else if(GetAsyncKeyState('D') & 0x8000)
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.x += 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(270.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('Q'))
		{
			
			if(m_bAttackBtn == false)
			{
				m_pSoundMgr->Hit(L"Hit1.wav");
				m_bAttackBtn = true;
			}			
		}
		else if(GetAsyncKeyState('E'))	//튕겨내기
		{
			if(m_bReflectAni == false)
			{
				m_pSoundMgr->GengiSound(L"GenjiSound.wav");
				m_bReflectAni = true;
			}
		}
	}

	if(m_pCameraMgr->GetDisType() == m_pCameraMgr->DIS_STAGE1_BOSS)
	{
		if(GetAsyncKeyState('A') && GetAsyncKeyState('W'))
		{
			m_pInfo->m_vPos.z += 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x += 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(225.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('A') && GetAsyncKeyState('S'))
		{
			m_pInfo->m_vPos.z += 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x -= 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(135.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('D') && GetAsyncKeyState('W'))
		{
			m_pInfo->m_vPos.z -= 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x += 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(315.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('D') && GetAsyncKeyState('S'))
		{
			m_pInfo->m_vPos.z -= 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x -= 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(45.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('A') & 0x8000)
		{
			m_pInfo->m_vPos.z += 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(180.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('D') & 0x8000)
		{
			m_pInfo->m_vPos.z -= 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(0.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('S') & 0x8000)
		{
			m_pInfo->m_vPos.x -= 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

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

			if(GetAsyncKeyState('Q'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('W') & 0x8000)
		{
			m_pInfo->m_vPos.x += 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(270.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('Q') & 0x8000)
		{
			
			if(m_bAttackBtn == false)
			{
				m_pSoundMgr->Hit(L"Hit2.wav");
				m_bAttackBtn = true;
			}

		}
		else if(GetAsyncKeyState('E'))	//튕겨내기
		{
			if(m_bReflectAni == false)
			{
				m_pSoundMgr->GengiSound(L"GenjiSound.wav");
				m_bReflectAni = true;
			}
		}
	}

	if(m_pCameraMgr->GetDisType() == m_pCameraMgr->DIS_STAGE2_BOSS)
	{
		if(GetAsyncKeyState('A') && GetAsyncKeyState('W'))
		{
			m_pInfo->m_vPos.z += 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x += 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(225.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{

				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('A') && GetAsyncKeyState('S'))
		{
			m_pInfo->m_vPos.z += 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x -= 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(135.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{

				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('D') && GetAsyncKeyState('W'))
		{
			m_pInfo->m_vPos.z -= 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x += 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(315.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{

				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('D') && GetAsyncKeyState('S'))
		{
			m_pInfo->m_vPos.z -= 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x -= 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(45.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{

				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('A') & 0x8000)
		{
			m_pInfo->m_vPos.z += 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(180.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{

				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('D') & 0x8000)
		{
			m_pInfo->m_vPos.z -= 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(0.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{

				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('S') & 0x8000)
		{
			m_pInfo->m_vPos.x -= 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

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

			if(GetAsyncKeyState('Q'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('W') & 0x8000)
		{
			m_pInfo->m_vPos.x += 1.f * m_fSpeed * m_fTime;
			m_eUnitStateType = ENGINE::US_MOVE;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(270.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{

				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}

		if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if(m_bReflectAni == false)
			{
				
				m_bReflectAni = true;
			}
		}	
	}

	if(m_pCameraMgr->GetDisType() == m_pCameraMgr->DIS_STAGE2)
	{
		//오른쪽 위 대각선
		if(GetAsyncKeyState('W') && GetAsyncKeyState('D'))
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.z += 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x += 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(225.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//왼쪽 위 대각선
		else if(GetAsyncKeyState('W') && GetAsyncKeyState('A'))
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.z += 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x -= 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(135.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//오른쪽 아래 대각선
		else if(GetAsyncKeyState('S') && GetAsyncKeyState('D'))
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.z -= 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x += 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(315.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//왼쪽 아래 대각선
		else if(GetAsyncKeyState('S') && GetAsyncKeyState('A'))
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.z -= 1.f * m_fSpeed * m_fTime;
			m_pInfo->m_vPos.x -= 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(45.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//위
		else if(GetAsyncKeyState('W') & 0x8000)
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.z += 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(180.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//아래
		else if(GetAsyncKeyState('S') & 0x8000)
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.z -= 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(0.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//왼쪽
		else if(GetAsyncKeyState('A') & 0x8000)
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.x -= 1.f * m_fSpeed * m_fTime;

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

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//오른쪽
		else if(GetAsyncKeyState('D') & 0x8000)
		{
			m_eUnitStateType = ENGINE::US_MOVE;
			m_pInfo->m_vPos.x += 1.f * m_fSpeed * m_fTime;

			m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);
			m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = D3DXToRadian(0.f);	

			if(m_bLeg_MoveUp == false)
			{		
				m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos.x += 0.08f;
				m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos.x -= 0.08f;

				m_bLeg_MoveUp = true;
				m_bLegIdle = false;
			}

			m_pInfo->m_fAngle[ENGINE::ANGLE_Y] = D3DXToRadian(270.f);
			D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);

			if(GetAsyncKeyState('Q'))
			{
				
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('Q'))
		{
			
			if(m_bAttackBtn == false)
			{
				m_pSoundMgr->Hit(L"Hit1.wav");
				m_bAttackBtn = true;
			}			
		}
		
		else if(GetAsyncKeyState('E'))	//튕겨내기
		{
			if(m_bReflectAni == false)
			{
				m_pSoundMgr->GengiSound(L"GenjiSound.wav");
				m_bReflectAni = true;
			}
		}
	}

	if(GetAsyncKeyState('V') & 0x8000)
	{
		m_pTerrainCol->GetJumpAble() = true;
		m_pTerrainCol->GetCollision() = false;
	}

	if(GetAsyncKeyState('Z') & 0x8000) // 메테오
	{
		if(m_bSkillUse == false)
		{
			m_pSoundMgr->PlaySound(L"MetheoSpeak.wav");
			m_bSkillUse = true;
			m_fSkillTime = 0.f;
		}
		
		if(m_bMetheo_Use == false)
			m_bMetheo_Use = true;
		

		/*m_pMetheo = CMetheo::Create(m_pDevice, m_pInfo->m_vPos, m_pInfo->m_vDir);
		ENGINE::CLayerMgr::GetInstance()->AddObject(L"Metheo",m_pMetheo,
			ENGINE::LAYER_GAMELOGIC);

		((CMetheo*)(m_pMetheo))->GetStone() = true;*/
		((CPlayer2_Skill2*)ENGINE::CLayerMgr::GetInstance()->GetGameObject(L"Player2 Skill2", ENGINE::LAYER_UI))->GetClick() = true;
	}

	if(GetAsyncKeyState('X') & 0x8000)		//아돈 빠가 돈. 
	{
		if(m_bSkillUse == false)
		{
			m_pSoundMgr->Zarya(L"ZaryaUlti.wav");
			m_bSkillUse = true;
			m_fSkillTime = 0.f;
		}

		if(m_bUseAdonbbagadon == false)
		{
			m_bUseAdonbbagadon = true;
			m_vecParticlePos = m_pInfo->m_vPos;

			m_fSkill_2_Size = 1.f;

			m_pAdonbbagadon_IN = CSkillParticle::Create(m_pDevice);
			dynamic_cast<CSkillParticle*>(m_pAdonbbagadon_IN)->SetColor(255.f/255.f, 250.f/255.f, 250.f/255.f, 1.f);
			dynamic_cast<CSkillParticle*>(m_pAdonbbagadon_IN)->SetSize(m_fSkill_2_Size, m_fSkill_2_Size);
			dynamic_cast<CSkillParticle*>(m_pAdonbbagadon_IN)->SetPos(m_vecParticlePos);
			m_pLayerMgr->AddObject(L"Addonbbagadon_IN", m_pAdonbbagadon_IN, ENGINE::LAYER_GAMELOGIC);
			dynamic_cast<CSkillParticle*>(m_pAdonbbagadon_IN)->SetAni();

			m_pAdonbbagadon_OUT = CSkillParticle::Create(m_pDevice);
			dynamic_cast<CSkillParticle*>(m_pAdonbbagadon_OUT)->SetColor(0.f/255.f, 0.f/255.f, 205.f/255.f, 0.1f);
			dynamic_cast<CSkillParticle*>(m_pAdonbbagadon_OUT)->SetSize(m_fSkill_2_Size, m_fSkill_2_Size);
			dynamic_cast<CSkillParticle*>(m_pAdonbbagadon_OUT)->SetPos(m_vecParticlePos);
			m_pLayerMgr->AddObject(L"Addonbbagadon_OUT", m_pAdonbbagadon_OUT, ENGINE::LAYER_GAMELOGIC);
			dynamic_cast<CSkillParticle*>(m_pAdonbbagadon_OUT)->SetAni();
		}		

		((CPlayer2_Skill*)ENGINE::CLayerMgr::GetInstance()->GetGameObject(L"Player2 Skill", ENGINE::LAYER_UI))->GetClick() = true;
	}
}

void CPlayer_Elond::SetBodyPos(D3DXVECTOR3& pos)
{
	D3DXVECTOR3 pMovePos = pos - m_pInfo->m_vPos;

	SetPos(pos);

	for(int i=0; i<ENGINE::BP_END; ++i)
	{
		m_pBodyPart[i]->GetInfo()->m_vPos += pos;
	}
}

void CPlayer_Elond::BodyAnimation(void)
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

void CPlayer_Elond::Ani_Idle(void)
{
	//오른, 왼팔 들썩.
	if(m_bArmIdle == false)
	{
		m_pBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_vPos.y += 0.0001f;
		m_pBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_vPos.y += 0.0001f;

		m_pBodyPart[ENGINE::BP_R_ARM_UP]->GetInfo()->m_vPos.y += 0.0001f;
		m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.y += 0.0001f;

		if((m_pBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_vPos.y + 2.f) >= (m_pInfo->m_vPos.y + 0.014f))
			m_bArmIdle = true;
	}
	else
	{
		m_pBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_vPos.y -= 0.0001f;
		m_pBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_vPos.y -= 0.0001f;

		m_pBodyPart[ENGINE::BP_R_ARM_UP]->GetInfo()->m_vPos.y -= 0.0001f;
		m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.y -= 0.0001f;

		if((m_pBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_vPos.y + 2.f) <= (m_pInfo->m_vPos.y))
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

		if((m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos.y + 2.f) >= (m_pInfo->m_vPos.y + 0.008f))
			m_bBodyIdle = true;
	}
	else
	{
		m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos.y -= 0.00007f;
		m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos.y -= 0.00007f;

		if((m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos.y + 2.f) <= (m_pInfo->m_vPos.y - 0.008f))
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
}

void CPlayer_Elond::Ani_Attack(void)
{
	D3DXVECTOR3 X_Cross;
	D3DXVec3Cross(&X_Cross, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);

	X_Cross.y += 0.5f;
	X_Cross.x += 1.5f;

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fAttackArmAngle));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fAttackArmAngle));

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pSword)->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(m_fAttackArmAngle-55.f));

	m_fAttackArmAngle -= 20.f;

	if(m_fAttackArmAngle <= -90.f)
	{
		m_fAttackArmAngle = 180.f;
		m_bAttackBtn = false;
	}
}

void CPlayer_Elond::Jump(void)
{
	if(m_pTerrainCol->GetJumpAble() == true)
	{
		m_pTerrainCol->GetCollision() = false;

		m_pInfo->m_vPos.y += 0.098 * m_Gravity * 3.f;
	}

	if(m_pInfo->m_vPos.y < 1)
	{
		m_pTerrainCol->GetCollision() = true;
		m_pInfo->m_vPos.y = 1;
		m_Gravity = 0;
		m_pTerrainCol->GetJumpAble() = false;
	}
}	

void CPlayer_Elond::Ani_LegWalk(void)
{
	D3DXVECTOR3 X_Cross;
	D3DXVec3Cross(&X_Cross, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, -1.f, 0.f));

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

void CPlayer_Elond::Ani_WalkHandShake(void)
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

void CPlayer_Elond::Ani_Sword(void)
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

			m_pSword->GetInfo()->m_vPos = D3DXVECTOR3(m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.x + 0.4f,
				m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.y + 0.2f,
				m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.z - 0.5f);

			if(m_bMove_HandShake)
			{
				m_pSword->GetInfo()->m_vPos.y += (m_fSwordMove += 0.02f);
			}
			else
			{
				m_pSword->GetInfo()->m_vPos.y += (m_fSwordMove -= 0.02f);
			}
		}
		break;
	}
}

void CPlayer_Elond::Ani_Reflection(void)
{
	D3DXVECTOR3 X_Cross;
	D3DXVec3Cross(&X_Cross, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);

	X_Cross.y += 0.5f;
	X_Cross.x += 1.5f;

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(90.f));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(90.f));

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pSword)->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(0.f));

	m_pSword->GetInfo()->m_vPos.z = m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.z - 1.f;

	D3DXVECTOR3 Sword_Cross;
	D3DXVec3Cross(&Sword_Cross, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos);

	Sword_Cross.z -= 1.f;
	m_fReflectAngle += 5.f;

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pSword)->GetMatrix())[ENGINE::MAT_AXIS], &Sword_Cross, D3DXToRadian(m_fReflectAngle));

	if(m_fReflectAngle >= 600.f)
	{
		m_bReflectAni = false;
		m_fReflectAngle = 0.f;
	}
}

void CPlayer_Elond::Ani_SkillUse(void)
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

	m_pSword->GetInfo()->m_vPos.z = m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.z - 1.f;

	D3DXVECTOR3 Sword_Cross;
	D3DXVec3Cross(&Sword_Cross, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos);

	Sword_Cross.z -= 1.f;

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pSword)->GetMatrix())[ENGINE::MAT_AXIS], &Sword_Cross, D3DXToRadian(0.f));

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pSword)->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(50.f));

	m_fSkillTime += 1.f * m_pTimeMgr->GetTime();

	if(m_fSkillTime > 1.f)
		m_bSkillUse = false;
}

void CPlayer_Elond::AttMode(void)
{
	m_fAttTime += 1.f * m_pTimeMgr->GetTime();

	if(m_fAttTime >= 0.2f)
	{
		if(m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->size())
		{
			list<ENGINE::CGameObject*>::iterator iter = m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->begin();
			list<ENGINE::CGameObject*>::iterator iter_end = m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->end();

			for(; iter != iter_end; ++iter)
			{
				D3DXVECTOR3	vCenter;

				vCenter.x = abs(m_pInfo->m_vPos.x + ((CMonster_Orc*)(*iter))->GetInfo()->m_vPos.x) / 2;
				vCenter.y = abs(m_pInfo->m_vPos.y + ((CMonster_Orc*)(*iter))->GetInfo()->m_vPos.y) / 2;
				vCenter.z = abs(m_pInfo->m_vPos.z + ((CMonster_Orc*)(*iter))->GetInfo()->m_vPos.z) / 2;

				float fX = m_pInfo->m_vPos.x - ((CMonster_Orc*)(*iter))->GetInfo()->m_vPos.x;
				float fY = m_pInfo->m_vPos.y - ((CMonster_Orc*)(*iter))->GetInfo()->m_vPos.y;
				float fZ = m_pInfo->m_vPos.z - ((CMonster_Orc*)(*iter))->GetInfo()->m_vPos.z;

				float fDistance = sqrtf(fX * fX + fY * fY + fZ * fZ);

				if(fDistance < 5.f)
				{
					m_pUnitCol->SetColInfo(&m_pInfo->m_vPos, &((CMonster_Orc*)m_pLayerMgr->GetGameObject(L"Monser_Orc", ENGINE::LAYER_MONSTER))->GetInfo()->m_vPos/*공격하는 상대의 포스*/);

					((CMonster_Orc*)(*iter))->GetAttHp() -= 1;
					//cout << "넌 깎여야한다. " << endl;
					m_pSoundMgr->PlayerAtt2(L"PlayerAtt2.wav");

					m_fAttTime = 0.f;

					// HIT 이펙트
					ENGINE::CGameObject* pGameObject = CHitParticle::Create(m_pDevice, vCenter);
					ENGINE::CLayerMgr::GetInstance()->AddObject(L"HitParticle", pGameObject, ENGINE::LAYER_GAMELOGIC);
					dynamic_cast<CHitParticle*>(pGameObject)->SetAni();
				}
			}
		}

		m_fAttTime = 0.f;
	}
}	

void CPlayer_Elond::AttBoss(void)
{
	m_fAttTime += 1.f * m_pTimeMgr->GetTime();

	if(m_fAttTime >= 0.2f)
	{
		if(m_pLayerMgr->GetObjList(L"Stage1_Boss", ENGINE::LAYER_MONSTER) == NULL)
			return;

		if(m_pLayerMgr->GetObjList(L"Stage1_Boss", ENGINE::LAYER_MONSTER)->size())
		{
			list<ENGINE::CGameObject*>::iterator iter = m_pLayerMgr->GetObjList(L"Stage1_Boss", ENGINE::LAYER_MONSTER)->begin();
			list<ENGINE::CGameObject*>::iterator iter_end = m_pLayerMgr->GetObjList(L"Stage1_Boss", ENGINE::LAYER_MONSTER)->end();

			for(; iter != iter_end; ++iter)
			{
				D3DXVECTOR3	vCenter;

				vCenter.x = abs(m_pInfo->m_vPos.x + ((CBoss_1*)(*iter))->GetInfo()->m_vPos.x) / 2;
				vCenter.y = abs(m_pInfo->m_vPos.y + ((CBoss_1*)(*iter))->GetInfo()->m_vPos.y) / 2;
				vCenter.z = abs(m_pInfo->m_vPos.z + ((CBoss_1*)(*iter))->GetInfo()->m_vPos.z) / 2;

				float fX = m_pInfo->m_vPos.x - ((CBoss_1*)(*iter))->GetInfo()->m_vPos.x;
				float fY = m_pInfo->m_vPos.y - ((CBoss_1*)(*iter))->GetInfo()->m_vPos.y;
				float fZ = m_pInfo->m_vPos.z - ((CBoss_1*)(*iter))->GetInfo()->m_vPos.z;

				float fDistance = sqrtf(fX * fX + fY * fY + fZ * fZ);

				if(fDistance < 5.f)
				{
					m_pUnitCol->SetColInfo(&m_pInfo->m_vPos, &((CBoss_1*)m_pLayerMgr->GetGameObject(L"Stage1_Boss", ENGINE::LAYER_MONSTER))->GetInfo()->m_vPos/*공격하는 상대의 포스*/);

					((CBoss_1*)(*iter))->GetAttHp() -= 1;
					//cout << "넌 깎여야한다. " << endl;

					m_fAttTime = 0.f;

					// HIT 이펙트
					ENGINE::CGameObject* pGameObject = CHitParticle::Create(m_pDevice, vCenter);
					ENGINE::CLayerMgr::GetInstance()->AddObject(L"HitParticle", pGameObject, ENGINE::LAYER_GAMELOGIC);
					dynamic_cast<CHitParticle*>(pGameObject)->SetAni();
				}
			}
		}

		m_fAttTime = 0.f;
	}
}

void CPlayer_Elond::Skill_1_Metheo(void)
{
	m_fMetheoTime += 1.f * m_pTimeMgr->GetTime();	

	if(m_fMetheoTime > 0.8f)
	{
		int iCount = 0;

		list<ENGINE::CGameObject*>::iterator iter		= m_pLayerMgr->GetObjList(L"Metheo", ENGINE::LAYER_GAMELOGIC)->begin();
		list<ENGINE::CGameObject*>::iterator iter_end	= m_pLayerMgr->GetObjList(L"Metheo", ENGINE::LAYER_GAMELOGIC)->end();

		for(; iter != iter_end; ++iter)
		{
			if(m_iCountMetheo == iCount)
			{
				dynamic_cast<CMetheo*>(*iter)->GetDestPos() = D3DXVECTOR3(m_pInfo->m_vPos.x + rand()%15, -5.f, m_pInfo->m_vPos.z + rand()%15);
				dynamic_cast<CMetheo*>(*iter)->GetStone() = true;

				++m_iCountMetheo;
				m_fMetheoTime = 0.f;
				break;
			}
			else
				++iCount;
		}
	}

	if(m_iCountMetheo >= 6)
	{
		m_iCountMetheo = 0;
		m_bMetheo_Use = false;
		m_fMetheoTime = 0.f;
	}
}

void CPlayer_Elond::Skill_2_Adonbbagadon(void)
{
	if(m_bSkill_2_fase_1 == false)	//발사
	{
		m_fSkill_2_Time += 1.f * m_pTimeMgr->GetTime();

		m_vecParticlePos.x += 0.08f;

		dynamic_cast<CSkillParticle*>(m_pAdonbbagadon_IN)->SetPos(m_vecParticlePos);
		dynamic_cast<CSkillParticle*>(m_pAdonbbagadon_OUT)->SetPos(m_vecParticlePos);

		if(m_fSkill_2_Time > 1.5f)
		{
			m_fSkill_2_Time = 0.f;
			m_bSkill_2_fase_1 = true;
		}
	}
	else if(m_bSkill_2_fase_2 == false) //모으기
	{
		m_fSkill_2_Time += 1.f * m_pTimeMgr->GetTime();

		D3DXVECTOR3 vecDir;
		D3DXVECTOR3 vecLength;
		float fLength = 0.f;

		list<ENGINE::CGameObject*>::iterator iter		= m_pLayerMgr->GetObjList(L"Dumy_Orc", ENGINE::LAYER_MONSTER)->begin();
		list<ENGINE::CGameObject*>::iterator iter_end	= m_pLayerMgr->GetObjList(L"Dumy_Orc", ENGINE::LAYER_MONSTER)->end();

		for(; iter != iter_end; ++iter)
		{
			vecLength = dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos -  m_vecParticlePos;

			fLength = D3DXVec3Length(&vecLength);

			if(fLength <= 15.f)				
			{
				vecDir = m_vecParticlePos - dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos;
				dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos += (vecDir / 5.f);				
			}
		}

		iter		= m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->begin();
		iter_end	= m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->end();

		for(; iter != iter_end; ++iter)
		{
			vecLength = dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos - m_vecParticlePos;

			fLength = D3DXVec3Length(&vecLength);

			if(fLength <= 15.f)
			{
				vecDir = m_vecParticlePos - dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos;
				dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos += (vecDir / 5.f);	
			}
		}

		if(m_fSkill_2_Time > 2.f)
		{
			m_fSkill_2_Time = 0.f;
			m_bSkill_2_fase_2 = true;
		}
	}
	else if(m_bSkill_2_fase_3 == false) //폭발
	{
		m_fSkill_2_Time += 1.f * m_pTimeMgr->GetTime();

		m_fSkill_2_Size += 0.2f;

		dynamic_cast<CSkillParticle*>(m_pAdonbbagadon_IN)->SetSize(m_fSkill_2_Size, m_fSkill_2_Size);
		dynamic_cast<CSkillParticle*>(m_pAdonbbagadon_OUT)->SetSize(m_fSkill_2_Size, m_fSkill_2_Size);

		D3DXVECTOR3 vecLength;
		float fLength = 0.f;

		list<ENGINE::CGameObject*>::iterator iter		= m_pLayerMgr->GetObjList(L"Dumy_Orc", ENGINE::LAYER_MONSTER)->begin();
		list<ENGINE::CGameObject*>::iterator iter_end	= m_pLayerMgr->GetObjList(L"Dumy_Orc", ENGINE::LAYER_MONSTER)->end();

		for(; iter != iter_end; ++iter)
		{
			vecLength = dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos - m_vecParticlePos;

			fLength = D3DXVec3Length(&vecLength);

			if(fLength <= 5.f)
				dynamic_cast<CMonster_Orc*>(*iter)->GetAttHp() = 0;
		}

		iter		= m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->begin();
		iter_end	= m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->end();

		for(; iter != iter_end; ++iter)
		{
			vecLength = dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos - m_vecParticlePos;

			fLength = D3DXVec3Length(&vecLength);

			if(fLength <= 5.f)
				dynamic_cast<CMonster_Orc*>(*iter)->GetAttHp() = 0;
		}	

		if(m_fSkill_2_Time > 1.f)
		{
			m_fSkill_2_Time = 0.f;
			m_bSkill_2_fase_1 = false;
			m_bSkill_2_fase_2 = false;
			m_bSkill_2_fase_3 = false;
			m_bUseAdonbbagadon = false;

			m_pLayerMgr->GetGameObject(L"Addonbbagadon_IN", ENGINE::LAYER_GAMELOGIC)->GetAlive() = false;
			m_pLayerMgr->GetGameObject(L"Addonbbagadon_OUT", ENGINE::LAYER_GAMELOGIC)->GetAlive() = false;
		}
	}
}

