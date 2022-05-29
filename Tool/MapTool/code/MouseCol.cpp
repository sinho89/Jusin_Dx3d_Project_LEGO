#include "stdafx.h"
#include "MouseCol.h"

#include "Export_Function.h"
#include "CameraObserver.h"

CMouseCol::CMouseCol(void)
: m_pInfoSubject(ENGINE::Get_InfoSubject())
, m_pCameraObserver(NULL)
{

}

CMouseCol::~CMouseCol(void)
{
	Release();
}


CMouseCol* CMouseCol::Create(void)
{
	CMouseCol* pMouseCol = new CMouseCol;

	if(FAILED(pMouseCol->Initialize()))
		ENGINE::Safe_Delete(pMouseCol);

	return pMouseCol;
}

ENGINE::CCollision* CMouseCol::Clone(void)
{
	++(*m_pwRefCnt);

	return new CMouseCol(*this);
}

void CMouseCol::PickTerrain(CPoint	_Pos,
							D3DXVECTOR3* pOut, 
							ENGINE::VTXTEX* pTerrainVtx,
							int _VtxCntX,
							int _VtxCntZ,
							int	_VtxItv,
							float _VtxHeight)
{
	Translation_ViewSpace(_Pos);

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	Translation_Local(&matIdentity);

	ENGINE::VTXTEX*		pVertex = pTerrainVtx;

	float	fU, fV, fDist;

	for(int z = 0; z < _VtxCntZ - 1; ++z)
	{
		for(int x = 0; x < _VtxCntX - 1; ++x)
		{
			int		iIndex = z * _VtxCntX + x;

			// 오른쪽 위
			if(D3DXIntersectTri(&pVertex[iIndex + _VtxCntX + 1].vPos,
				&pVertex[iIndex + _VtxCntX].vPos,
				&pVertex[iIndex + 1]. vPos,
				&m_vPivotPos,
				&m_vRayDir,
				&fU, &fV, &fDist))
			{
				*pOut = pVertex[iIndex + _VtxCntX + 1].vPos +
						(pVertex[iIndex + _VtxCntX].vPos - pVertex[iIndex + _VtxCntX + 1].vPos) * fU +
						(pVertex[iIndex + 1].vPos - pVertex[iIndex + _VtxCntX + 1].vPos) * fV;

				pVertex[iIndex + _VtxCntX + 1].vPos.y += _VtxHeight;
				pVertex[iIndex + _VtxCntX].vPos.y += _VtxHeight;
				pVertex[iIndex + 1].vPos.y += _VtxHeight;

				return;
			}
			// 왼쪽 아래
			if(D3DXIntersectTri(&pVertex[iIndex].vPos,
				&pVertex[iIndex + 1].vPos,
				&pVertex[iIndex + _VtxCntX]. vPos,
				&m_vPivotPos,
				&m_vRayDir,
				&fU, &fV, &fDist))
			{
				*pOut = pVertex[iIndex].vPos +
					(pVertex[iIndex + 1].vPos - pVertex[iIndex].vPos) * fU +
					(pVertex[iIndex + _VtxCntX].vPos - pVertex[iIndex].vPos) * fV;


				pVertex[iIndex].vPos.y += _VtxHeight;
				pVertex[iIndex + _VtxCntX].vPos.y += _VtxHeight;
				pVertex[iIndex + 1].vPos.y += _VtxHeight;
				return;
			}

		}
	}


}

void CMouseCol::PickObject(D3DXVECTOR3* pOut, 
						   const ENGINE::VTXTEX* pVertex, 
						   const D3DXMATRIX* pMatWorld)
{
	//Translation_ViewSpace();
	Translation_Local(pMatWorld);

	float fU, fV, fDist;

	if(D3DXIntersectTri(&pVertex[1].vPos,
		&pVertex[0].vPos,
		&pVertex[2]. vPos,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = pVertex[1].vPos +
			(pVertex[0].vPos - pVertex[1].vPos) * fU +
			(pVertex[2].vPos - pVertex[1].vPos) * fV;

		return;
	}

	if(D3DXIntersectTri(&pVertex[3].vPos,
		&pVertex[2].vPos,
		&pVertex[0]. vPos,
		&m_vPivotPos,
		&m_vRayDir,
		&fU, &fV, &fDist))
	{
		*pOut = pVertex[3].vPos +
			(pVertex[2].vPos - pVertex[3].vPos) * fU +
			(pVertex[0].vPos - pVertex[3].vPos) * fV;

		return;
	}
}

HRESULT CMouseCol::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	return S_OK;
}

void CMouseCol::Translation_ViewSpace(CPoint _Pos)
{
	CPoint	ptMouse = _Pos;

	D3DXMATRIX	matProj = *m_pCameraObserver->GetProj();

	D3DXVECTOR3		vTemp;

	vTemp.x = (float(ptMouse.x) / (ENGINE::WINCX >> 1) - 1.f) / matProj._11;
	vTemp.y = (float(-ptMouse.y) / (ENGINE::WINCY >> 1) + 1.f) / matProj._22;
	vTemp.z = 1.f;

	m_vPivotPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vRayDir   = vTemp - m_vPivotPos;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

	
}

void CMouseCol::Translation_Local(const D3DXMATRIX* pWorld)
{
	D3DXMATRIX	matView = *m_pCameraObserver->GetView();

	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);


	D3DXMATRIX	matWorld;
	D3DXMatrixInverse(&matWorld, NULL, pWorld);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matWorld);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorld);
}

void CMouseCol::Release(void)
{
	if((*m_pwRefCnt) == 0)
	{
		ENGINE::CCollision::Release();
		ENGINE::Safe_Delete(m_pCameraObserver);
	}

	else
	{
		--(*m_pwRefCnt);
	}

	
}

