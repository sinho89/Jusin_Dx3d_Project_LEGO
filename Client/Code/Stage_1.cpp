#include "stdafx.h"
#include "Stage_1.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "Pipeline.h"
#include "Component.h"
#include "Transform.h"
#include "LayerMgr.h"

#include "Terrain.h"
#include "Player.h"
#include "SkyBox.h"
#include "UI.h"
#include "Player_Isildur.h"
#include "CollisionMgr.h"
#include "Player_Elond.h"
#include "Elf_Soldier.h"

#include "Heart.h"
#include "Monster_Orc.h"
#include "PlayerGround.h"
#include "PlayerGround2.h"
#include "Lhpclass.h"
#include "UI2.h"
#include "Heart2.h"
#include "Player1Skill.h"
#include "Player1Skill2.h"
#include "Player2Skill.h"
#include "Player2Skill2.h"
#include "Petal.h"
#include "AttParticle.h"
#include "SkillParticle.h"
#include "HitParticle.h"
#include "Boss_1.h"
#include "Metheo.h"
#include "BossApearParticle.h"
#include "SoundMgr.h"

#include "FieldFire.h"

CStage_1::CStage_1(LPDIRECT3DDEVICE9 pDevice)
: CClientScene(pDevice)
, m_pBossAppear(false), m_bIsildurSound(false), m_bCrowl(false), fCrowTime(0.f)
, m_pElond(NULL), m_pIsildur(NULL)
{

}

CStage_1::~CStage_1(void)
{
	Release();
}

HRESULT CStage_1::Initialize(void)
{

	m_pSoundMgr->LoadSoundFile();

	CCollisionMgr::GetInstance()->AddColObject(CCollisionMgr::COL_TERRAIN);	

	m_pSoundMgr->PlayBGMSound(L"LordOfTheRing.ogg");

	CreateTerrain();

	AddBuffer();
	AddTexture();
	AddObject();

	SetCameraToPlayer();

	m_pLayerMgr->SetDivisionType(ENGINE::CLayerMgr::DIV_SINGLE);
	m_pLayerMgr->GetObjList(L"Player Heart", ENGINE::LAYER_UI)->reverse();

	m_fTime = 0;
	m_fOriTime = 5;
	m_fCount = 2;

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.25f, // 45 - degree
		ENGINE::WINCX / ENGINE::WINCY,
		1.0f,
		1000.0f);

	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj);	

	m_pCameraMgr->Stage1InitCam();

	m_bFieldFire	= false;
	m_fFieldFire	= 0.f;

	return S_OK;	
}

void CStage_1::Update(void)
{
	SetCameraToPlayer();

	m_fTime += 0.1f * m_pTimeMgr->GetTime();

	m_fFieldFire += 0.1f * m_pTimeMgr->GetTime();

	fCrowTime += 0.1f * m_pTimeMgr->GetTime();


	if(m_fTime >= 1.5f )
	{
		if(!m_bIsildurSound)
		{
			m_pSoundMgr->IsildurSound(L"IsildurSound.wav");

			m_bIsildurSound = true;
			m_fTime = 0;
		}
	}
	if(fCrowTime >= 1.7f)
	{
		if(!m_bCrowl)
		{
			m_pSoundMgr->Crowing(L"Crowing.wav");
			m_bCrowl = true;
			fCrowTime = 0;
		}
	}	

	m_pCameraMgr->SetTime(m_pTimeMgr->GetTime());
	m_pCameraMgr->OpeningAction(m_pTimeMgr->GetTime());
	m_pCameraMgr->DivisionCamera();
	m_pCameraMgr->Stage1BasicView();
	m_pCameraMgr->Stage1BossShakeCamera();


	if(m_pCameraMgr->Stage1BossAction(m_pTimeMgr->GetTime()))
	{
		m_fTime += 1.f * m_pTimeMgr->GetTime();
		if(m_fTime >= 2.f)
			m_pSoundMgr->EarthBoom(L"EarthBoom.wav");
		ENGINE::CGameObject* pGameObject = CAttParticle::Create(m_pDevice);
		m_pLayerMgr->AddObject(L"VolcanoParticle", pGameObject, ENGINE::LAYER_GAMELOGIC);
		dynamic_cast<CAttParticle*>(pGameObject)->SetAni();
	}

	
	

	//list<ENGINE::CGameObject*>::iterator iter		= m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->begin();
	//list<ENGINE::CGameObject*>::iterator iter_end	= m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->end();

	//int iCount = 0;
	//for(; iter != iter_end; ++iter)
	//{
	//	if(dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos.x >= 4999.f)
	//	{
	//		++iCount;

	//		if(iCount >= 6)
	//		{
	//			m_pCameraMgr->GetBossAction() = true;
	//			break;
	//		}
	//	}	
	//}

	//화산 볼 때, 보스가 등장한다!

	if(m_pCameraMgr->GetBossReady() == true)
	{
		if(m_pBossAppear == false)	
		{
			m_pSoundMgr->PlayBGMSound(L"Stage1_Boss_BGM_2.ogg");
			
			if(	dynamic_cast<CBoss_1*>(m_pBoss)->GetApear() == false)
			{
				dynamic_cast<CBoss_1*>(m_pBoss)->GetApear() = true;

				dynamic_cast<CBoss_1*>(m_pBoss)->GetInfo()->m_vPos = D3DXVECTOR3(220.f, 10.f, 50.f);
			}

			list<ENGINE::CGameObject*>::iterator iter		= m_pLayerMgr->GetObjList(L"Dumy_Orc", ENGINE::LAYER_MONSTER)->begin();
			list<ENGINE::CGameObject*>::iterator iter_end	= m_pLayerMgr->GetObjList(L"Dumy_Orc", ENGINE::LAYER_MONSTER)->end();

			for(; iter != iter_end; ++iter)
			{
				(*iter)->GetAlive() = false;
			}

			iter		= m_pLayerMgr->GetObjList(L"Elf_Soldier", ENGINE::LAYER_NPC)->begin();
			iter_end	= m_pLayerMgr->GetObjList(L"Elf_Soldier", ENGINE::LAYER_NPC)->end();

			for(; iter != iter_end; ++iter)
			{
				dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos.x += 30.f;
			}

			ENGINE::CGameObject* pIsildur	= m_pLayerMgr->GetGameObject(L"Player_Isildur", ENGINE::LAYER_PLAYER);
			ENGINE::CGameObject* pElond		= m_pLayerMgr->GetGameObject(L"Player_Elond", ENGINE::LAYER_PLAYER);

			dynamic_cast<CClientObj*>(pIsildur)->GetInfo()->m_vPos = D3DXVECTOR3(100.f, 2.f, 40.f);
			dynamic_cast<CClientObj*>(pElond)->GetInfo()->m_vPos = D3DXVECTOR3(100.f, 2.f, 60.f);

			m_pBossAppear = true;
		}		
	}

	if(GetAsyncKeyState(VK_RETURN))
	{
		m_pLayerMgr->Release();
		m_pManagement->SceneChange(CSceneSelector(ENGINE::SC_STAGE_2));
		return;
	}

	if(dynamic_cast<CBoss_1*>(m_pBoss)->GetAttHp() == 100)
	{
		ENGINE::CGameObject* pGameObject = CSkillParticle::Create(m_pDevice);
		dynamic_cast<CSkillParticle*>(pGameObject)->SetColor(124.f/255.f, 252.f/255.f, 0.f/255.f, 0.3f);
		dynamic_cast<CSkillParticle*>(pGameObject)->SetSize(25.f, 25.f);
		dynamic_cast<CSkillParticle*>(pGameObject)->SetPos(D3DXVECTOR3(200.f, 20.f, 50.f));
		m_pLayerMgr->AddObject(L"HellGate", pGameObject, ENGINE::LAYER_GAMELOGIC);
		dynamic_cast<CSkillParticle*>(pGameObject)->SetAni();

		dynamic_cast<CBoss_1*>(m_pBoss)->GetAttHp() = 50;
	}

	if(dynamic_cast<CClientObj*>(m_pIsildur)->GetInfo()->m_vPos.x >= 200.f &&
		dynamic_cast<CClientObj*>(m_pElond)->GetInfo()->m_vPos.x >= 200.f)
	{
		m_pLayerMgr->Release();
		m_pManagement->SceneChange(CSceneSelector(ENGINE::SC_STAGE_2));
		return;
	}

	CScene::Update();
}

void CStage_1::Render(void)
{
	CScene::Render();
}

HRESULT CStage_1::AddBuffer(void)
{
	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_P_Sword_Hand_Cube");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_P_Sword_Guard_Cube");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_P_Sword_Blade_Cube");	

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_M_Sword_Hand_Cube");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_M_Sword_Guard_Cube");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_M_Sword_Blade_Cube");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_Boss_Sword_Hand_Cube");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_Boss_Sword_Guard_Cube");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_Boss_Sword_Blade_Cube");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_Normal_Cube");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_Orc_Cube");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_Boss_Cube");

	m_pResourcesMgr->AddObjBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX_CUSTOM, L"Buffer_Cube_Example", 250.f, 100.f, 150.f);

	m_pResourcesMgr->AddObjBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX_CUSTOM, L"Buffer_Cube_Metheo", 1.f, 1.f, 1.f);


	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer RcTex");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer RcTex2");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer RcLHP");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer RcHeart");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer RcHeart2");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer Ground");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer Ground2");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer RcSkill");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer RcSkill2");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer RcHealSkill");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer RcFireSkill");

	return S_OK;
}

HRESULT CStage_1::AddTexture(void)
{
	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture SkyBox", 
		L"../../Texture/SkyBox/SkyBox_Example.dds",  1);

	////////////////////////////////////////////////////////////////
	//엘론드

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture ElondFace", 
		L"../../Texture/Cube_dds/ElondFace.dds",  1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture ElondCloath", 
		L"../../Texture/Cube_dds/ElondCloath.dds",  1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture ElondArmLeg", 
		L"../../Texture/Cube_dds/ElondArmLeg.dds",  1);

	////////////////////////////////////////////////////////////////
	//이실두르

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture EsildurFace", 
		L"../../Texture/Cube_dds/EsildurFace.dds",  1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture EsildurCloath", 
		L"../../Texture/Cube_dds/EsildurCloath.dds",  1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture EsildurArmLeg", 
		L"../../Texture/Cube_dds/EsildurArmLeg.dds",  1);

	////////////////////////////////////////////////////////////////
	//오크
	
	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture OrcFace", 
		L"../../Texture/Cube_dds/OrcFace.dds",  1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture OrcArmLeg", 
		L"../../Texture/Cube_dds/OrcArmLeg.dds",  1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture OrcCloath", 
		L"../../Texture/Cube_dds/OrcCloath.dds",  1);


	////////////////////////////////////////////////////////////////
	//엘프 솔저

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture ElfFace", 
		L"../../Texture/Cube_dds/ElfFace.dds",  1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture ElfCloath", 
		L"../../Texture/Cube_dds/ElfCloath.dds",  1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_CUBE, L"Texture ElfArmLeg", 
		L"../../Texture/Cube_dds/ElfArmLeg.dds",  1);

	////////////////////////////////////////////////////////////////

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture PlayerFace", 
		L"../../Texture/UI/Esildoor.png",  1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture PlayerHeart", 
		L"../../Texture/UI/HP.png", 1);
	
	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture PlayerGround", 
		L"../../Texture/UI/ICON.png", 1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture PlayerGround2", 
		L"../../Texture/UI/ICON2.png", 1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture LHP", 
		L"../../Texture/UI/LHP.png", 1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture PlayerHeart2", 
		L"../../Texture/UI/HP.png", 1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture PlayerFace2", 
		L"../../Texture/UI/Elond.png", 1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture JumpSkill", 
		L"../../Texture/UI/JumpSkill.png", 1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture ShockWaveSkill", 
		L"../../Texture/UI/ShockWaveSkill.png", 1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture HealSkill", 
		L"../../Texture/UI/HealSkill.png", 1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture FireBoom", 
		L"../../Texture/UI/FireBoom.png", 1);

	return S_OK;
}

HRESULT CStage_1::AddObject(void)
{
	ENGINE::CGameObject* pGameObject = NULL;


	pGameObject = m_pIsildur = CPlayer_Elond::Create(m_pDevice);
	dynamic_cast<CPlayer_Elond*>(pGameObject)->SetPos(D3DXVECTOR3(60.f, 1.f, 40.f));
	m_pLayerMgr->AddObject(L"Player_Elond", pGameObject, ENGINE::LAYER_PLAYER);

	pGameObject = m_pElond = CPlayer_Isildur::Create(m_pDevice);
	dynamic_cast<CPlayer_Isildur*>(pGameObject)->SetPos(D3DXVECTOR3(60.f, 2.f, 50.f));
	m_pLayerMgr->AddObject(L"Player_Isildur", pGameObject, ENGINE::LAYER_PLAYER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_MOB);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(80.f, 1.f, 50.f));
	m_pLayerMgr->AddObject(L"Monser_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_MOB);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(80.f, 1.f, 45.f));
	m_pLayerMgr->AddObject(L"Monser_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_MOB);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(120.f, 1.f, 45.f));
	m_pLayerMgr->AddObject(L"Monser_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_MOB);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(100.f, 1.f, 50.f));
	m_pLayerMgr->AddObject(L"Monser_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_MOB);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(130.f, 1.f, 40.f));
	m_pLayerMgr->AddObject(L"Monser_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_MOB);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(125.f, 1.f, 42.f));
	m_pLayerMgr->AddObject(L"Monser_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CPetal::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"Petal", pGameObject, ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CPetal*>(pGameObject)->SetAni();

	pGameObject = CMetheo::Create(m_pDevice);
	dynamic_cast<CMetheo*>(pGameObject)->SetPos(0.f, 300.f, 90.f);
	m_pLayerMgr->AddObject(L"Metheo",pGameObject, ENGINE::LAYER_GAMELOGIC);

	pGameObject = CMetheo::Create(m_pDevice);
	dynamic_cast<CMetheo*>(pGameObject)->SetPos(0.f, 300.f, 75.f);
	m_pLayerMgr->AddObject(L"Metheo",pGameObject, ENGINE::LAYER_GAMELOGIC);

	pGameObject = CMetheo::Create(m_pDevice);
	dynamic_cast<CMetheo*>(pGameObject)->SetPos(0.f, 300.f, 60.f);
	m_pLayerMgr->AddObject(L"Metheo",pGameObject, ENGINE::LAYER_GAMELOGIC);

	pGameObject = CMetheo::Create(m_pDevice);
	dynamic_cast<CMetheo*>(pGameObject)->SetPos(0.f, 300.f, 45.f);
	m_pLayerMgr->AddObject(L"Metheo",pGameObject, ENGINE::LAYER_GAMELOGIC);

	pGameObject = CMetheo::Create(m_pDevice);
	dynamic_cast<CMetheo*>(pGameObject)->SetPos(0.f, 300.f, 30.f);
	m_pLayerMgr->AddObject(L"Metheo",pGameObject, ENGINE::LAYER_GAMELOGIC);

	pGameObject = CMetheo::Create(m_pDevice);
	dynamic_cast<CMetheo*>(pGameObject)->SetPos(0.f, 300.f, 15.f);
	m_pLayerMgr->AddObject(L"Metheo",pGameObject, ENGINE::LAYER_GAMELOGIC);

	pGameObject = CMetheo::Create(m_pDevice);
	dynamic_cast<CMetheo*>(pGameObject)->SetPos(0.f, 300.f, 0.f);
	m_pLayerMgr->AddObject(L"Metheo",pGameObject, ENGINE::LAYER_GAMELOGIC);


	for(int j=0; j<2; ++j)
	{
		for(int i=0; i<10; ++i)
		{
			if(j == 0)
			{
				pGameObject = CElf_Soldier::Create(m_pDevice, CElf_Soldier::NPC_HEALER);
				dynamic_cast<CElf_Soldier*>(pGameObject)->SetPos(D3DXVECTOR3(40.f + (j * 4), 2.f, 20.f + float(i*7)));
				m_pLayerMgr->AddObject(L"Elf_Soldier", pGameObject, ENGINE::LAYER_NPC);
			}
			else
			{
				pGameObject = CElf_Soldier::Create(m_pDevice, CElf_Soldier::NPC_SOLDIER);
				dynamic_cast<CElf_Soldier*>(pGameObject)->SetPos(D3DXVECTOR3(40.f + (j * 4), 2.f, 20.f + float(i*7)));
				m_pLayerMgr->AddObject(L"Elf_Soldier", pGameObject, ENGINE::LAYER_NPC);				
			}
		}
	}

	//상단 더미 오크=========================================================================
	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(170, 2.f, 80.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(155.f, 2.f, 60.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(125.f, 2.f, 65.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(95.f, 2.f, 66.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(70.f, 2.f, 72.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(120.f, 2.f, 78.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(132.f, 2.f, 73.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(111.f, 2.f, 71.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(105.f, 2.f, 62.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(94.f, 2.f, 64.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(73.f, 2.f, 77.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(104.f, 2.f, 71.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(112.f, 2.f, 69.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(58.f, 2.f, 65.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(56.f, 2.f, 63.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(57.f, 2.f, 53.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(172.f, 2.f, 72.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(163.f, 2.f, 80.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(123.f, 2.f, 78.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(164.f, 2.f, 66.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	//하단 더미 오크=====================================================================

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(86.f, 2.f, 39.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(133.f, 2.f, 21.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(154.f, 2.f, 31.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(94.f, 2.f, 28.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(98.f, 2.f, 37.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(107.f, 2.f, 36.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(99.f, 2.f, 27.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(110.f, 2.f, 25.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(120.f, 2.f, 24.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(166.f, 2.f, 36.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(65.f, 2.f, 40.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(180.f, 2.f, 33.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(85.f, 2.f, 22.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(107.f, 2.f, 26.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(100.f, 2.f, 23.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(165.f, 2.f, 39.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(80.f, 2.f, 31.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(95.f, 2.f, 32.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(137.f, 2.f, 36.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(159.f, 2.f, 20.f));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_MONSTER);
	//BOSS=========================================================================
	pGameObject = m_pBoss = CBoss_1::Create(m_pDevice);
	//dynamic_cast<CBoss_1*>(m_pBoss)->GetInfo()->m_vPos = D3DXVECTOR3(220.f, 20.f, 50.f);
	//dynamic_cast<CBoss_1*>(m_pBoss)->GetApear() = true;
	m_pLayerMgr->AddObject(L"Stage1_Boss", pGameObject, ENGINE::LAYER_MONSTER);

	/*for(int i=0; i<16; ++i)
	{
		pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
		dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(120.f, 2.f, 20.f + float(i*4)));
		m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_GAMELOGIC);
	}*/

	/*int frand = rand() % 21 + 60;
	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_DUMMY);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(120.f, 2.f, float(frand)));
	m_pLayerMgr->AddObject(L"Dumy_Orc", pGameObject, ENGINE::LAYER_GAMELOGIC);*/
	
	pGameObject = CSkyBox::Create(m_pDevice);
	//dynamic_cast<CSkyBox*>(pGameObject)->SetPos(150.f, 45.f, 50.f);
	dynamic_cast<CSkyBox*>(pGameObject)->SetPos(D3DXVECTOR3(150.f, 45.f, 50.f));
	m_pLayerMgr->AddObject(L"SkyBox_Example", pGameObject, ENGINE::LAYER_ENVIRONMENT);

	pGameObject = CUI::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"Player Face", pGameObject, ENGINE::LAYER_UI);

	pGameObject = CUI2::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"Player Face2", pGameObject, ENGINE::LAYER_UI);

	pGameObject = CPlayerGround::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"Player Ground", pGameObject, ENGINE::LAYER_UI);

	pGameObject = CPlayerGround2::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"Player Ground2", pGameObject, ENGINE::LAYER_UI);

	return S_OK;
}

void CStage_1::SetCameraToPlayer()
{
	m_pCameraMgr->SetPlayer1(m_pLayerMgr->GetComponent(L"Player_Isildur", L"Transform", ENGINE::LAYER_PLAYER));
	m_pCameraMgr->SetPlayer2(m_pLayerMgr->GetComponent(L"Player_Elond", L"Transform", ENGINE::LAYER_PLAYER));
}

void CStage_1::CreateTerrain(void) // 터레인 생성
{
	ENGINE::CGameObject* pGameObject = NULL;

	ENGINE::TERRAIN_INFO* pLoadInfo = LoadTerrain();

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::CVIBuffer::BUFFER_TERRAIN, L"Buffer_Terrain",  pLoadInfo->m_VtxCntX, pLoadInfo->m_VtxCntZ,  pLoadInfo->m_Itv);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture Terrain", 
		L"../../Texture/terrain/grass_mix_s.jpg",  1);

	pGameObject = CTerrain::Create(m_pDevice, 
		pLoadInfo->m_VtxCntX, 
		pLoadInfo->m_VtxCntZ, 
		pLoadInfo->m_Itv,
		L"Buffer_Terrain");

	for(DWORD i=0; i<*dynamic_cast<CTerrain*>(pGameObject)->GetVtxCnt(); ++i)
	{
		((CTerrain*)pGameObject)->GetVtxTex()[i]			= pLoadInfo->m_pVertex[i];
		((CTerrain*)pGameObject)->GetVtxConvertTex()[i]	= pLoadInfo->m_pConvertVertex[i];
	}

	*((CTerrain*)pGameObject)->GetVtxCntX() = pLoadInfo->m_VtxCntX;
	*((CTerrain*)pGameObject)->GetVtxCntZ() = pLoadInfo->m_VtxCntZ;
	*((CTerrain*)pGameObject)->GetVtxItv() = pLoadInfo->m_Itv;

	m_pLayerMgr->AddObject(L"Terrain", pGameObject, ENGINE::LAYER_ENVIRONMENT);
}

void CStage_1::Release(void)
{
	ENGINE::Safe_Single_Destory(m_pResourcesMgr);
	ENGINE::Safe_Single_Destory(m_pLayerMgr);
}

CStage_1* CStage_1::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage_1*	pStage_1 = new CStage_1(pDevice);

	if(FAILED(pStage_1->Initialize()))
		ENGINE::Safe_Delete(pStage_1);

	return pStage_1;
}

ENGINE::TERRAIN_INFO* CStage_1::LoadTerrain(void)
{
	DWORD	dwByte = 0;

	HANDLE hFile = CreateFile(L"../../Data/Stage1.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ENGINE::TERRAIN_INFO* pSaveTerrain = new ENGINE::TERRAIN_INFO;

	ReadFile(hFile, pSaveTerrain, sizeof(ENGINE::TERRAIN_INFO), &dwByte, NULL);

	pSaveTerrain->m_pVertex			= new ENGINE::VTXTEX[pSaveTerrain->m_dwVtxCnt];
	pSaveTerrain->m_pConvertVertex	= new ENGINE::VTXTEX[pSaveTerrain->m_dwVtxCnt];

	for(DWORD i=0; i<pSaveTerrain->m_dwVtxCnt; ++i)
	{
		ReadFile(hFile, &pSaveTerrain->m_pVertex[i], sizeof(ENGINE::VTXTEX), &dwByte, NULL);
		ReadFile(hFile, &pSaveTerrain->m_pConvertVertex[i], sizeof(ENGINE::VTXTEX), &dwByte, NULL);
	}

	CloseHandle(hFile);

	return pSaveTerrain;
}

