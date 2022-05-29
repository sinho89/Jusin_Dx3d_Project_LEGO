#include "stdafx.h"
#include "CubeObject.h"

#include "Pipeline.h"

CCubeObject::CCubeObject( LPDIRECT3DDEVICE9 pDevice )
:CClientObj(pDevice)
, m_pVertex(NULL)
, m_pConvertVertex(NULL)
, m_dwVtxCnt(0)
{
	int mat = ENGINE::MAT_END;

	for(int i=0; i<ENGINE::MAT_END; ++i)
		D3DXMatrixIdentity(&m_mMat[i]);

	ZeroMemory(m_fItv, sizeof(float)*3);
}

CCubeObject::~CCubeObject()
{
	Release();
}

HRESULT CCubeObject::Initialize( void )
{
	m_dwVtxCnt			= 8;
	m_pVertex			= new ENGINE::VTXCUBE[m_dwVtxCnt];
	m_pConvertVertex	= new ENGINE::VTXCUBE[m_dwVtxCnt];	

	return S_OK;
}

void CCubeObject::Update( void )
{
	CClientObj::Update();

	for(size_t i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pConvertVertex[i] = m_pVertex[i];

		//X
		if(i == 0 || i == 4 || i == 7 || i == 3)
		{
			m_pConvertVertex[i].vPos.x -= m_fItv[ENGINE::ITV_X];
		}
		else
		{
			m_pConvertVertex[i].vPos.x += m_fItv[ENGINE::ITV_X];
		}

		//YÃà
		if(i == 0 || i == 4 || i == 5 || i == 1)
		{
			m_pConvertVertex[i].vPos.y += m_fItv[ENGINE::ITV_Y];
		}
		else
		{
			m_pConvertVertex[i].vPos.y -= m_fItv[ENGINE::ITV_Y];
		}

		//ZÃà
		if(i >= 0 && i < 4)
		{
			m_pConvertVertex[i].vPos.z -= m_fItv[ENGINE::ITV_Z];
		}
		else
		{
			m_pConvertVertex[i].vPos.z += m_fItv[ENGINE::ITV_Z];
		}

		D3DXVec3TransformCoord(&m_pConvertVertex[i].vPos, &m_pConvertVertex[i].vPos, &m_pInfo->m_matWorld);
	}	
}

void CCubeObject::Render( void )
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CCubeObject::Release(void)
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

void CCubeObject::SetInterver(ENGINE::SAVECUBE* pSaveCube)
{
	m_fItv[ENGINE::ITV_X] = pSaveCube->m_fItv_X;
	m_fItv[ENGINE::ITV_Y] = pSaveCube->m_fItv_Y;
	m_fItv[ENGINE::ITV_Z] = pSaveCube->m_fItv_Z;
}

void CCubeObject::SetDirection(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &ENGINE::g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDir, &m_pInfo->m_vDir);
}
