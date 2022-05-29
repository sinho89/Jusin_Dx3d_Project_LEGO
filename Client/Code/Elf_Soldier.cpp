#include "stdafx.h"
#include "Elf_Soldier.h"

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

CElf_Soldier::CElf_Soldier(LPDIRECT3DDEVICE9 pDevice)
: CUnit(pDevice)
, m_pLayerMgr(ENGINE::Get_LayerMgr()), m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL), m_pTerrainVertex(NULL), m_pSword(NULL)
, m_bSwordIdle(false), m_bArmIdle(false), m_bHeadIdle(false), m_bBodyIdle(false), m_bLegIdle(false)
, m_bSwordAttack(false), m_fAttackArmAngle(0.f), m_bSkillUse(false), m_fSkillTime(false), m_bAttackBtn(false)
, m_fAttackTime(0.f), m_fSkillUseTime(0.f)
{
}

CElf_Soldier::CElf_Soldier(LPDIRECT3DDEVICE9 pDevice, NPCTYPE _eNPCType)
: CUnit(pDevice)
, m_pLayerMgr(ENGINE::Get_LayerMgr()), m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_pTerrainCol(NULL), m_pTerrainVertex(NULL), m_pSword(NULL)
, m_bSwordIdle(false), m_bArmIdle(false), m_bHeadIdle(false), m_bBodyIdle(false), m_bLegIdle(false)
, m_bSwordAttack(false), m_fAttackArmAngle(0.f), m_bSkillUse(false), m_fSkillTime(false), m_bAttackBtn(false)
, m_eNPCType(_eNPCType), m_fAttackTime(0.f), m_fSkillUseTime(0.f)
{
}

CElf_Soldier::~CElf_Soldier(void)
{
	Release();
}

HRESULT CElf_Soldier::Initialize(void)
{
	m_fSpeed = 10.f;

	m_iEffectRand = 0;
	m_bEffect = false;
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

	if(m_eNPCType == NPC_SOLDIER)
	{
		m_pSword = CSword::Create(m_pDevice, CSword::UT_PLAYER);
	}

	CloseHandle(hFile);

	FAILED_CHECK(AddComponent());



	return S_OK;
}

HRESULT CElf_Soldier::AddComponent(void)
{
	CUnit::AddComponent();

	return S_OK;
}

void CElf_Soldier::Update(void)
{
	CUnit::Update();

	BodyAnimation();

	if(m_eNPCType == NPC_SOLDIER)
	{
		m_fAttackTime += 1.f * m_pTimeMgr->GetTime();

		if(m_fAttackTime > 0.2f)
		{
			Ani_Attack();
		}
	}
	else if(m_eNPCType == NPC_HEALER)
	{
		m_fSkillUseTime += 1.f * m_pTimeMgr->GetTime();

		if(m_fSkillUseTime > 3.f)
		{
			Ani_SkillUse();			
		}
	}

	D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], D3DXToRadian(-90.f));
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

	for(int i=0; i<ENGINE::BP_END; ++i)
		m_pBodyPart[i]->Update();


	if(m_eNPCType == NPC_SOLDIER)
	{
		(dynamic_cast<CCubeObject*>(m_pSword)->GetMatrix())[ENGINE::MAT_PARENT] = m_mMat[ENGINE::MAT_PARENT];

		m_pSword->Update();

		if(m_bAttackBtn == false && m_bSkillUse == false)
			Ani_Sword();
	}
	else if(m_eNPCType == NPC_HEALER)
	{

	}

	ENGINE::CGameObject::Update();

	m_eUnitStateType = ENGINE::US_IDLE;
}

void CElf_Soldier::Render(void)
{
	for(int i=0; i<ENGINE::BP_END; ++i)
		m_pBodyPart[i]->Render();

	if(m_eNPCType == NPC_SOLDIER)
	{
		m_pSword->Render();
	}
}

void CElf_Soldier::Release(void)
{

}

CElf_Soldier* CElf_Soldier::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CElf_Soldier* pElf_Soldier = new CElf_Soldier(pDevice);

	if(FAILED(pElf_Soldier->Initialize()))
		ENGINE::Safe_Delete(pElf_Soldier);

	return pElf_Soldier;
}

CElf_Soldier* CElf_Soldier::Create(LPDIRECT3DDEVICE9 pDevice, NPCTYPE _eNPCType)
{
	CElf_Soldier* pElf_Soldier = new CElf_Soldier(pDevice, _eNPCType);

	if(FAILED(pElf_Soldier->Initialize()))
		ENGINE::Safe_Delete(pElf_Soldier);

	return pElf_Soldier;
}

void CElf_Soldier::CreateBodyPart(ENGINE::SAVECUBE* pSaveCube)
{
	switch(pSaveCube->m_SaveType)
	{
	case ENGINE::SOBJ_HEAD:
		m_pBodyPart[ENGINE::BP_HEAD] = CHead::Create(m_pDevice, CHead::UT_ELF_SOLDIER);
		m_pBodyPart[ENGINE::BP_HEAD]->SetInfo(pSaveCube);
		dynamic_cast<CHead*>(m_pBodyPart[ENGINE::BP_HEAD])->SetInterver(pSaveCube);		
		//m_pLayerMgr->AddObject(L"Isildur_Head", m_pBodyPart[ENGINE::BP_HEAD], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_BODY:
		m_pBodyPart[ENGINE::BP_BODY] = CBody::Create(m_pDevice, CBody::UT_ELF_SOLDIER);
		m_pBodyPart[ENGINE::BP_BODY]->SetInfo(pSaveCube);
		dynamic_cast<CBody*>(m_pBodyPart[ENGINE::BP_BODY])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_Body", m_pBodyPart[ENGINE::BP_BODY], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_L_ARM_UP:
		m_pBodyPart[ENGINE::BP_L_ARM_UP] = CLeftArm_Up::Create(m_pDevice, CLeftArm_Up::UT_ELF_SOLDIER);
		m_pBodyPart[ENGINE::BP_L_ARM_UP]->SetInfo(pSaveCube);
		dynamic_cast<CLeftArm_Up*>(m_pBodyPart[ENGINE::BP_L_ARM_UP])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_L_Arm_Up", m_pBodyPart[ENGINE::BP_L_ARM_UP], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_L_ARM_DOWN:
		m_pBodyPart[ENGINE::BP_L_ARM_DOWN] = CLeftArm_Down::Create(m_pDevice, CLeftArm_Down::UT_ELF_SOLDIER);		
		m_pBodyPart[ENGINE::BP_L_ARM_DOWN]->SetInfo(pSaveCube);
		dynamic_cast<CLeftArm_Down*>(m_pBodyPart[ENGINE::BP_L_ARM_DOWN])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_L_Arm_Down", m_pBodyPart[ENGINE::BP_L_ARM_DOWN], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_R_ARM_UP:
		m_pBodyPart[ENGINE::BP_R_ARM_UP] = CRightArm_Up::Create(m_pDevice, CRightArm_Up::UT_ELF_SOLDIER);
		m_pBodyPart[ENGINE::BP_R_ARM_UP]->SetInfo(pSaveCube);
		dynamic_cast<CRightArm_Up*>(m_pBodyPart[ENGINE::BP_R_ARM_UP])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_R_Arm_Up", m_pBodyPart[ENGINE::BP_R_ARM_UP], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_R_ARM_DOWN:
		m_pBodyPart[ENGINE::BP_R_ARM_DOWN] = CRightArm_Down::Create(m_pDevice, CRightArm_Down::UT_ELF_SOLDIER);
		m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->SetInfo(pSaveCube);
		dynamic_cast<CRightArm_Down*>(m_pBodyPart[ENGINE::BP_R_ARM_DOWN])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_R_Arm_Down", m_pBodyPart[ENGINE::BP_R_ARM_DOWN], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_L_LEG:
		m_pBodyPart[ENGINE::BP_L_LEG] = CLeftLeg::Create(m_pDevice, CLeftLeg::UT_ELF_SOLDIER);
		m_pBodyPart[ENGINE::BP_L_LEG]->SetInfo(pSaveCube);
		dynamic_cast<CLeftLeg*>(m_pBodyPart[ENGINE::BP_L_LEG])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_L_Leg", m_pBodyPart[ENGINE::BP_L_LEG], ENGINE::LAYER_GAMELOGIC);
		break;

	case ENGINE::SOBJ_R_LEG:
		m_pBodyPart[ENGINE::BP_R_LEG] = CRightLeg::Create(m_pDevice, CRightLeg::UT_ELF_SOLDIER);
		m_pBodyPart[ENGINE::BP_R_LEG]->SetInfo(pSaveCube);
		dynamic_cast<CRightLeg*>(m_pBodyPart[ENGINE::BP_R_LEG])->SetInterver(pSaveCube);
		//m_pLayerMgr->AddObject(L"Isildur_R_Leg", m_pBodyPart[ENGINE::BP_R_LEG], ENGINE::LAYER_GAMELOGIC);
		break;
	}
}

void CElf_Soldier::SetBodyPos(D3DXVECTOR3& pos)
{
	SetPos(pos);

	for(int i=0; i<ENGINE::BP_END; ++i)
	{
		m_pBodyPart[i]->GetInfo()->m_vPos += pos;
	}
}

void CElf_Soldier::BodyAnimation(void)
{
	switch(m_eUnitStateType)
	{
	case ENGINE::US_IDLE:
		Ani_Idle();
		break;

	case ENGINE::US_ATTACK:
		Ani_Attack();
		break;

	case ENGINE::US_END:
		break;
	}
}

void CElf_Soldier::Ani_Idle(void)
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
	}

	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_L_LEG])->GetMatrix())[ENGINE::MAT_AXIS], &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(40.f));
	D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pBodyPart[ENGINE::BP_R_LEG])->GetMatrix())[ENGINE::MAT_AXIS], &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(40.f));

	if(m_eNPCType == NPC_SOLDIER)
	{
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
}

void CElf_Soldier::Ani_Attack(void)
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
		m_fAttackTime = 0.f;
	}
}

void CElf_Soldier::Ani_Sword(void)
{
	if(m_eUnitStateType == ENGINE::US_IDLE)
	{
		D3DXVECTOR3 X_Cross;
		D3DXVec3Cross(&X_Cross, &m_pInfo->m_vDir, &D3DXVECTOR3(0.f, -1.f, 0.f));

		D3DXMatrixRotationAxis(&(dynamic_cast<CCubeObject*>(m_pSword)->GetMatrix())[ENGINE::MAT_AXIS], &X_Cross, D3DXToRadian(-55.f));

		m_pSword->GetInfo()->m_vPos = D3DXVECTOR3(m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.x + 0.4f,
			m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.y + 0.5f,
			m_pBodyPart[ENGINE::BP_R_ARM_DOWN]->GetInfo()->m_vPos.z - 0.5f);
	}
}

void CElf_Soldier::Ani_SkillUse(void)
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
		m_fSkillUseTime = 0.f;
	}
}


