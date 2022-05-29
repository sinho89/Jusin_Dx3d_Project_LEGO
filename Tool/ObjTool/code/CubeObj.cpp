#include "stdafx.h"
#include "CubeObj.h"

#include "Export_Function.h"
#include "TimeMgr.h"
#include "Management.h"

#include "VIBuffer.h"
#include "Texture.h"
#include "Transform.h"
#include "Pipeline.h"
#include "CameraObserver.h"
#include "CollisionMgr.h"
#include "MouseCol.h"
#include "CubeTex.h"

CCubeObj::CCubeObj(LPDIRECT3DDEVICE9 pDevice, const wstring& wstrResourceKey)
: CGameObject(pDevice)
, m_pResourcesMgr(ENGINE::Get_ResourceMgr())
, m_pTimeMgr(ENGINE::Get_Time())
, m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pManagement(ENGINE::Get_Management())
, m_pCollisionMgr(CCollisionMgr::GetInstance())
, m_fTime(0.f)
, m_dwVtxCnt(0)
, m_pMouseCol(NULL)
, m_pInfo(NULL)
, m_pConvertVertex(NULL)
, m_pVertex(NULL)
, m_pConvertVertex_Col(NULL)
, m_pVertex_Col(NULL)
, m_pCamObserver(NULL)
, m_pObjToolView(NULL)
, m_wstrResourceKey(wstrResourceKey)
, m_ePickID(ENGINE::PICK_NONE)
, m_fDist(0)
, m_bSetTex(false)
, m_bMove(false)
, m_eCubeSaveType(ENGINE::SOBJ_CUBE)
, m_fItv_X(0.f)
, m_fItv_Y(0.f)
, m_fItv_Z(0.f)
{
}

CCubeObj::~CCubeObj(void)
{
	Release();
}

HRESULT CCubeObj::Initialize(void)
{
	m_pCamObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCamObserver);

	m_fDistance = 10.f;
	m_fCameraAngle = D3DXToRadian(45.f);
	m_fSpeed = 0.2f;

	m_dwVtxCnt			=	8;

	if(m_wstrResourceKey == L"Buffer_CubeTex")
	{
		m_pVertex			=	new ENGINE::VTXCUBE[m_dwVtxCnt];
		m_pConvertVertex	=	new ENGINE::VTXCUBE[m_dwVtxCnt];

		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, m_wstrResourceKey, m_pVertex);

		for(int i=0; i<8; ++i)
		{
			m_pOriginPos[i] = m_pVertex[i].vPos;
		}
	}
	else
	{
		m_pVertex_Col			=	new ENGINE::VTXCUBECOL[m_dwVtxCnt];
		m_pConvertVertex_Col	=	new ENGINE::VTXCUBECOL[m_dwVtxCnt];

		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, m_wstrResourceKey, m_pVertex_Col);
	}	
	
	FAILED_CHECK(AddComponent());

	return S_OK;
}

void CCubeObj::SetDirection(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &ENGINE::g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);
}
void CCubeObj::SetInfo(ENGINE::CComponent* _pCube)
{
	//ENGINE::CComponent*	pComponent = NULL;

	//memcpy(m_pInfo, _pCube, sizeof(ENGINE::CTransform));

	//m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", m_pInfo));

	//m_pInfo = _pCube;
}

HRESULT CCubeObj::AddComponent(void)
{
	ENGINE::CComponent*	pComponent = NULL;

	//Transform
	pComponent = m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, m_wstrResourceKey);
	m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));
	
	//Collision Mouse
	pComponent = m_pCollisionMgr->CloneColObject(CCollisionMgr::COL_MOUSE);
	m_pMouseCol = dynamic_cast<CMouseCol*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"MouseCol", pComponent));

	//Texture
	/*pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture_CubeTex");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));*/
	
	return S_OK;
}

void CCubeObj::SetTransform(void)
{
	const D3DXMATRIX*		pMatView = m_pCamObserver->GetView();
	NULL_CHECK(pMatView);

	const D3DXMATRIX*		pMatProj = m_pCamObserver->GetProj();
	NULL_CHECK(pMatProj);

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		if(m_wstrResourceKey == L"Buffer_CubeTex")
		{
			m_pConvertVertex[i] = m_pVertex[i];

			//X
			if(i == 0 || i == 4 || i == 7 || i == 3)
			{
				m_pConvertVertex[i].vPos.x -= m_fItv_X;
			}
			else
			{
				m_pConvertVertex[i].vPos.x += m_fItv_X;
			}

			//YÃà
			if(i == 0 || i == 4 || i == 5 || i == 1)
			{
				m_pConvertVertex[i].vPos.y += m_fItv_Y;
			}
			else
			{
				m_pConvertVertex[i].vPos.y -= m_fItv_Y;
			}

			//ZÃà
			if(i >= 0 && i < 4)
			{
				m_pConvertVertex[i].vPos.z -= m_fItv_Z;
			}
			else
			{
				m_pConvertVertex[i].vPos.z += m_fItv_Z;
			}

			ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
			ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pMatView);

			if(m_pConvertVertex[i].vPos.z < 1.f)
				m_pConvertVertex[i].vPos.z = 1.f;

			ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, pMatProj);
		}
		else
		{
			m_pConvertVertex_Col[i] = m_pVertex_Col[i];

			ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex_Col[i].vPos, &m_pConvertVertex_Col[i].vPos, &m_pInfo->m_matWorld);
			ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex_Col[i].vPos, &m_pConvertVertex_Col[i].vPos, pMatView);

			if(m_pConvertVertex_Col[i].vPos.z < 1.f)
				m_pConvertVertex_Col[i].vPos.z = 1.f;

			ENGINE::CPipeline::MyTransformCoord(&m_pConvertVertex_Col[i].vPos, &m_pConvertVertex_Col[i].vPos, pMatProj);
		}		
	}
}

CCubeObj* CCubeObj::Create(LPDIRECT3DDEVICE9 pDevice, 
						   const wstring& wstrResourceKey)
{
	CCubeObj*		pCubeObj = new CCubeObj(pDevice, wstrResourceKey);

	if(FAILED(pCubeObj->Initialize()))
		ENGINE::Safe_Delete(pCubeObj);

	return pCubeObj;
}

void CCubeObj::Update(void)
{
	/*if(m_bMove == true)
	{
		Move();
	}*/

	SetDirection();

	ENGINE::CPipeline::MakeWorldMatrix(&m_pInfo->m_matWorld, 
		&D3DXVECTOR3(1.f, 1.f, 1.f), 
		m_pInfo->m_fAngle, 
		&m_pInfo->m_vPos);

	ENGINE::CGameObject::Update();

	SetTransform();
}

void CCubeObj::Render(void)
{
	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00000088);
	m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if(m_wstrResourceKey == L"Buffer_CubeTex")
	{
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, m_wstrResourceKey, m_pConvertVertex);
	}
	else
	{
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, m_wstrResourceKey, m_pConvertVertex_Col);
	}

	if(m_bSetTex)
		m_pTexture->Render(m_iCount);

	m_pBuffer->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	if(m_wstrResourceKey == L"Buffer_CubeTex")
	{
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, m_wstrResourceKey, m_pVertex);
	}
}

void CCubeObj::Release(void)
{
	if(m_pVertex != NULL)
	{
		ENGINE::Safe_Delete_Array(m_pVertex);
	}

	if(m_pVertex_Col != NULL)
	{
		ENGINE::Safe_Delete_Array(m_pVertex_Col);
	}

	if(m_pConvertVertex != NULL)
	{
		ENGINE::Safe_Delete_Array(m_pConvertVertex);
	}

	if(m_pConvertVertex_Col != NULL)
	{
		ENGINE::Safe_Delete_Array(m_pConvertVertex_Col);
	}

	ENGINE::CInfoSubject::GetInstance()->UnSubscribe(m_pCamObserver);
	ENGINE::Safe_Delete(m_pCamObserver);
}

void CCubeObj::SetPos(const D3DXVECTOR3& vPos)
{
	m_pInfo->m_vPos = vPos;
}

void CCubeObj::PickCube(CPoint _pos)
{
	m_fDist = 9999999999.f;
	m_pMouseCol->PickObject(_pos, m_pVertex, &m_pInfo->m_matWorld, m_ePickID, m_fDist, D3DXVECTOR3(m_fItv_X, m_fItv_Y, m_fItv_Z));

	//if(m_ePickID != ENGINE::PICK_NONE)
	//{
	//	//const ENGINE::CComponent* TempComp = GetComponent(L"Buffer");

	//	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	//	m_pObjToolView = (CObjToolView*)pMainFrame->GetActiveView();

	//	const ENGINE::ITVXYZ* pITVXYZ = m_pBuffer->GetItv();

	//	m_pObjToolView->CreatePickingCube(m_ePickID, pITVXYZ, m_pInfo->m_vPos);
	//	m_ePickID = ENGINE::PICK_NONE;

	//	return true;
	//}

	//return false;
}

void CCubeObj::SetTexture(int _iSel)
{
	m_bSetTex = true;
	m_iCount = _iSel;
	ENGINE::CComponent*	pComponent = NULL;

	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture_Cube");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
}

void CCubeObj::Move(void)
{
	/*if(GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
		m_pInfo->m_vPos.y += 0.02f;
	if(GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
		m_pInfo->m_vPos.y -= 0.02f;
	if(GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
		m_pInfo->m_vPos.x -= 0.02f;
	if(GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
		m_pInfo->m_vPos.x += 0.02f;
	if(GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
		m_pInfo->m_vPos.z += 0.02f;
	if(GetAsyncKeyState(VK_NUMPAD9) & 0x8000)
		m_pInfo->m_vPos.z -= 0.02f;*/
}

