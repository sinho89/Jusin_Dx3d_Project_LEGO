#include "stdafx.h"
#include "SkyBox.h"
#include "Export_Function.h"
#include "Pipeline.h"
#include "Transform.h"
#include "LayerMgr.h"
#include "CameraMgr.h"

#include "Stage_1.h"


CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pDevice)
: CCubeObject(pDevice)
, JumpAble(false)
, m_bCollision(true)
, m_Gravity(0.f)
{
	
}

CSkyBox::~CSkyBox()
{
	Release();
}
HRESULT CSkyBox::Initialize(void)
{	
	CCubeObject::Initialize();
	m_fSpeed			= 10;

	m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Cube_Example", m_pVertex);

	FAILED_CHECK(AddComponent());
	return S_OK;
}

void CSkyBox::KeyInput(void)
{
	float m_fTime = m_pTimeMgr->GetTime();

	if(GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		JumpAble = true;
		m_bCollision = false;
	}
}	

HRESULT CSkyBox::AddComponent(void)
{
	ENGINE::CComponent*		pComponent = NULL;

	// Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// Buffer
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Cube_Example");
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

void CSkyBox::SetTransform(void)
{

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
	}
	
}

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSkyBox*		pSkyBox = new CSkyBox(pDevice);

	if(FAILED(pSkyBox->Initialize()))
		ENGINE::Safe_Delete(pSkyBox);

	return pSkyBox;
}

void CSkyBox::Update(void)
{
	CCubeObject::SetDirection();
		
	/*float m_fTime = m_pTimeMgr->GetTime();
	
	if(!m_bCollision)
	{
		m_Gravity += m_fTime;

		m_pInfo->m_vPos.y -= 0.098 * m_Gravity * m_Gravity;
	}*/

	CollisionTerrain();
	D3DXMATRIX	matScale, matRot, matTrans;

	D3DXMatrixRotationY(&matRot, m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);
	D3DXMatrixTranslation(&matTrans, m_pInfo->m_vPos.x, m_pInfo->m_vPos.y, m_pInfo->m_vPos.z);

	m_pInfo->m_matWorld = matRot * matTrans;

	ENGINE::CGameObject::Update();	

	SetTransform();

	//KeyInput();
	//Jump();
}

void CSkyBox::Render(void)
{
	m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Cube_Example", m_pConvertVertex);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pTexture->Render(0);
	m_pBuffer->Render();
}

void CSkyBox::Release(void)
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

void CSkyBox::Jump(void)
{
	if(JumpAble)
	{
		m_bCollision = false;
		m_pInfo->m_vPos.y += 0.098 * m_Gravity * 1;
	}

	if(m_pInfo->m_vPos.y < 1)
	{
		m_bCollision = true;
		m_pInfo->m_vPos.y = 1;
		m_Gravity = 0;
		JumpAble = false;
	}
}

void CSkyBox::CollisionTerrain(void)
{
	//Y값이 1이면 Gravity는 0
	//collision이 true;
	//false이면 중력받게하고 JumpAble이 true;

	if(m_bCollision == true)
	{
		
	}
	
	if(m_bCollision == false)
	{
		//JumpAble = true;
	}
}