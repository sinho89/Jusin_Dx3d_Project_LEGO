#include "stdafx.h"
#include "Sword.h"

CSword::CSword(LPDIRECT3DDEVICE9 pDevice)
: CCubeObject(pDevice)
, m_eUnitType(UT_PLAYER)
, m_pGuardBuffer(NULL)
, m_pSwordBuffer(NULL)
{

}

CSword::CSword(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType)
: CCubeObject(pDevice)
, m_eUnitType(eUnitType)
, m_pGuardBuffer(NULL)
, m_pSwordBuffer(NULL)
{
	
}

CSword::~CSword(void)
{
	Release();
}

HRESULT CSword::Initialize(void)
{
	CCubeObject::Initialize();

	m_pHandGuard_Vertex				=	new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pHandGuard_ConverterVertex	=	new ENGINE::VTXCUBE[m_dwVtxCnt];

	m_pSword_Vertex					=	new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pSword_ConverterVertex		=	new ENGINE::VTXCUBE[m_dwVtxCnt];
	
	switch(m_eUnitType)
	{
	case UT_PLAYER:
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_P_Sword_Hand_Cube", m_pVertex);
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_P_Sword_Guard_Cube", m_pHandGuard_Vertex);
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_P_Sword_Blade_Cube", m_pSword_Vertex);
		break;

	case UT_ORC:
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_M_Sword_Hand_Cube", m_pVertex);
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_M_Sword_Guard_Cube", m_pHandGuard_Vertex);
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_M_Sword_Blade_Cube", m_pSword_Vertex);
		break;

	case UT_BOSS:
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Boss_Sword_Hand_Cube", m_pVertex);
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Boss_Sword_Guard_Cube", m_pHandGuard_Vertex);
		m_pResourcesMgr->GetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Boss_Sword_Blade_Cube", m_pSword_Vertex);
		break;
	}

	TCHAR szFilePath[MAX_PATH] = L"../../Data/Object_Data/Genzi_Sword.dat";
	HANDLE	hFile	= CreateFile(szFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwByte	= 0;

	bool b1 = true, b2 = true, b3 = true;

	while(true)
	{
		ENGINE::SAVECUBE*	pSaveCube = new ENGINE::SAVECUBE;
		ReadFile(hFile, pSaveCube, sizeof(ENGINE::SAVECUBE), &dwByte, NULL);

		if(dwByte == 0)
		{
			ENGINE::Safe_Delete(pSaveCube);
			break;
		}
		
		if(b1)
		{
			m_fItv[0][ENGINE::ITV_X] = pSaveCube->m_fItv_X;
			m_fItv[0][ENGINE::ITV_Y] = pSaveCube->m_fItv_Y;
			m_fItv[0][ENGINE::ITV_Z] = pSaveCube->m_fItv_Z;

			m_fAngle[0][ENGINE::ANGLE_X] = pSaveCube->m_fAngle[ENGINE::ANGLE_X];
			m_fAngle[0][ENGINE::ANGLE_Y] = pSaveCube->m_fAngle[ENGINE::ANGLE_Y];
			m_fAngle[0][ENGINE::ANGLE_Z] = pSaveCube->m_fAngle[ENGINE::ANGLE_Z];

			m_vSwordDir[0] = pSaveCube->m_vDir;
			m_vSwordPos[0] = pSaveCube->m_vPos;

			b1 = false;
		}
		else if(b2)
		{
			m_fItv[1][ENGINE::ITV_X] = pSaveCube->m_fItv_X;
			m_fItv[1][ENGINE::ITV_Y] = pSaveCube->m_fItv_Y;
			m_fItv[1][ENGINE::ITV_Z] = pSaveCube->m_fItv_Z;

			m_fAngle[1][ENGINE::ANGLE_X] = pSaveCube->m_fAngle[ENGINE::ANGLE_X];
			m_fAngle[1][ENGINE::ANGLE_Y] = pSaveCube->m_fAngle[ENGINE::ANGLE_Y];
			m_fAngle[1][ENGINE::ANGLE_Z] = pSaveCube->m_fAngle[ENGINE::ANGLE_Z];

			m_vSwordDir[1] = pSaveCube->m_vDir;
			m_vSwordPos[1] = pSaveCube->m_vPos;

			b2 = false;
		}
		else if(b3)
		{
			m_fItv[2][ENGINE::ITV_X] = pSaveCube->m_fItv_X;
			m_fItv[2][ENGINE::ITV_Y] = pSaveCube->m_fItv_Y;
			m_fItv[2][ENGINE::ITV_Z] = pSaveCube->m_fItv_Z;

			m_fAngle[2][ENGINE::ANGLE_X] = pSaveCube->m_fAngle[ENGINE::ANGLE_X];
			m_fAngle[2][ENGINE::ANGLE_Y] = pSaveCube->m_fAngle[ENGINE::ANGLE_Y];
			m_fAngle[2][ENGINE::ANGLE_Z] = pSaveCube->m_fAngle[ENGINE::ANGLE_Z];

			m_vSwordDir[2] = pSaveCube->m_vDir;
			m_vSwordPos[2] = pSaveCube->m_vPos;

			b3 = false;
		}		

		ENGINE::Safe_Delete(pSaveCube);
	}	

	CloseHandle(hFile);

	FAILED_CHECK(AddComponent());

	return S_OK;
}

HRESULT CSword::AddComponent(void)
{
	ENGINE::CComponent*		pComponent = NULL;

	pComponent = m_pInfo = ENGINE::CTransform::Create(ENGINE::g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	switch(m_eUnitType)
	{
	case UT_PLAYER:
		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_P_Sword_Hand_Cube");
		m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Hand", pComponent));

		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_P_Sword_Guard_Cube");
		m_pGuardBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pGuardBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Guard", pComponent));

		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_P_Sword_Blade_Cube");
		m_pSwordBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pSwordBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Sword", pComponent));
		break;

	case UT_ORC:
		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_M_Sword_Hand_Cube");
		m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_M_Sword_Guard_Cube");
		m_pGuardBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pGuardBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Guard", pComponent));

		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_M_Sword_Blade_Cube");
		m_pSwordBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pSwordBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Sword", pComponent));
		break;

	case UT_BOSS:
		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Boss_Sword_Hand_Cube");
		m_pBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Boss_Sword_Guard_Cube");
		m_pGuardBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pGuardBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Guard", pComponent));

		pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Boss_Sword_Blade_Cube");
		m_pSwordBuffer = dynamic_cast<ENGINE::CVIBuffer*>(pComponent);
		NULL_CHECK_RETURN(m_pSwordBuffer, E_FAIL);
		m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer_Sword", pComponent));
		break;
	}	

	pComponent = m_pResourcesMgr->CloneResource(ENGINE::RESOURCE_DYNAMIC, L"Texture SkyBox");
	m_pTexture = dynamic_cast<ENGINE::CTexture*>(pComponent);
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

void CSword::Update(void)
{
	CCubeObject::SetDirection();

	/*D3DXMatrixScaling(&m_mMat[ENGINE::MAT_SCALE], 1.f, 1.f, 1.f);
	D3DXMatrixRotationX(&m_mMat[ENGINE::MAT_ROT_X], m_pInfo->m_fAngle[ENGINE::ANGLE_X]);
	D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);
	D3DXMatrixRotationZ(&m_mMat[ENGINE::MAT_ROT_Z], m_pInfo->m_fAngle[ENGINE::ANGLE_Z]);*/
	//D3DXMatrixTranslation(&m_mMat[ENGINE::MAT_TRANS], m_pInfo->m_vPos.x * 1.f, m_pInfo->m_vPos.y * 1.f, m_pInfo->m_vPos.z * 1.f);

	ENGINE::CGameObject::Update();

	for(int j=0; j<3; ++j)
	{
		if(m_eUnitType == UT_BOSS)
			D3DXMatrixScaling(&m_mMat[ENGINE::MAT_SCALE], 8.f, 8.f, 8.f);
		else
			D3DXMatrixScaling(&m_mMat[ENGINE::MAT_SCALE], 0.5f, 0.5f, 0.5f);
		
		D3DXMatrixRotationX(&m_mMat[ENGINE::MAT_ROT_X], m_pInfo->m_fAngle[ENGINE::ANGLE_X]);
		D3DXMatrixRotationY(&m_mMat[ENGINE::MAT_ROT_Y], m_pInfo->m_fAngle[ENGINE::ANGLE_Y]);
		D3DXMatrixRotationZ(&m_mMat[ENGINE::MAT_ROT_Z], m_pInfo->m_fAngle[ENGINE::ANGLE_Z]);
		
		if(m_eUnitType == UT_BOSS)
		{
			D3DXMatrixTranslation(&m_mMat[ENGINE::MAT_TRANS]
								,m_pInfo->m_vPos.x + 2.f + m_vSwordPos[j].x * 8.f
								,m_pInfo->m_vPos.y - 0.f + m_vSwordPos[j].y * 8.f
								,m_pInfo->m_vPos.z - 6.f + m_vSwordPos[j].z * 8.f);
		}
		else
		{
			D3DXMatrixTranslation(&m_mMat[ENGINE::MAT_TRANS]
								,m_pInfo->m_vPos.x + m_vSwordPos[j].x * 0.5f
								,m_pInfo->m_vPos.y + m_vSwordPos[j].y * 0.5f
								,m_pInfo->m_vPos.z + m_vSwordPos[j].z * 0.5f);
		}

		

		D3DXMatrixIdentity(&m_pInfo->m_matWorld);
		m_pInfo->m_matWorld =	m_mMat[ENGINE::MAT_SCALE] *  
								m_mMat[ENGINE::MAT_ROT_X] * 
								m_mMat[ENGINE::MAT_ROT_Y] * 
								m_mMat[ENGINE::MAT_ROT_Z] * 
								m_mMat[ENGINE::MAT_TRANS] * 
								m_mMat[ENGINE::MAT_AXIS] *
								m_mMat[ENGINE::MAT_PARENT];
		
		for(size_t i = 0; i < m_dwVtxCnt; ++i)
		{
			if(j == 0)
			{
				m_pConvertVertex[i] = m_pVertex[i];

				//X
				if(i == 0 || i == 4 || i == 7 || i == 3)
				{
					m_pConvertVertex[i].vPos.x -= m_fItv[j][ENGINE::ITV_X];
				}										
				else									
				{										
					m_pConvertVertex[i].vPos.x += m_fItv[j][ENGINE::ITV_X];
				}

				//Y축
				if(i == 0 || i == 4 || i == 5 || i == 1)
				{
					m_pConvertVertex[i].vPos.y += m_fItv[j][ENGINE::ITV_Y];
				}										
				else									
				{										
					m_pConvertVertex[i].vPos.y -= m_fItv[j][ENGINE::ITV_Y];
				}

				//Z축
				if(i >= 0 && i < 4)
				{
					m_pConvertVertex[i].vPos.z -= m_fItv[j][ENGINE::ITV_Z];
				}										
				else									
				{										
					m_pConvertVertex[i].vPos.z += m_fItv[j][ENGINE::ITV_Z];
				}

				D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
			}
			else if(j == 1)
			{
				m_pHandGuard_ConverterVertex[i] = m_pHandGuard_Vertex[i];

				//X
				if(i == 0 || i == 4 || i == 7 || i == 3)
				{
					m_pHandGuard_ConverterVertex[i].vPos.x -= m_fItv[j][ENGINE::ITV_X];
				}													
				else												
				{													
					m_pHandGuard_ConverterVertex[i].vPos.x += m_fItv[j][ENGINE::ITV_X];
				}

				//Y축
				if(i == 0 || i == 4 || i == 5 || i == 1)
				{
					m_pHandGuard_ConverterVertex[i].vPos.y += m_fItv[j][ENGINE::ITV_Y];
				}													
				else												
				{													
					m_pHandGuard_ConverterVertex[i].vPos.y -= m_fItv[j][ENGINE::ITV_Y];
				}

				//Z축
				if(i >= 0 && i < 4)
				{
					m_pHandGuard_ConverterVertex[i].vPos.z -= m_fItv[j][ENGINE::ITV_Z];
				}													
				else												
				{													
					m_pHandGuard_ConverterVertex[i].vPos.z += m_fItv[j][ENGINE::ITV_Z];
				}

				D3DXVec3TransformCoord(&m_pHandGuard_ConverterVertex[i].vPos, &m_pHandGuard_ConverterVertex[i].vPos, &m_pInfo->m_matWorld);
			}
			else if(j == 2)
			{
				m_pSword_ConverterVertex[i] = m_pSword_Vertex[i];

				//X
				if(i == 0 || i == 4 || i == 7 || i == 3)
				{
					m_pSword_ConverterVertex[i].vPos.x -= m_fItv[j][ENGINE::ITV_X];
				}												
				else											
				{												
					m_pSword_ConverterVertex[i].vPos.x += m_fItv[j][ENGINE::ITV_X];
				}

				//Y축
				if(i == 0 || i == 4 || i == 5 || i == 1)
				{
					m_pSword_ConverterVertex[i].vPos.y += m_fItv[j][ENGINE::ITV_Y];
				}												
				else											
				{												
					m_pSword_ConverterVertex[i].vPos.y -= m_fItv[j][ENGINE::ITV_Y];
				}

				//Z축
				if(i >= 0 && i < 4)
				{
					m_pSword_ConverterVertex[i].vPos.z -= m_fItv[j][ENGINE::ITV_Z];
				}												
				else											
				{												
					m_pSword_ConverterVertex[i].vPos.z += m_fItv[j][ENGINE::ITV_Z];
				}

				D3DXVec3TransformCoord(&m_pSword_ConverterVertex[i].vPos, &m_pSword_ConverterVertex[i].vPos, &m_pInfo->m_matWorld);
			}
		}
	}	
}

void CSword::Render(void)
{
	CCubeObject::Render();

	switch(m_eUnitType)
	{
	case UT_PLAYER:
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_P_Sword_Hand_Cube", m_pConvertVertex);
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_P_Sword_Guard_Cube", m_pHandGuard_ConverterVertex);
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_P_Sword_Blade_Cube", m_pSword_ConverterVertex);
		break;

	case UT_ORC:
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_M_Sword_Hand_Cube", m_pConvertVertex);
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_M_Sword_Guard_Cube", m_pHandGuard_ConverterVertex);
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_M_Sword_Blade_Cube", m_pSword_ConverterVertex);
		break;

	case UT_BOSS:
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Boss_Sword_Hand_Cube", m_pConvertVertex);
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Boss_Sword_Guard_Cube", m_pHandGuard_ConverterVertex);
		m_pResourcesMgr->SetVtxInfo(ENGINE::RESOURCE_DYNAMIC, L"Buffer_Boss_Sword_Blade_Cube", m_pSword_ConverterVertex);
		break;
	}

	m_pTexture->Render(0);

	m_pBuffer->Render();
	m_pGuardBuffer->Render();
	m_pSwordBuffer->Render();
}

void CSword::Release(void)
{

}

CSword* CSword::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSword* pSword = new CSword(pDevice);

	if(FAILED(pSword->Initialize()))
		ENGINE::Safe_Delete(pSword);

	return pSword;
}

CSword* CSword::Create(LPDIRECT3DDEVICE9 pDevice, UNITTYPE eUnitType)
{
	CSword* pSword = new CSword(pDevice, eUnitType);

	if(FAILED(pSword->Initialize()))
		ENGINE::Safe_Delete(pSword);

	return pSword;
}

D3DXVECTOR3& CSword::GetSwordPos(int _iIndex)
{
	return m_vSwordPos[_iIndex];
}

float* CSword::GetSwordAngle(int _iIndex)
{
	return m_fAngle[_iIndex];
}

