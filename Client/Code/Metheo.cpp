#include "stdafx.h"
#include "Metheo.h"
#include "Export_Function.h"
#include "Player_Elond.h"
#include "Monster_Orc.h"
#include "SoundMgr.h"

CMetheo::CMetheo(LPDIRECT3DDEVICE9 pDevice)
: CCubeObject(pDevice)
, m_pElond(NULL), m_pMetheoParticle(NULL)
, m_bMakeStone(false), m_bDamageCheck(false), m_bShakeCam(false)
, m_Gravity(0.f), m_fSkyFall(0.f)
{
}

CMetheo::~CMetheo(void)
{

}
HRESULT CMetheo::Initialize()
{
	CCubeObject::Initialize();
	m_fSpeed			= 10;
	m_fAngle			= 0.f;

	
	m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Cube_Metheo", m_pVertex);

	FAILED_CHECK(AddComponent());

	m_bMakeStone = false;

	m_pMetheoParticle = CSkillParticle::Create(m_pDevice);
	dynamic_cast<CSkillParticle*>(m_pMetheoParticle)->SetColor(220.f/255.f, 0.f/255.f, 0.f/255.f, 0.5f);
	dynamic_cast<CSkillParticle*>(m_pMetheoParticle)->SetSize(4.f, 4.f);
	dynamic_cast<CSkillParticle*>(m_pMetheoParticle)->SetPos(m_pInfo->m_vPos);
	m_pLayerMgr->AddObject(L"Metheo_Particle", m_pMetheoParticle, ENGINE::LAYER_GAMELOGIC);
	dynamic_cast<CSkillParticle*>(m_pMetheoParticle)->SetAni();

	return S_OK;
}

HRESULT CMetheo::AddComponent(void)
{
	ENGINE::CComponent*		pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// Buffer
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Cube_Metheo");
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	return S_OK;
}

void CMetheo::SetTransform(void)
{
	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
	}
}

void CMetheo::SetPos(float fX, float fY, float fZ)
{
	m_pInfo->m_vPos.x = fX;
	m_pInfo->m_vPos.y = fY;
	m_pInfo->m_vPos.z = fZ;
}

CMetheo* CMetheo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CMetheo*		pMetheo = new CMetheo(pDevice);

	if(FAILED(pMetheo->Initialize()))
		ENGINE::Safe_Delete(pMetheo);

	return pMetheo;
}

void CMetheo::Update(void)
{
	if(m_bMakeStone)
	{
		//CCubeObject::SetDirection();
		m_pInfo->m_vDir = m_DestPos - m_pInfo->m_vPos;
		D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);

		m_pInfo->m_vPos += m_pInfo->m_vDir;

		//m_pLayerMgr->GetComponent(L"Player_Elond", L"Transform", ENGINE::LAYER_PLAYER);
		//m_pInfo->m_vPos.y -= 0.5f;

		m_pInfo->m_fAngle[ENGINE::ANGLE_X] += 0.1f;
		m_pInfo->m_fAngle[ENGINE::ANGLE_Z] += 0.1f;
		//D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);
		D3DXMATRIX	matScale, matRotX, matRotY, matRotZ, matTrans;

		D3DXMatrixTranslation(&matTrans, m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

		D3DXMatrixRotationX(&matRotX, m_pInfo->m_fAngle[ENGINE::ANGLE_X]);
		D3DXMatrixRotationY(&matRotY, m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);
		D3DXMatrixRotationZ(&matRotZ, m_pInfo->m_fAngle[ENGINE::ANGLE_Z]);

		m_pInfo->m_matWorld =  matRotX * matRotY * matRotZ * matTrans;

		SetTransform();

		/*D3DXMATRIX matX, matY, matZ;

		D3DXMatrixRotationX(&matX, 15.f);
		D3DXMatrixRotationY(&matY, 15.f);
		D3DXMatrixRotationZ(&matZ, 15.f);


		m_pInfo->m_matWorld = matX + matY + matZ;*/


		//cout << "Y :" << m_pInfo->m_vPos.y << endl;

		dynamic_cast<CSkillParticle*>(m_pMetheoParticle)->SetPos(m_pInfo->m_vPos);

		ENGINE::CGameObject::Update();	

		if(m_pInfo->m_vPos.y <= 0.f)
		{
			if(m_bShakeCam == false)
			{
				*(m_pCameraMgr->GetBossShake()) = true;
				m_bShakeCam = true;
			}

			Metheo_Damage();
		}

		if(m_pInfo->m_vPos.y <= -5.f)
		{

			m_pCameraMgr = false;
			m_bMakeStone = false;
			m_bDamageCheck = false;
			SetPos(0.f, 300.f, 50.f);
			m_fSkyFall = 0.f;
		}
	}
}

void CMetheo::Render(void)
{
	if(m_bMakeStone)
	{
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Cube_Metheo", m_pConvertVertex);

		m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		//m_pTexture->Render(0);
		m_pBuffer->Render();
	}
}	

void CMetheo::Release(void)
{
	if(m_pVertex != NULL)
	{
		ENGINE::Safe_Delete_Array(m_pVertex);
	}

	if(m_pConvertVertex != NULL)
	{
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
	}
}

void CMetheo::Metheo_Damage(void)
{
	if(m_bDamageCheck == false)
	{
		m_pSoundMgr->PlayerMetheoSound(L"Metheo.wav");

		m_bDamageCheck = true;

		D3DXVECTOR3 vecLength;
		float fLength = 0.f;

		list<ENGINE::CGameObject*>::iterator iter		= m_pLayerMgr->GetObjList(L"Dumy_Orc", ENGINE::LAYER_MONSTER)->begin();
		list<ENGINE::CGameObject*>::iterator iter_end	= m_pLayerMgr->GetObjList(L"Dumy_Orc", ENGINE::LAYER_MONSTER)->end();

		for(; iter != iter_end; ++iter)
		{
			vecLength = dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos -  m_pInfo->m_vPos;

			fLength = D3DXVec3Length(&vecLength);

			if(fLength <= 20.f)
				dynamic_cast<CMonster_Orc*>(*iter)->GetAttHp() = 0;
		}

		iter		= m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->begin();
		iter_end	= m_pLayerMgr->GetObjList(L"Monser_Orc", ENGINE::LAYER_MONSTER)->end();

		for(; iter != iter_end; ++iter)
		{
			vecLength = dynamic_cast<CClientObj*>(*iter)->GetInfo()->m_vPos -  m_pInfo->m_vPos;

			fLength = D3DXVec3Length(&vecLength);

			if(fLength <= 20.f)
				dynamic_cast<CMonster_Orc*>(*iter)->GetAttHp() = 0;
		}
	}
}



