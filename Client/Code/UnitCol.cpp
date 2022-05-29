#include "stdafx.h"
#include "UnitCol.h"
#include "Heart.h"
#include "LayerMgr.h"
#include "Player_Elond.h"
#include "GameObject.h"
#include "TimeMgr.h"
#include "Monster_Orc.h"
#include "SoundMgr.h"

CUnitCol::CUnitCol(void)
: m_pTime(ENGINE::Get_Time())
, m_bAtt(false)
{
	*m_pPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_fTime = 0.f;
}

CUnitCol::~CUnitCol(void)
{
	Release();
}

void CUnitCol::Update(void)
{
	
	
}

ENGINE::CCollision* CUnitCol::Clone(void)
{
	++(*m_pwRefCnt);

	return new CUnitCol(*this);
}

CUnitCol* CUnitCol::Create(void)
{
	return new CUnitCol;
}

void CUnitCol::Release(void)
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

void CUnitCol::SetColInfo(D3DXVECTOR3* pPos, D3DXVECTOR3* pOrcPos)
{
	// 여기서 리스트 돌려주면서 충돌체크 

	float fX = pPos->x - pOrcPos->x;
	float fY = pPos->y - pOrcPos->y;
	float fZ = pPos->z - pOrcPos->z;

	float fDistance = sqrtf(fX * fX + fY * fY + fZ * fZ);

	if(fDistance < 11.f)
	{
		//m_bAtt = true;
		
		D3DXVECTOR3	vDir = (*pOrcPos) - (*pPos);

		(*pOrcPos) += vDir * 0.5f;
	
		//SetMinHeart();
	}
	else
	{
		return;
	}
}

void CUnitCol::SetMinHeart(void)
{

	list<ENGINE::CGameObject*>::iterator iter = ENGINE::CLayerMgr::GetInstance()->GetObjList(L"Player Heart", ENGINE::LAYER_UI)->begin();
	list<ENGINE::CGameObject*>::iterator iter_end = ENGINE::CLayerMgr::GetInstance()->GetObjList(L"Player Heart", ENGINE::LAYER_UI)->end();

	//공격을 받았을때.
	for(iter; iter != iter_end; ++iter)
	{
		(*iter)->GetAlive() = false;
		break;
	}

	//list<ENGINE::CGameObject*>::iterator iter2 = ENGINE::CLayerMgr::GetInstance()->GetObjList(L"Player Heart2", ENGINE::LAYER_UI)->begin();
	//list<ENGINE::CGameObject*>::iterator iter_end2 = ENGINE::CLayerMgr::GetInstance()->GetObjList(L"Player Heart2", ENGINE::LAYER_UI)->end();

	////공격을 받았을때.
	//for(iter2; iter2 != iter_end2; ++iter2)
	//{
	//	(*iter2)->GetAlive() = false;
	//	break;
	//}

}


