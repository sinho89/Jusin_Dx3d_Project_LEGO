#include "Collision.h"

ENGINE::CCollision::CCollision(void)
: m_pwRefCnt(new WORD(0))
{

}

ENGINE::CCollision::~CCollision(void)
{
	//Release();

	/*if((*m_pwRefCnt) == 0)
		ENGINE::Safe_Delete(m_pwRefCnt);
	else
		--(*m_pwRefCnt);*/
}

void ENGINE::CCollision::Release(void)
{
	if((*m_pwRefCnt) == 0)
		ENGINE::Safe_Delete(m_pwRefCnt);
	else
		--(*m_pwRefCnt);
}

