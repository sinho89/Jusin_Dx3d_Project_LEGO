#include "stdafx.h"
#include "TerrainCol.h"
#include "Terrain.h"

CTerrainCol::CTerrainCol(void)
: m_pTerrain(NULL)
, m_bCollision(true)
, m_JumpAble(false)
{
	
}

CTerrainCol::~CTerrainCol(void)
{
	Release();
}

void CTerrainCol::Update(void)
{
	int iItv	= *(m_pTerrain->GetVtxItv());
	int iCntX	= *(m_pTerrain->GetVtxCntX());

	int		iIndex = (int(m_pPos->z) / iItv) * iCntX + (int(m_pPos->x) / iItv);

	if(iIndex < 0)
		return;

	float	fRatioX = (m_pPos->x - m_pTerrainVtx[iIndex + iCntX].vPos.x) / iItv;
	float	fRatioZ = (m_pTerrainVtx[iIndex + iCntX].vPos.z - m_pPos->z) / iItv;

	D3DXPLANE		Plane;

	if(fRatioX > fRatioZ)	// 오른쪽 위
	{
		D3DXPlaneFromPoints(&Plane, 
			&m_pTerrainVtx[iIndex + iCntX].vPos,
			&m_pTerrainVtx[iIndex + iCntX + 1].vPos,
			&m_pTerrainVtx[iIndex + 1].vPos);
		//충돌되는 불값을 바꿔주고 리턴... ㅇㅋ ? ㅇ ㅇㅋ
	}
	else					// 왼쪽 아래
	{
		D3DXPlaneFromPoints(&Plane, 
			&m_pTerrainVtx[iIndex + iCntX].vPos,
			&m_pTerrainVtx[iIndex + 1].vPos,
			&m_pTerrainVtx[iIndex].vPos);
		//충돌
	}

	// ax + by + cz + d = 0
	// by = - ax - cz - d 
	// y = (- ax - cz - d) / b 

	if(m_bCollision == true)
	{
		m_pPos->y = (-Plane.a * m_pPos->x - Plane.c * m_pPos->z - Plane.d) / Plane.b;
		m_pPos->y += 2.f;
	}
}

ENGINE::CCollision* CTerrainCol::Clone(void)
{
	++(*m_pwRefCnt);

	return new CTerrainCol(*this);
}

CTerrainCol* CTerrainCol::Create(void)
{
	return new CTerrainCol;
}

void CTerrainCol::SetColInfo(D3DXVECTOR3* pPos, const ENGINE::VTXTEX* pTerrainVtx, CTerrain* pTerrain)
{
	m_pPos			= pPos;
	m_pTerrainVtx	= pTerrainVtx;
	m_pTerrain		= pTerrain;
}

void CTerrainCol::Release(void)
{
	if((*m_pwRefCnt) == 0)
	{
		ENGINE::CCollision::Release();
	}

	else
	{
		--(*m_pwRefCnt);
	}
}

bool& CTerrainCol::GetCollision(void)
{
	return m_bCollision;
}

bool& CTerrainCol::GetJumpAble(void)
{
	return m_JumpAble;
}


