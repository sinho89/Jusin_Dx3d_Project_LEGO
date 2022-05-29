#include "stdafx.h"
#include "Player_Isildur.h"

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
#include "UnitCol.h"
#include "..\Engine\Utility\code\GameObject.h"
#include "Sword.h"
#include "Player1Skill.h"
#include "Player1Skill2.h"
#include "Player2Skill.h"
#include "Heart2.h"
#include "Lhpclass.h"
#include "AttParticle.h"
#include "Monster_Orc.h"
#include "HitParticle.h"
#include "Boss_1.h"

CPlayer_Isildur::CPlayer_Isildur(LPDIRECT3DDEVICE9 pDevice)
: CUnit(pDevice)
, m_pLayerMgr(ENGINE::Get_LayerMgr()), m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL), m_pTerrainVertex(NULL), m_pUnitCol(NULL), m_pSword(NULL)
, m_bArmIdle(false), m_bHeadIdle(false), m_bBodyIdle(false), m_bLegIdle(false)
, m_bLeg_MoveUp(false), m_bMove_LegWalk(false), m_bMove_HandShake(false), m_bSwordIdle(false), m_bSwordMove(false)
, m_fLeftLeg_AngleX(0), m_fRightLeg_AngleX(0), m_fLeftArm_AngleX(0), m_fRightArm_AngleX(0)
, m_Gravity(0.f), m_fSwordMove(0.f), m_fAttackArmAngle(180.f)
, m_bAttackBtn(false), m_bReflectAni(false), m_bSkillUse(false), m_bCrush(false)
, m_fReflectAngle(0.f), m_fSkillTime(0.f), m_fCrushTime(0), m_fAttTime(0.f)
, m_fSkill_1_Length(0.5f), m_fSkill_1_Time(0.f), m_bSkill_1_Use(false)
, m_bPaticleMake(false) , m_bSkill_2_Fase1(false), m_bSkill_2_Fase2(false), m_bSkill_2_Fase3(false)
, m_bSkill_2_Use(false), m_fSkill_2_Time(0.f)
{
	m_pDyingBodyPart[ENGINE::BP_BODY] = CBody::Create(pDevice, CBody::UT_PLAYER_ESILDUR);
	m_pDyingBodyPart[ENGINE::BP_HEAD] = CHead::Create(pDevice, CHead::UT_PLAYER_ESILDUR);
	m_pDyingBodyPart[ENGINE::BP_R_ARM_UP] = CRightArm_Up::Create(pDevice, CRightArm_Up::UT_PLAYER_ESILDUR);
	m_pDyingBodyPart[ENGINE::BP_R_ARM_DOWN] = CRightArm_Down::Create(pDevice, CRightArm_Down::UT_PLAYER_ESILDUR);
	m_pDyingBodyPart[ENGINE::BP_L_ARM_UP] = CLeftArm_Up::Create(pDevice, CLeftArm_Up::UT_PLAYER_ESILDUR);
	m_pDyingBodyPart[ENGINE::BP_L_ARM_DOWN] = CLeftArm_Down::Create(pDevice, CLeftArm_Down::UT_PLAYER_ESILDUR);
	m_pDyingBodyPart[ENGINE::BP_R_LEG] = CRightLeg::Create(pDevice, CRightLeg::UT_PLAYER_ESILDUR);
	m_pDyingBodyPart[ENGINE::BP_L_LEG] = CLeftLeg::Create(pDevice, CLeftLeg::UT_PLAYER_ESILDUR);

	for(int i=0; i<8; ++i)
	{
		m_pParticle[i] = NULL;
	}
}

CPlayer_Isildur::~CPlayer_Isildur(void)
{
	Release();
}

HRESULT CPlayer_Isildur::Initialize(void)
{
	m_fSpeed = 10.f;
	TCHAR szFilePath[MAX_PATH] = L"../../Data/Object_Data/OriginBody.dat";
	HANDLE	hFile	= CreateFile(szFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwByte	= 0;

	m_pInfo->m_iHp = 3;

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

	pGameObject = CPlayer1_Skill::Create(m_pDevice, -500.f, 350.f);
	m_pLayerMgr->AddObject(L"Player Skill1", pGameObject, ENGINE::LAYER_UI);

	pGameObject = CPlayer1_Skill2::Create(m_pDevice, -430.f, 350.f);
	m_pLayerMgr->AddObject(L"Player Skill2", pGameObject, ENGINE::LAYER_UI);

	
	for(int i = 0; i < 3; ++i)
	{
		pGameObject = CHeart2::Create(m_pDevice, 480.f + (i*30.f), 300.f);
		m_pLayerMgr->AddObject(L"Player Heart2", pGameObject, ENGINE::LAYER_UI);
	}

	/*for(int i = 0; i < 3; ++i)
	{
		pGameObject = CLhpclass::Create(m_pDevice, 480.f + (i*30.f), 300.f);
		m_pLayerMgr->AddObject(L"Esildur LHP", pGameObject, ENGINE::LAYER_UI);

		pGameObject = CLhpclass::Create(m_pDevice, -520.f + (i*30.f), 300.f);
		m_pLayerMgr->AddObject(L"Elond LHP", pGameObject, ENGINE::LAYER_UI);
	}*/

	FAILED_CHECK(AddComponent());
	

	return S_OK;
}

HRESULT CPlayer_Isildur::AddComponent(void)
{
	CUnit::AddComponent();

	ENGINE::CComponent*		pComponent = NULL;

	pComponent = m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_TERRAIN);
	m_pTerrainCol = dynamic_cast<CTerrainCol*>(pComponent);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"TerrainCol", pComponent));

	return S_OK;
}

void CPlayer_Isildur::Update(void)
{
	CUnit::Update();

	float m_fTime = m_pTimeMgr->GetTime();

	if(!m_pTerrainCol->GetCollision())
	{
		m_Gravity += m_fTime;

		m_pInfo->m_vPos.y -= 0.098f * m_Gravity * m_Gravity * 4.f;
	}

	if(GetAsyncKeyState('0') & 0x8000)
	{
		ENGINE::CGameObject* pGameObject = CAttParticle::Create(m_pDevice);
		m_pLayerMgr->AddObject(L"VolcanoParticle", pGameObject, ENGINE::LAYER_GAMELOGIC);
		dynamic_cast<CAttParticle*>(pGameObject)->SetAni();
	}


	ENGINE::CGameObject* pTerrain = m_pLayerMgr->GetGameObject(L"Terrain", ENGINE::LAYER_ENVIRONMENT);
	m_pTerrainVertex = dynamic_cast<CTerrain*>(pTerrain)->GetTerrainVertex();
	m_pTerrainCol->SetColInfo(&m_pInfo->m_vPos, m_pTerrainVertex, dynamic_cast<CTerrain*>(pTerrain));

	KeyDown();

	Jump();

	BodyAnimation();

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

	if(m_bSkill_1_Use)
		Skill_1_Explosion();

	if(m_bSkill_2_Use)
		Skill_2_Slash();

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
 
	if(m_bAttackBtn == false && m_bReflectAni == false && m_bSkillUse == false && m_bCrush == false)
		Ani_Sword();

	m_pSword->Update();
	ENGINE::CGameObject::Update();

	m_eUnitStateType = ENGINE::US_IDLE;
}

void CPlayer_Isildur::Render(void)
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
void CPlayer_Isildur::Release(void)
{

}

CPlayer_Isildur* CPlayer_Isildur::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayer_Isildur* pIsildur = new CPlayer_Isildur(pDevice);

	if(FAILED(pIsildur->Initialize()))
		ENGINE::Safe_Delete(pIsildur);

	return pIsildur;
}

void CPlayer_Isildur::CreateBodyPart(ENGINE::SAVECUBE* pSaveCube)
{
	switch(pSaveCube->m_SaveType)
	{
	case ENGINE::SOBJ_HEAD:
		m_pBodyPart[ENGINE::BP_HEAD] = CHead::Create(m_pDevice, CHead::UT_PLAYER_ESILDUR);
		m_pBodyPart[ENGINE::BP_HEAD]->SetInfo(pSaveCube);
		dynamic_cast<CHead*>(m_pBodyPart[ENGINE::BP_HEAD])->SetInterver(pSaveCube);
		dynamic_cast<CHead*>(m_pDyingBodyPart[ENGINE::BP_HEAD])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_BODY:
		m_pBodyPart[ENGINE::BP_BODY] = CBody::Create(m_pDevice, CBody::UT_PLAYER_ESILDUR);
		m_pBodyPart[ENGINE::BP_BODY]->SetInfo(pSaveCube);
		dynamic_cast<CBody*>(m_pBodyPart[ENGINE::BP_BODY])->SetInterver(pSaveCube);
		dynamic_cast<CBody*>(m_pDyingBodyPart[ENGINE::BP_BODY])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_L_ARM_UP:
		m_pBodyPart[ENGINE::BP_L_ARM_UP] = CLeftArm_Up::Create(m_pDevice, CLeftArm_Up::UT_PLAYER_ESILDUR);
		m_pBodyPart[ENGINE::BP_L_ARM_UP]->SetInfo(pSaveCube);
		dynamic_cast<CLeftArm_Up*>(m_pBodyPart[ENGINE::BP_L_ARM_UP])->SetInterver(pSaveCube);
		dynamic_cast<CLeftArm_Up*>(m_pDyingBodyPart[ENGINE::BP_L_ARM_UP])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_L_ARM_DOWN:
		m_pBodyPart[ENGINE::BP_L_ARM_DOWN] = CLeftArm_Down::Create(m_pDevice, CLeftArm_Down::UT_PLAYER_ESILDUR);		
		m_pBodyPart[ENGINE::BP_L_ARM_DOWN]->SetInfo(pSaveCube);
		dynamic_cast<CLeftArm_Down*>(m_pBodyPart[ENGINE::BP_L_ARM_DOWN])->SetInterver(pSaveCube);		
		dynamic_cast<CLeftArm_Down*>(m_pDyingBodyPart[ENGINE::BP_L_ARM_DOWN])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_R_ARM_UP:
		m_pBodyPart[ENGINE::BP_R_ARM_UP] = CRightArm_Up::Create(m_pDevice, CRightArm_Up::UT_PLAYER_ESILDUR);
		m_pBodyPart[ENGINE::BP_R_ARM_UP]->SetInfo(pSaveCube);
		dynamic_cast<CRightArm_Up*>(m_pBodyPart[ENGINE::BP_R_ARM_UP])->SetInterver(pSaveCube);
		dynamic_cast<CRightArm_Up*>(m_pDyingBodyPart[ENGINE::BP_R_ARM_UP])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_R_ARM_DOWN:
		m_pBodyPart[ENGINE::BP_R_ARM_DOWN] = CRightArm_Down::Create(m_pDevice, CRightArm_Down::UT_PLAYER_ESILDUR);
		m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->SetInfo(pSaveCube);
		dynamic_cast<CRightArm_Down*>(m_pBodyPart[ENGINE::BP_R_ARM_DOWN])->SetInterver(pSaveCube);
		dynamic_cast<CRightArm_Down*>(m_pDyingBodyPart[ENGINE::BP_R_ARM_DOWN])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_L_LEG:
		m_pBodyPart[ENGINE::BP_L_LEG] = CLeftLeg::Create(m_pDevice, CLeftLeg::UT_PLAYER_ESILDUR);
		m_pBodyPart[ENGINE::BP_L_LEG]->SetInfo(pSaveCube);
		dynamic_cast<CLeftLeg*>(m_pBodyPart[ENGINE::BP_L_LEG])->SetInterver(pSaveCube);
		dynamic_cast<CLeftLeg*>(m_pDyingBodyPart[ENGINE::BP_L_LEG])->SetInterver(pSaveCube);
		break;

	case ENGINE::SOBJ_R_LEG:
		m_pBodyPart[ENGINE::BP_R_LEG] = CRightLeg::Create(m_pDevice, CRightLeg::UT_PLAYER_ESILDUR);
		m_pBodyPart[ENGINE::BP_R_LEG]->SetInfo(pSaveCube);
		dynamic_cast<CRightLeg*>(m_pBodyPart[ENGINE::BP_R_LEG])->SetInterver(pSaveCube);
		dynamic_cast<CRightLeg*>(m_pDyingBodyPart[ENGINE::BP_R_LEG])->SetInterver(pSaveCube);
		break;
	}
}

void CPlayer_Isildur::KeyDown(void)
{
	float m_fTime = m_pTimeMgr->GetTime();	

	if(m_pCameraMgr->GetDisType() == m_pCameraMgr->DIS_NORMAL)
	{
		//¿À¸¥ÂÊ À§ ´ë°¢¼±
		if(GetAsyncKeyState('I') && GetAsyncKeyState('L'))
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//¿ÞÂÊ À§ ´ë°¢¼±
		else if(GetAsyncKeyState('I') && GetAsyncKeyState('J'))
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//¿À¸¥ÂÊ ¾Æ·¡ ´ë°¢¼±
		else if(GetAsyncKeyState('K') && GetAsyncKeyState('L'))
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//¿ÞÂÊ ¾Æ·¡ ´ë°¢¼±
		else if(GetAsyncKeyState('K') && GetAsyncKeyState('J'))
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//À§
		else if(GetAsyncKeyState('I') & 0x8000)
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//¾Æ·¡
		else if(GetAsyncKeyState('K') & 0x8000)
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//¿ÞÂÊ
		else if(GetAsyncKeyState('J') & 0x8000)
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//¿À¸¥ÂÊ
		else if(GetAsyncKeyState('L') & 0x8000)
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('H') & 0x8000)
		{
			if(m_bAttackBtn == false)
			{
				m_pSoundMgr->Hit2(L"Hit1.wav");
				m_bAttackBtn = true;
			}
		
		}
		else if(GetAsyncKeyState('N'))	//Æ¨°Ü³»±â
		{
			if(m_bReflectAni == false)
			{
				m_bReflectAni = true;
			}
		}
	}
	
	if(m_pCameraMgr->GetDisType() == m_pCameraMgr->DIS_STAGE1_BOSS)
	{
		if(GetAsyncKeyState('J') && GetAsyncKeyState('I'))
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('J') && GetAsyncKeyState('K'))
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('L') && GetAsyncKeyState('I'))
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('L') && GetAsyncKeyState('K'))
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('J') & 0x8000)
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('L') & 0x8000)
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('K') & 0x8000)
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('I') & 0x8000)
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('H') & 0x8000)
		{
			if(m_bAttackBtn == false)
			{
				m_pSoundMgr->Hit2(L"Hit1.wav");
				m_bAttackBtn = true;
			}

		}
		else if(GetAsyncKeyState('N'))	//Æ¨°Ü³»±â
		{
			if(m_bReflectAni == false)
			{
				m_bReflectAni = true;
			}
		}
	}

	if(m_pCameraMgr->GetDisType() == m_pCameraMgr->DIS_STAGE2)
	{
		//¿À¸¥ÂÊ À§ ´ë°¢¼±
		if(GetAsyncKeyState('I') && GetAsyncKeyState('L'))
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//¿ÞÂÊ À§ ´ë°¢¼±
		else if(GetAsyncKeyState('I') && GetAsyncKeyState('J'))
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//¿À¸¥ÂÊ ¾Æ·¡ ´ë°¢¼±
		else if(GetAsyncKeyState('K') && GetAsyncKeyState('L'))
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit1.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//¿ÞÂÊ ¾Æ·¡ ´ë°¢¼±
		else if(GetAsyncKeyState('K') && GetAsyncKeyState('J'))
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//À§
		else if(GetAsyncKeyState('I') & 0x8000)
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//¾Æ·¡
		else if(GetAsyncKeyState('K') & 0x8000)
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//¿ÞÂÊ
		else if(GetAsyncKeyState('J') & 0x8000)
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		//¿À¸¥ÂÊ
		else if(GetAsyncKeyState('L') & 0x8000)
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

			if(GetAsyncKeyState('H'))
			{
				if(m_bAttackBtn == false)
				{
					m_pSoundMgr->Hit2(L"Hit2.wav");
					m_bAttackBtn = true;
				}			
			}

			Ani_LegWalk();

			if(m_bAttackBtn == false)
				Ani_WalkHandShake();
		}
		else if(GetAsyncKeyState('H'))
		{
			if(m_bAttackBtn == false)
			{
				m_pSoundMgr->Hit2(L"Hit2.wav");
				m_bAttackBtn = true;
			}			
		}
		else if(GetAsyncKeyState('N'))	//Æ¨°Ü³»±â
		{
			if(m_bReflectAni == false)
			{
				m_bReflectAni = true;
			}
		}
	}

	if(GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_pTerrainCol->GetJumpAble() = true;
		m_pTerrainCol->GetCollision() = false;
	}	

	if(GetAsyncKeyState('5') & 0x8000)
	{
		if(m_bSkillUse == false)
		{
			m_pSoundMgr->EarthBoom(L"FiveSkill.wav");
			m_bSkillUse = true;
			m_fSkillTime = 0.f;
		}

		if(m_bPaticleMake == false)
			Skill_1_Effect();

		((CPlayer1_Skill*)ENGINE::CLayerMgr::GetInstance()->GetGameObject(L"Player Skill1", ENGINE::LAYER_UI))->GetClick() = true;
		m_bSkill_1_Use = true;
	}

	if(GetAsyncKeyState('6') & 0x8000)
	{
		if(m_bSkillUse == false)
		{
			m_pSoundMgr->Energy(L"Energing.wav");
			//m_pSoundMgr->PlaySound(L"SixSkill.wav");
			m_bSkillUse = true;
			m_fSkillTime = 0.f;
		}

		if(m_bPaticleMake == false)
		{
			m_fSkill2_Size = 1.5f;
			m_pParticle2 = CSkillParticle::Create(m_pDevice);
			dynamic_cast<CSkillParticle*>(m_pParticle2)->SetColor(32.f/255.f, 178.f/255.f, 170.f/255.f, 0.1f);
			dynamic_cast<CSkillParticle*>(m_pParticle2)->SetSize(m_fSkill2_Size, m_fSkill2_Size);
			dynamic_cast<CSkillParticle*>(m_pParticle2)->SetPos(m_pInfo->m_vPos);
			m_pLayerMgr->AddObject(L"Skill_2_Effect", m_pParticle2, ENGINE::LAYER_GAMELOGIC);
			dynamic_cast<CSkillParticle*>(m_pParticle2)->SetAni();

			m_bPaticleMake = true;
			m_bSkill_2_Pos = m_pInfo->m_vPos;
		}

		((CPlayer1_Skill2*)ENGINE::CLayerMgr::GetInstance()->GetGameObject(L"Player Skill2", ENGINE::LAYER_UI))->GetClick() = true;
		m_bSkill_2_Use = true;
	}
}

void CPlayer_Isildur::SetBodyPos(D3DXVECTOR3& pos)
{
	D3DXVECTOR3 pMovePos = pos - m_pInfo->m_vPos;

	SetPos(pos);

	for(int i=0; i<ENGINE::BP_END; ++i)
	{
		m_pBodyPart[i]->GetInfo()->m_vPos += pos;
	}
}

void CPlayer_Isildur::BodyAnimation(void)
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

void CPlayer_Isildur::Jump(void)
{
	if(m_pTerrainCol->GetJumpAble() == true)
	{
		m_pTerrainCol->GetCollision() = false;
		m_pInfo->m_vPos.y += 0.098 * m_Gravity * 3.f;

	}

	if(m_pInfo->m_vPos.y < 2.f)
	{
		m_pTerrainCol->GetCollision() = true;
		m_pInfo->m_vPos.y = 0;
		m_Gravity = 0;
		m_pTerrainCol->GetJumpAble() = false;
	}
}

void CPlayer_Isildur::Ani_Idle(void)
{
	//¿À¸¥, ¿ÞÆÈ µé½â.
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

	//¿À¸¥, ¿ÞÆÈ ¸öÀ» Áß½ÉÀ¸·Î °øÀü
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(45.f));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(45.f));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_UP])->GetMatrix())[ENGINE::MAT_AXIS], &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(45.f));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_ARM_DOWN])->GetMatrix())[ENGINE::MAT_AXIS], &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(45.f));

	//¸Ó¸®
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

	//¸öÅë È¸Àü, µé½â
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

	//¿Þ, ¿À¸¥ ´Ù¸® ¹ú¸®±â.
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

void CPlayer_Isildur::Ani_Attack(void)
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


void CPlayer_Isildur::BoomCrush(void)
{
	m_fCrushTime += 1.f * m_pTimeMgr->GetTime();

	for(int i = 0; i < ENGINE::BP_END; ++i)
	{
		if(m_pTerrainCol->GetJumpAble())
		{			
			m_pDyingBodyPart[i]->GetInfo()->m_vPos.y -= 0.098f * m_Gravity * 0.5f;
		}
	}

	//¿À¸¥ÆÈ ¾î±ú
	m_pDyingBodyPart[ENGINE::BP_R_ARM_UP]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = 85.f;
	D3DXVec3Cross(&m_pDyingBodyPart[ENGINE::BP_R_ARM_UP]->GetInfo()->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
	//¿À¸¥ÆÈ ¼Õ
	m_pDyingBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = 85.f;
	//¿ÞÆÈ ¾î±ú
	m_pDyingBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = -85.f;
	D3DXVec3Cross(&m_pDyingBodyPart[ENGINE::BP_L_ARM_UP]->GetInfo()->m_vDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
	//¿ÞÆÈ ¼Õ
	m_pDyingBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_fAngle[ENGINE::ANGLE_X] = -85.f;
	D3DXVECTOR3 vRight;
	D3DXVec3Cross(&vRight, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
	D3DXVec3Cross(&m_pDyingBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &vRight);
	//¸Ó¸®
	D3DXVec3Cross(&vRight, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);
	D3DXVec3Cross(&m_pDyingBodyPart[ENGINE::BP_HEAD]->GetInfo()->m_vDir, &m_pInfo->m_vDir, &vRight);
	//¸öÅë
	//¿À¸¥´Ù¸®
	m_pDyingBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = -85.f;
	D3DXVec3Cross(&m_pDyingBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vDir, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f));
	//¿Þ´Ù¸®
	m_pDyingBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_fAngle[ENGINE::ANGLE_Z] = 85.f;
	D3DXVec3Cross(&m_pDyingBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vDir, &D3DXVECTOR3(0.f, 1.f, 0.f), &m_pInfo->m_vDir);

	/*m_pBodyPart[ENGINE::BP_HEAD]->GetInfo()->m_vDir = m_pBodyPart[ENGINE::BP_HEAD]->GetInfo()->m_vPos - m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos;
	m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vDir = m_pBodyPart[ENGINE::BP_BODY]->GetInfo()->m_vPos - m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos;
	m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vDir = m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos - m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos;
	m_pBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_vDir = m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos - m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos;
	m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vDir = m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos - m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos;*/

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

	
	//m_pBodyPart[ENGINE::BP_HEAD]->GetInfo()->m_vPos = m_pBodyPart[ENGINE::BP_HEAD]->GetInfo()->m_vDir * m_Gravity * 0.01f * m_pTimeMgr->GetTime();
	//m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vPos = m_pBodyPart[ENGINE::BP_R_LEG]->GetInfo()->m_vDir * m_Gravity * 0.01f * m_pTimeMgr->GetTime();
	//m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vPos = m_pBodyPart[ENGINE::BP_L_LEG]->GetInfo()->m_vDir * m_Gravity * 0.01f * m_pTimeMgr->GetTime();
	//m_pBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_vPos = m_pBodyPart[ENGINE::BP_L_ARM_DOWN]->GetInfo()->m_vDir * m_Gravity * 0.01f * m_pTimeMgr->GetTime();
	//m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos = m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vDir * m_Gravity * 0.01f * m_pTimeMgr->GetTime();

	
	/*for(int i = 0; i < ENGINE::BP_END; ++i)
	{
		m_pBodyPart[i]->GetInfo()->m_vPos.x = fRand * m_pBodyPart[i]->GetInfo()->m_vDir.x * m_Gravity;
		m_pBodyPart[i]->GetInfo()->m_vPos.y = fRand * m_pBodyPart[i]->GetInfo()->m_vDir.y * m_Gravity;
		m_pBodyPart[i]->GetInfo()->m_vPos.z = fRand * m_pBodyPart[i]->GetInfo()->m_vDir.z * m_Gravity;

		if(m_pBodyPart[i]->GetInfo()->m_vPos.y > 1.6f)
			m_pTerrainCol->GetCollision() = false;
	}*/

	if(m_fCrushTime > 3.f)
	{
		m_bCrush = false;
		m_pTerrainCol->GetJumpAble() = false;
	}
}

void CPlayer_Isildur::Ani_LegWalk(void)
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

void CPlayer_Isildur::Ani_WalkHandShake(void)
{
	//ENGINE::VTXCUBE* CubeVtx =  dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_BODY])->GetConvertVertex();
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

void CPlayer_Isildur::Ani_Sword(void)
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

void CPlayer_Isildur::Ani_Reflection(void)
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
	m_fReflectAngle += 25.f;

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pSword)->GetMatrix())[ENGINE::MAT_AXIS], &Sword_Cross, D3DXToRadian(m_fReflectAngle));

	if(m_fReflectAngle >= 600.f)
	{
		m_bReflectAni = false;
		m_fReflectAngle = 0.f;
	}
}

void CPlayer_Isildur::Ani_SkillUse(void)
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

void CPlayer_Isildur::AttMode(void)
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
					m_pUnitCol->SetColInfo(&m_pInfo->m_vPos, &((CMonster_Orc*)m_pLayerMgr->GetGameObject(L"Monser_Orc", ENGINE::LAYER_MONSTER))->GetInfo()->m_vPos/*°ø°ÝÇÏ´Â »ó´ëÀÇ Æ÷½º*/);

					((CMonster_Orc*)(*iter))->GetAttHp() -= 1;
					//cout << "³Í ±ð¿©¾ßÇÑ´Ù. " << endl;

					m_pSoundMgr->PlayerAtt(L"PlayerAtt2.wav");

					m_fAttTime = 0.f;

					// HIT ÀÌÆåÆ®
					ENGINE::CGameObject* pGameObject = CHitParticle::Create(m_pDevice, vCenter);
					ENGINE::CLayerMgr::GetInstance()->AddObject(L"HitParticle", pGameObject, ENGINE::LAYER_GAMELOGIC);
					dynamic_cast<CHitParticle*>(pGameObject)->SetAni();
				}
			}
		}

		m_fAttTime = 0.f;
	}
}

void CPlayer_Isildur::Skill_1_Explosion(void)
{
	m_fSkill_1_Time += 1.f * m_pTimeMgr->GetTime();

	D3DXVECTOR3 vecLength;
	float fLength = 0.f;

	list<ENGINE::CGameObject*>::iterator iter		= m_pLayerMgr->GetObjList(L"Dumy_Orc", ENGINE::LAYER_MONSTER)->begin();
	list<ENGINE::CGameObject*>::iterator iter_end	= m_pLayerMgr->GetObjList(L"Dumy_Orc", ENGINE::LAYER_MONSTER)->end();

	for(; iter != iter_end; ++iter)
	{
		vecLength = dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos -  m_pInfo->m_vPos;

		fLength = D3DXVec3Length(&vecLength);

		if(fLength <= m_fSkill_1_Length)
			dynamic_cast<CMonster_Orc*>(*iter)->GetAttHp() = 0;
	}

	iter		= m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->begin();
	iter_end	= m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->end();
																			   
	for(; iter != iter_end; ++iter)
	{
		vecLength = dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos -  m_pInfo->m_vPos;

		fLength = D3DXVec3Length(&vecLength);

		if(fLength <= m_fSkill_1_Length)
			dynamic_cast<CMonster_Orc*>(*iter)->GetAttHp() = 0;
	}

	if(m_fSkill_1_Time > 2.f)
	{
		m_bPaticleMake = false;
		m_bSkill_1_Use = false;
		m_fSkill_1_Time = 0.f;
		m_fSkill_1_Length = 0.5f;

		for(int i=0; i<8; ++i)
		{
			list<ENGINE::CGameObject*>::iterator iter		= m_pLayerMgr->GetObjList(L"Skill_1_Effect", ENGINE::LAYER_GAMELOGIC)->begin();
			list<ENGINE::CGameObject*>::iterator iter_end	= m_pLayerMgr->GetObjList(L"Skill_1_Effect", ENGINE::LAYER_GAMELOGIC)->end();

			for(; iter != iter_end; ++iter)
				(*iter)->GetAlive() = false;
		}
	}
	else
	{
		m_fSkill_1_Length += 0.1f;

		dynamic_cast<CSkillParticle*>(m_pParticle[0])->SetPos(D3DXVECTOR3(m_bSkill_1_Pos.x, m_bSkill_1_Pos.y, m_bSkill_1_Pos.z + m_fSkill_1_Length));

		dynamic_cast<CSkillParticle*>(m_pParticle[1])->SetPos(D3DXVECTOR3(m_bSkill_1_Pos.x + (m_fSkill_1_Length/2.f), m_bSkill_1_Pos.y, m_bSkill_1_Pos.z + (m_fSkill_1_Length/2.f)));
		
		dynamic_cast<CSkillParticle*>(m_pParticle[2])->SetPos(D3DXVECTOR3(m_bSkill_1_Pos.x + m_fSkill_1_Length, m_bSkill_1_Pos.y, m_bSkill_1_Pos.z));

		dynamic_cast<CSkillParticle*>(m_pParticle[3])->SetPos(D3DXVECTOR3(m_bSkill_1_Pos.x + (m_fSkill_1_Length/2.f), m_bSkill_1_Pos.y, m_bSkill_1_Pos.z - (m_fSkill_1_Length/2.f)));

		dynamic_cast<CSkillParticle*>(m_pParticle[4])->SetPos(D3DXVECTOR3(m_bSkill_1_Pos.x, m_bSkill_1_Pos.y, m_bSkill_1_Pos.z - m_fSkill_1_Length));
		
		dynamic_cast<CSkillParticle*>(m_pParticle[5])->SetPos(D3DXVECTOR3(m_bSkill_1_Pos.x - (m_fSkill_1_Length/2.f), m_bSkill_1_Pos.y, m_bSkill_1_Pos.z - (m_fSkill_1_Length/2.f)));
		
		dynamic_cast<CSkillParticle*>(m_pParticle[6])->SetPos(D3DXVECTOR3(m_bSkill_1_Pos.x - m_fSkill_1_Length, m_bSkill_1_Pos.y, m_bSkill_1_Pos.z));
		
		dynamic_cast<CSkillParticle*>(m_pParticle[7])->SetPos(D3DXVECTOR3(m_bSkill_1_Pos.x - (m_fSkill_1_Length/2.f), m_bSkill_1_Pos.y, m_bSkill_1_Pos.z + (m_fSkill_1_Length/2.f)));
	}
}

void CPlayer_Isildur::Skill_2_Slash(void)
{
	if(m_bSkill_2_Fase1 == false)
	{
		m_fSkill_2_Time += 1.f * m_pTimeMgr->GetTime();
	
		m_fSkill2_Size += 0.05f;

		dynamic_cast<CSkillParticle*>(m_pParticle2)->SetSize(m_fSkill2_Size, m_fSkill2_Size);

		if(m_fSkill_2_Time > 1.5f)
		{
			m_bSkill_2_Fase1 = true;
			m_fSkill_2_Time = 0.f;
		}
	}
	else if(m_bSkill_2_Fase2 == false)
	{
		m_fSkill_2_Time += 1.f * m_pTimeMgr->GetTime();

		m_fSkill2_Size -= 0.06f;

		dynamic_cast<CSkillParticle*>(m_pParticle2)->SetSize(m_fSkill2_Size, m_fSkill2_Size);

		if(m_fSkill_2_Time > 1.5f)
		{
			m_pSoundMgr->PlaySound(L"SixSkill.wav");
			m_bSkill_2_Fase2 = true;
			m_fSkill_2_Time = 0.f;
		}
	}
	else if(m_bSkill_2_Fase3 == false)
	{
		
		m_fSkill_2_Time += 1.f * m_pTimeMgr->GetTime();

		m_fSkill2_Size += 0.1f;
		m_bSkill_2_Pos.x += 0.5f;
	
		dynamic_cast<CSkillParticle*>(m_pParticle2)->SetSize(m_fSkill2_Size, m_fSkill2_Size);
		dynamic_cast<CSkillParticle*>(m_pParticle2)->SetPos(m_bSkill_2_Pos);

		D3DXVECTOR3 vecLength;
		float fLength = 0.f;

		list<ENGINE::CGameObject*>::iterator iter		= m_pLayerMgr->GetObjList(L"Dumy_Orc", ENGINE::LAYER_MONSTER)->begin();
		list<ENGINE::CGameObject*>::iterator iter_end	= m_pLayerMgr->GetObjList(L"Dumy_Orc", ENGINE::LAYER_MONSTER)->end();

		for(; iter != iter_end; ++iter)
		{
			vecLength = dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos -  m_bSkill_2_Pos;

			fLength = D3DXVec3Length(&vecLength);

			if(fLength <= 5.f)
				dynamic_cast<CMonster_Orc*>(*iter)->GetAttHp() = 0;
		}

		iter		= m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->begin();
		iter_end	= m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->end();

		for(; iter != iter_end; ++iter)
		{
			vecLength = dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos - m_bSkill_2_Pos;

			fLength = D3DXVec3Length(&vecLength);

			if(fLength <= 5.f)
				dynamic_cast<CMonster_Orc*>(*iter)->GetAttHp() = 0;
		}		

		if(m_fSkill_2_Time > 2.f)
		{
			m_bSkill_2_Fase3 = false;
			m_bSkill_2_Fase2 = false;
			m_bSkill_2_Fase1 = false;
			m_bPaticleMake = false;
			m_bSkill_2_Use = false;
			m_fSkill_2_Time = 0.f;

			m_pLayerMgr->GetGameObject(L"Skill_2_Effect", ENGINE::LAYER_GAMELOGIC)->GetAlive() = false;
		}
	}
}

void CPlayer_Isildur::DeletePlayer(void)
{
	float m_fTime = m_pTimeMgr->GetTime();

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

	if(GetAsyncKeyState('H'))
	{
		if(m_bAttackBtn == false)
		{
			m_bAttackBtn = true;
		}			
	}

	Ani_LegWalk();

	if(m_bAttackBtn == false)
		Ani_WalkHandShake();
}

void CPlayer_Isildur::AttBoss(void)
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
					m_pUnitCol->SetColInfo(&m_pInfo->m_vPos, &((CBoss_1*)m_pLayerMgr->GetGameObject(L"Stage1_Boss", ENGINE::LAYER_MONSTER))->GetInfo()->m_vPos/*°ø°ÝÇÏ´Â »ó´ëÀÇ Æ÷½º*/);

					((CBoss_1*)(*iter))->GetAttHp() -= 1;
					//cout << "³Í ±ð¿©¾ßÇÑ´Ù. " << endl;

					m_fAttTime = 0.f;

					// HIT ÀÌÆåÆ®
					ENGINE::CGameObject* pGameObject = CHitParticle::Create(m_pDevice, vCenter);
					ENGINE::CLayerMgr::GetInstance()->AddObject(L"HitParticle", pGameObject, ENGINE::LAYER_GAMELOGIC);
					dynamic_cast<CHitParticle*>(pGameObject)->SetAni();
				}
			}
		}

		m_fAttTime = 0.f;
	}
}

void CPlayer_Isildur::Skill_1_Effect(void)
{
	m_pParticle[0] = CSkillParticle::Create(m_pDevice);
	dynamic_cast<CSkillParticle*>(m_pParticle[0])->SetColor(64.f/255.f, 224.f/255.f, 208.f/255.f, 0.1f);
	dynamic_cast<CSkillParticle*>(m_pParticle[0])->SetSize(2.f, 2.f);
	dynamic_cast<CSkillParticle*>(m_pParticle[0])->SetPos(m_pInfo->m_vPos);
	m_pLayerMgr->AddObject(L"Skill_1_Effect", m_pParticle[0], ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CSkillParticle*>(m_pParticle[0])->SetAni();

	m_pParticle[1] = CSkillParticle::Create(m_pDevice);
	dynamic_cast<CSkillParticle*>(m_pParticle[1])->SetColor(64.f/255.f, 224.f/255.f, 208.f/255.f, 0.1f);
	dynamic_cast<CSkillParticle*>(m_pParticle[1])->SetSize(2.f, 2.f);
	dynamic_cast<CSkillParticle*>(m_pParticle[1])->SetPos(m_pInfo->m_vPos);
	m_pLayerMgr->AddObject(L"Skill_1_Effect", m_pParticle[1], ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CSkillParticle*>(m_pParticle[1])->SetAni();

	m_pParticle[2] = CSkillParticle::Create(m_pDevice);
	dynamic_cast<CSkillParticle*>(m_pParticle[2])->SetColor(64.f/255.f, 224.f/255.f, 208.f/255.f, 0.1f);
	dynamic_cast<CSkillParticle*>(m_pParticle[2])->SetSize(2.f, 2.f);
	dynamic_cast<CSkillParticle*>(m_pParticle[2])->SetPos(m_pInfo->m_vPos);
	m_pLayerMgr->AddObject(L"Skill_1_Effect", m_pParticle[2], ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CSkillParticle*>(m_pParticle[2])->SetAni();

	m_pParticle[3] = CSkillParticle::Create(m_pDevice);
	dynamic_cast<CSkillParticle*>(m_pParticle[3])->SetColor(64.f/255.f, 224.f/255.f, 208.f/255.f, 0.1f);
	dynamic_cast<CSkillParticle*>(m_pParticle[3])->SetSize(2.f, 2.f);
	dynamic_cast<CSkillParticle*>(m_pParticle[3])->SetPos(m_pInfo->m_vPos);
	m_pLayerMgr->AddObject(L"Skill_1_Effect", m_pParticle[3], ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CSkillParticle*>(m_pParticle[3])->SetAni();

	m_pParticle[4] = CSkillParticle::Create(m_pDevice);
	dynamic_cast<CSkillParticle*>(m_pParticle[4])->SetColor(64.f/255.f, 224.f/255.f, 208.f/255.f, 0.1f);
	dynamic_cast<CSkillParticle*>(m_pParticle[4])->SetSize(2.f, 2.f);
	dynamic_cast<CSkillParticle*>(m_pParticle[4])->SetPos(m_pInfo->m_vPos);
	m_pLayerMgr->AddObject(L"Skill_1_Effect", m_pParticle[4], ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CSkillParticle*>(m_pParticle[4])->SetAni();

	m_pParticle[5] = CSkillParticle::Create(m_pDevice);
	dynamic_cast<CSkillParticle*>(m_pParticle[5])->SetColor(64.f/255.f, 224.f/255.f, 208.f/255.f, 0.1f);
	dynamic_cast<CSkillParticle*>(m_pParticle[5])->SetSize(2.f, 2.f);
	dynamic_cast<CSkillParticle*>(m_pParticle[5])->SetPos(m_pInfo->m_vPos);
	m_pLayerMgr->AddObject(L"Skill_1_Effect", m_pParticle[5], ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CSkillParticle*>(m_pParticle[5])->SetAni();

	m_pParticle[6] = CSkillParticle::Create(m_pDevice);
	dynamic_cast<CSkillParticle*>(m_pParticle[6])->SetColor(64.f/255.f, 224.f/255.f, 208.f/255.f, 0.1f);
	dynamic_cast<CSkillParticle*>(m_pParticle[6])->SetSize(2.f, 2.f);
	dynamic_cast<CSkillParticle*>(m_pParticle[6])->SetPos(m_pInfo->m_vPos);
	m_pLayerMgr->AddObject(L"Skill_1_Effect", m_pParticle[6], ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CSkillParticle*>(m_pParticle[6])->SetAni();

	m_pParticle[7] = CSkillParticle::Create(m_pDevice);
	dynamic_cast<CSkillParticle*>(m_pParticle[7])->SetColor(64.f/255.f, 224.f/255.f, 208.f/255.f, 0.1f);
	dynamic_cast<CSkillParticle*>(m_pParticle[7])->SetSize(2.f, 2.f);
	dynamic_cast<CSkillParticle*>(m_pParticle[7])->SetPos(m_pInfo->m_vPos);
	m_pLayerMgr->AddObject(L"Skill_1_Effect", m_pParticle[7], ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CSkillParticle*>(m_pParticle[7])->SetAni();

	m_bSkill_1_Pos = m_pInfo->m_vPos;
	m_bPaticleMake = true;
}
