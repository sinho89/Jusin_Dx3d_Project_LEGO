#include "Resources.h"

ENGINE::CResources::CResources(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_pwRefCnt(new WORD(0))
{

}

ENGINE::CResources::~CResources(void)
{

}

void ENGINE::CResources::Release(void)
{
	ENGINE::Safe_Delete(m_pwRefCnt);
}

