#include "stdafx.h"
#include "Stage_2.h"
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

#include "FieldFire.h"
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
#include "MagmaParticle.h"
#include "Boss_2.h"
#include "SoundMgr.h"
#include "Ring.h"
#include "Aim.h"
#include "Destroy.h"


const float kMoveAmt = 5.0f * (1.0f / 60.0f);

CStage_2::CStage_2(LPDIRECT3DDEVICE9 pDevice)
: CClientScene(pDevice)
, m_bBoss_2_Appear(false)
, m_bBoss2(NULL), m_fDelayTime(0.f), m_bRingThrow(false), m_bRing2(false)
, m_bReflectOn(false), m_fGravityTime(0.f), m_fGravity(0.f), m_BossAttSound(false), Stage2Att(false), Stage2Crowl(false)
, m_bGengi(false), m_bDestroy(false)
{

}

CStage_2::~CStage_2(void)
{
	Release();
}

HRESULT CStage_2::Initialize(void)
{
	CreateTerrain();

	AddBuffer();
	AddTexture();
	AddObject();
	SetCameraToPlayer();

	m_fTime = 0.f;
	m_pSoundMgr->PlayBGMSound(L"Stage2_BGM.ogg");
	m_pLayerMgr->SetDivisionType(ENGINE::CLayerMgr::DIV_SINGLE);
	m_pLayerMgr->GetObjList(L"Player Heart", ENGINE::LAYER_UI)->reverse();

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.25f, // 45 - degree
		ENGINE::WINCX / ENGINE::WINCY,
		1.0f,
		1000.0f);


	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj);

	m_pCameraMgr->Stage2InitCam();	

	return S_OK;
}

void CStage_2::Update(void)
{

	/*m_fTime += 1.f * m_pTimeMgr->GetTime();

	GetCursorPos(&m_pMouse);
	ScreenToClient(ENGINE::g_hWnd, &m_pMouse);*/


	SetCameraToPlayer();
	
	m_pCameraMgr->Stage2BossAction(m_pTimeMgr->GetTime());

	/*if(m_pCameraMgr->GetStage2BossStart())
	{
		if(m_bBoss_2_Appear == false)
		{
			dynamic_cast<CClientObj*>(m_bBoss2)->GetInfo()->m_vPos.y += 0.5f;

			if(dynamic_cast<CClientObj*>(m_bBoss2)->GetInfo()->m_vPos.y >= 2.f)
				m_bBoss_2_Appear = true;
		}
		else
		{
			dynamic_cast<CClientObj*>(m_bBoss2)->GetInfo()->m_vPos.y = 2.f;
		}
	}*/
	
	PlayerDelete();

	if(m_pCameraMgr->GetOzoom() == true)
	{
		ENGINE::CComponent* pPlayerInfo = m_pLayerMgr->GetComponent(L"Player_Isildur", L"Transform", ENGINE::LAYER_PLAYER);

		m_pSoundMgr->PlayBGMSound(L"Overwatch Bgm_PlayOfTheGame.mp3");

		ENGINE::CGameObject* pGameObject = CFieldFire::Create(m_pDevice);
		dynamic_cast<CFieldFire*>(pGameObject)->SetPos(((ENGINE::CTransform*)pPlayerInfo)->m_vPos);
		m_pLayerMgr->AddObject(L"Ozoom", pGameObject, ENGINE::LAYER_GAMELOGIC);
		dynamic_cast<CFieldFire*>(pGameObject)->SetAni();
		
		m_pSoundMgr->Crowing(L"Ozoom.wav");

		m_pCameraMgr->SetOzoom(false);
		
	}

	if(m_pCameraMgr->GetDisType() == m_pCameraMgr->DIS_STAGE2_BOSS)
	{
		ShowCursor(false);

		if(!m_bGengi)
		{
			m_pSoundMgr->GengiSound(L"GenjiSound.wav");
			m_bGengi = true;
		}

		const float kMaxAngle = 89.5f; // Max degree of rotation
		static float pitchAmt = 0.0f; // Amout we've looked up or down

		// Get the middle of the screen
		int midScrX = GetSystemMetrics(SM_CXSCREEN) >> 1;
		int midScrY = GetSystemMetrics(SM_CYSCREEN) >> 1;

		float amt;
		POINT pt = {0};

		GetCursorPos(&pt); // Get the current mouse position

		// Rotate left/right
		amt = float(pt.x - midScrX) * kMoveAmt;
		m_pCameraMgr->rotateY(D3DXToRadian(amt));

		// Calculate amount to rotate up/down
		amt = float(midScrY - pt.y) * kMoveAmt;

		// Clamp pitch amount
		if(pitchAmt + amt <= -kMaxAngle)
		{
			amt = -kMaxAngle - pitchAmt;
			pitchAmt = -kMaxAngle;
		}
		else if(pitchAmt + amt >= kMaxAngle)
		{
			amt = kMaxAngle - pitchAmt;
			pitchAmt = kMaxAngle;
		}
		else
		{
			pitchAmt += amt;
		}

		// Pitch camera
		m_pCameraMgr->Pitch(D3DXToRadian(amt)); 	

		SetCursorPos(midScrX, midScrY);

		m_pCameraMgr->SetStage2BossView();

		if(m_bRingThrow == false)
		{
			m_fDelayTime += 1.f * m_pTimeMgr->GetTime();

			if(m_fDelayTime > 2.f)
			{
				m_bRingThrow = true;
				m_bRing2 = true;
			}
		}
	}
	else
	{
		m_pCameraMgr->Stage2BasicView();
	}

	if(m_bRing2)
	{
		if(!m_BossAttSound)
		{
			m_pSoundMgr->HitSound(L"Stage2BossAtt.wav");	//날아오는것
			m_BossAttSound = true;
		}
		D3DXVECTOR3 m_vecDir = dynamic_cast<CClientObj*>(m_pUnit)->GetInfo()->m_vPos - dynamic_cast<CClientObj*>(m_pRing)->GetInfo()->m_vPos;

		dynamic_cast<CClientObj*>(m_pRing)->GetInfo()->m_vPos += ((m_vecDir) * m_pTimeMgr->GetTime());
	}

	if(dynamic_cast<CPlayer_Elond*>(m_pUnit)->GetReflectSkill())
	{
		m_bReflectOn = true;		
		m_bRing2 = false;
	}

	if(m_bReflectOn)
	{
		if(!Stage2Att)
		{	
			//반사해서 날아가는곳
			m_pSoundMgr->HitSound(L"Stage2Att.wav");

			Stage2Att = true;
		}
		
		D3DXVECTOR3 m_vecDir = dynamic_cast<CClientObj*>(m_bBoss2)->GetInfo()->m_vPos - dynamic_cast<CClientObj*>(m_pRing)->GetInfo()->m_vPos;

		dynamic_cast<CClientObj*>(m_pRing)->GetInfo()->m_vPos += ((m_vecDir) * m_pTimeMgr->GetTime());
	}

	if(m_bReflectOn && m_bRing2 == false && (dynamic_cast<CClientObj*>(m_bBoss2)->GetInfo()->m_vPos.x - 1.f <= dynamic_cast<CClientObj*>(m_pRing)->GetInfo()->m_vPos.x))
	{
		if(!Stage2Crowl)
		{	
			//부딪혔을때 사우론과 함께 떨어지는 곳.
			m_pSoundMgr->HitSound(L"Stage2Crowl.wav");
			Stage2Crowl = true;
		}
		
		m_fTime += 1.f * m_pTimeMgr->GetTime();
		if(m_fTime >= 2.f)
		{
			if(!m_bDestroy)
			{
				m_pLayerMgr->AddObject(L"Destroy", CDestroy::Create(m_pDevice, ENGINE::WINCX / 2, ENGINE::WINCY / 2) , ENGINE::LAYER_UI);

				m_pSoundMgr->PlaySound(L"Overwatch Sfx_MarkKill.wav");
				m_bDestroy = true;
			}
			
		}
		float fTime = 1.f * m_pTimeMgr->GetTime();
		
		m_fGravity += fTime;

		dynamic_cast<CClientObj*>(m_bBoss2)->GetInfo()->m_vPos.y	-= 0.098 * m_fGravity * m_fGravity * 2.f;
		dynamic_cast<CClientObj*>(m_pRing)->GetInfo()->m_vPos.y		-= 0.098 * m_fGravity * m_fGravity * 2.f;
	}

	CScene::Update();
}

void CStage_2::Render(void)
{
	CScene::Render();
}

void CStage_2::Release(void)
{
	ENGINE::Safe_Single_Destory(m_pResourcesMgr);
	ENGINE::Safe_Single_Destory(m_pLayerMgr);
}

CStage_2* CStage_2::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage_2*	pStage_2 = new CStage_2(pDevice);

	if(FAILED(pStage_2->Initialize()))
		ENGINE::Safe_Delete(pStage_2);

	return pStage_2;
}

HRESULT CStage_2::AddBuffer(void)
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
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_Normal_Cube");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX, L"Buffer_Orc_Cube");

	m_pResourcesMgr->AddObjBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_CUBETEX_CUSTOM, L"Buffer_Cube_Example", 1.f, 1.f, 1.f);

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

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer SauronEye");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer Ring");

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC,
		ENGINE::CVIBuffer::BUFFER_RCTEX, L"Buffer RcDestoy");


	return S_OK;
}

HRESULT CStage_2::AddTexture(void)
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

	//Stage2_Boss IMG
	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture SauronEye", 
		L"../../Texture/Sauron_Eye.png", 1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture Ring", 
		L"../../Texture/UI/Lord_of_The_Ring.png", 1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture Aim", 
		L"../../Texture/Aim.png", 1);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture Destroy", 
		L"../../Texture/UI/DestroySauron.png", 1);

	return S_OK;
}

HRESULT CStage_2::AddObject(void)
{
	ENGINE::CGameObject* pGameObject = NULL;

	pGameObject = m_pUnit = CPlayer_Elond::Create(m_pDevice);
	dynamic_cast<CPlayer_Elond*>(pGameObject)->SetPos(D3DXVECTOR3(6.f, 2.f, 25.f));
	m_pLayerMgr->AddObject(L"Player_Elond", pGameObject, ENGINE::LAYER_PLAYER);

	pGameObject = CPlayer_Isildur::Create(m_pDevice);
	dynamic_cast<CPlayer_Isildur*>(pGameObject)->SetPos(D3DXVECTOR3(8.f, 2.f, 30.f));
	m_pLayerMgr->AddObject(L"Player_Isildur", pGameObject, ENGINE::LAYER_PLAYER);

	//몬스터=======================================================================
	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_MOB);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(20.f, 2.f, 15.f));
	m_pLayerMgr->AddObject(L"Monser_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_MOB);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(30.f, 2.f, 20.f));
	m_pLayerMgr->AddObject(L"Monser_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_MOB);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(50.f, 2.f, 23.f));
	m_pLayerMgr->AddObject(L"Monser_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_MOB);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(60.f, 2.f, 28.f));
	m_pLayerMgr->AddObject(L"Monser_Orc", pGameObject, ENGINE::LAYER_MONSTER);

	pGameObject = CMonster_Orc::Create(m_pDevice, CMonster_Orc::ORC_MOB);
	dynamic_cast<CMonster_Orc*>(pGameObject)->SetPos(D3DXVECTOR3(70.f, 2.f, 35.f));
	m_pLayerMgr->AddObject(L"Monser_Orc", pGameObject, ENGINE::LAYER_MONSTER);
	//=============================================================================

	//보스=========================================================================
	pGameObject = m_bBoss2 = CBoss_2::Create(m_pDevice);
	dynamic_cast<CBoss_2*>(pGameObject)->SetPos(D3DXVECTOR3(250.f, 2.f, 35.f));//D3DXVECTOR3(250.f, 2.f, 35.f)
	m_pLayerMgr->AddObject(L"Boss_2", pGameObject, ENGINE::LAYER_MONSTER);
	//=============================================================================

	pGameObject = CPetal::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"Petal", pGameObject, ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CPetal*>(pGameObject)->SetAni();

	pGameObject = CSkillParticle::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"SkillParticle", pGameObject, ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CSkillParticle*>(pGameObject)->SetAni();

	pGameObject = CMagmaParticle::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"MagmaParticle", pGameObject, ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CMagmaParticle*>(pGameObject)->SetAni();

	pGameObject = CUI::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"Player Face", pGameObject, ENGINE::LAYER_UI);

	pGameObject = CUI2::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"Player Face2", pGameObject, ENGINE::LAYER_UI);

	pGameObject = CPlayerGround::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"Player Ground", pGameObject, ENGINE::LAYER_UI);

	pGameObject = CPlayerGround2::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"Player Ground2", pGameObject, ENGINE::LAYER_UI);

	pGameObject = m_pRing = CRing::Create(m_pDevice);
	dynamic_cast<CRing*>(pGameObject)->SetPos(D3DXVECTOR3(255.f, 2.f, 35.f));
	m_pLayerMgr->AddObject(L"Lord_of_The_Ring", pGameObject, ENGINE::LAYER_GAMELOGIC);

	pGameObject = CAim::Create(m_pDevice);
	m_pLayerMgr->AddObject(L"Aim", pGameObject, ENGINE::LAYER_UI);

	


	return S_OK;
}
void CStage_2::SetCameraToPlayer()
{
	m_pCameraMgr->SetPlayer1(m_pLayerMgr->GetComponent(L"Player_Isildur", L"Transform", ENGINE::LAYER_PLAYER));
	m_pCameraMgr->SetPlayer2(m_pLayerMgr->GetComponent(L"Player_Elond", L"Transform", ENGINE::LAYER_PLAYER));
}
ENGINE::TERRAIN_INFO* CStage_2::LoadTerrain( void )
{
	DWORD	dwByte = 0;

	HANDLE hFile = CreateFile(L"../../Data/Stage2.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

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


void CStage_2::CreateTerrain(void) // 터레인 생성
{
	ENGINE::CGameObject* pGameObject = NULL;

	ENGINE::TERRAIN_INFO* pLoadInfo = LoadTerrain();

	m_pResourcesMgr->AddBuffer(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::CVIBuffer::BUFFER_TERRAIN, L"Buffer_Terrain",  pLoadInfo->m_VtxCntX, pLoadInfo->m_VtxCntZ,  pLoadInfo->m_Itv);

	m_pResourcesMgr->AddTexture(m_pDevice, ENGINE::RESOURCE_DYNAMIC, 
		ENGINE::TEX_NORMAL, L"Texture Terrain", 
		L"../../Texture/terrain/lava_d.jpg",  1);

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

void CStage_2::PlayerDelete()
{
	if(m_pCameraMgr->GetPlayer1Delete())
	{
		ENGINE::CGameObject* pGameObject = m_pLayerMgr->GetGameObject(L"Player_Isildur", ENGINE::LAYER_PLAYER);
		((CPlayer_Isildur*)pGameObject)->DeletePlayer(); 

		ENGINE::CComponent* pPlayerInfo = m_pLayerMgr->GetComponent(L"Player_Isildur", L"Transform", ENGINE::LAYER_PLAYER);

		if(((ENGINE::CTransform*)pPlayerInfo)->m_vPos.x <= 40)
			m_pCameraMgr->SetPlayer1Delete(false);
	}
}
