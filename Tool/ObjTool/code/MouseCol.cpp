#include "stdafx.h"
#include "MouseCol.h"

#include "Export_Function.h"
#include "CameraObserver.h"

CMouseCol::CMouseCol(void)
: m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCamObserver(NULL)
{

	
}

CMouseCol::~CMouseCol(void)
{
	Release();
}

POINT CMouseCol::GetMousePos(void)
{
	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(ENGINE::g_hWnd, &pt);

	return pt;
}

CMouseCol* CMouseCol::Create(void)
{
	CMouseCol*	pMouseCol = new CMouseCol;

	if(FAILED(pMouseCol->Initialize()))
		ENGINE::Safe_Delete(pMouseCol);

	return pMouseCol;
}

ENGINE::CCollision* CMouseCol::Clone(void)
{
	++(*m_pwRefCnt);

	return new CMouseCol(*this);
}

void CMouseCol::PickObject(CPoint _pos,//D3DXVECTOR3* pOut, 
						   ENGINE::VTXCUBE* pVertex, 
						   const D3DXMATRIX* pMatWorld,
						   ENGINE::PICKINGID&	ePickID,
						   float&				_fDist,
						   D3DXVECTOR3&			_vecItv)
{

	CPoint ptMouse = _pos;

	Translation_ViewSpae(ptMouse);
	Translation_Local(pMatWorld);
	
	float fU, fV, fDist;


	bool	bCubeCheck = false;

	//rDist = 9999999999.f;

	ePickID = ENGINE::PICK_NONE;

	//z-
	if(D3DXIntersectTri(&(pVertex[1].vPos + _vecItv),
		&(pVertex[0].vPos + _vecItv),
		&(pVertex[2].vPos + _vecItv),
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(_fDist > fDist)
		{
			bCubeCheck = true;
			_fDist = fDist;
			ePickID = ENGINE::PICK_Z_M;
		}

	}

	if(D3DXIntersectTri(&(pVertex[3].vPos + _vecItv),
		&(pVertex[2].vPos + _vecItv),
		&(pVertex[0].vPos + _vecItv),
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(_fDist > fDist)
		{
			_fDist = fDist;
			ePickID = ENGINE::PICK_Z_M;
		}
	}

	//z+
	if(D3DXIntersectTri(&(pVertex[6].vPos + _vecItv),
		&(pVertex[7].vPos + _vecItv),
		&(pVertex[5].vPos + _vecItv),
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(_fDist > fDist)
		{
			bCubeCheck = true;
			_fDist = fDist;
			ePickID = ENGINE::PICK_Z_P;
		}
	}

	if(D3DXIntersectTri(&(pVertex[4].vPos + _vecItv),
		&(pVertex[5].vPos + _vecItv),
		&(pVertex[7].vPos + _vecItv),
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(_fDist > fDist)
		{
			bCubeCheck = true;
			_fDist = fDist;
			ePickID = ENGINE::PICK_Z_P;
		}
	}

	// y+ 
	if(D3DXIntersectTri(&(pVertex[0].vPos + _vecItv),
		&(pVertex[1].vPos + _vecItv),
		&(pVertex[4].vPos + _vecItv),
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(_fDist > fDist)
		{
			bCubeCheck = true;
			_fDist = fDist;
			ePickID = ENGINE::PICK_Y_P;
		}
	}

	if(D3DXIntersectTri(&(pVertex[5].vPos + _vecItv),
		&(pVertex[4].vPos + _vecItv),
		&(pVertex[1].vPos + _vecItv),
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(_fDist > fDist)
		{
			bCubeCheck = true;
			_fDist = fDist;
			ePickID = ENGINE::PICK_Y_P;
		}
	}

	// y- 
	if(D3DXIntersectTri(&(pVertex[2].vPos + _vecItv),
		&(pVertex[3].vPos + _vecItv),
		&(pVertex[6].vPos + _vecItv),
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(_fDist > fDist)
		{
			bCubeCheck = true;
			_fDist = fDist;
			ePickID = ENGINE::PICK_Y_M;
		}
	}

	if(D3DXIntersectTri(&(pVertex[7].vPos + _vecItv),
		&(pVertex[6].vPos + _vecItv),
		&(pVertex[3].vPos + _vecItv),
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(_fDist > fDist)
		{
			bCubeCheck = true;
			_fDist = fDist;
			ePickID = ENGINE::PICK_Y_M;
		}
	}

	// x+ 
	if(D3DXIntersectTri(&(pVertex[5].vPos + _vecItv),
		&(pVertex[1].vPos + _vecItv),
		&(pVertex[6].vPos + _vecItv),
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(_fDist > fDist)
		{
			bCubeCheck = true;
			_fDist = fDist;
			ePickID = ENGINE::PICK_X_P;
		}
	}

	if(D3DXIntersectTri(&(pVertex[2].vPos + _vecItv),
		&(pVertex[6].vPos + _vecItv),
		&(pVertex[1].vPos + _vecItv),
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(_fDist > fDist)
		{
			bCubeCheck = true;
			_fDist = fDist;
			ePickID = ENGINE::PICK_X_P;
		}
	}

	// x- 
	if(D3DXIntersectTri(&(pVertex[0].vPos + _vecItv),
		&(pVertex[4].vPos + _vecItv),
		&(pVertex[3].vPos + _vecItv),
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(_fDist > fDist)
		{
			bCubeCheck = true;
			_fDist = fDist;
			ePickID = ENGINE::PICK_X_M;
		}
	}

	if(D3DXIntersectTri(&(pVertex[7].vPos + _vecItv),
		&(pVertex[3].vPos + _vecItv),
		&(pVertex[4].vPos + _vecItv),
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		if(_fDist > fDist)
		{
			bCubeCheck = true;
			_fDist = fDist;
			ePickID = ENGINE::PICK_X_M;
		}
	}


	
}

HRESULT CMouseCol::Initialize(void)
{
	m_pCamObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCamObserver);

	return S_OK;
}

void CMouseCol::Translation_ViewSpae(CPoint _pos)
{
	POINT	ptMouse = _pos;

	D3DXMATRIX matProj = *m_pCamObserver->GetProj();

	D3DXVECTOR3	vTemp;

	vTemp.x = (float(ptMouse.x) / (ENGINE::WINCX >> 1) - 1.f) / matProj._11;
	vTemp.y = (float(-ptMouse.y) / (ENGINE::WINCY >> 1) + 1.f) / matProj._22;
	vTemp.z = 1.f;

	m_vPivotPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vRayDir	= vTemp - m_vPivotPos;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

void CMouseCol::Translation_Local(const D3DXMATRIX* pWorld)
{
	D3DXMATRIX matView = *m_pCamObserver->GetView();

	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);

	D3DXMATRIX matWorld;
	D3DXMatrixInverse(&matWorld, NULL, pWorld);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matWorld);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorld);
}

void CMouseCol::Release(void)
{
	if((*m_pwRefCnt) == 0)
	{
		ENGINE::CCollision::Release();
		ENGINE::Safe_Delete(m_pCamObserver);
	}
	else
	{
		--(*m_pwRefCnt);
	}
}

