#include "DivisionMgr.h"
#include "DivisionCamera.h"

IMPLEMENT_SINGLETON(ENGINE::CDivisionMgr)

ENGINE::CDivisionMgr::CDivisionMgr(void)
{
	m_pCam1P = NULL;
	m_pCam2P = NULL;
}

ENGINE::CDivisionMgr::~CDivisionMgr(void)
{

}
INT ENGINE::CDivisionMgr::Create(LPDIRECT3DDEVICE9 pDev)
{
	m_pCam1P = new CDivisionCamera;
	m_pCam2P = new CDivisionCamera;

	m_pCam1P->Create(pDev);
	m_pCam2P->Create(pDev);

	return 0;
}

INT ENGINE::CDivisionMgr::FrameMove()
{
	m_pCam1P->FrameMove();
	m_pCam1P->UpdateViewProj();

	D3DXVECTOR3	vcEye	= m_pCam1P->GetEye();
	D3DXVECTOR3	vcLook	= m_pCam1P->GetLook();
	D3DXVECTOR3	vcUp	= m_pCam1P->GetUP();

	D3DXVECTOR3	vcAxisX = m_pCam1P->GetAxisX();
	D3DXVECTOR3	vcAxisY = m_pCam1P->GetAxisY();
	D3DXVECTOR3	vcAxisZ = m_pCam1P->GetAxisZ();


	m_pCam1P->SetParamView(vcEye, vcEye - 1000.f * vcAxisX,	D3DXVECTOR3(0,100,0));
	m_pCam2P->SetParamView(vcEye, vcEye + 1000.f * vcAxisX,	D3DXVECTOR3(0,10,0));
	
	m_pCam1P->UpdateViewProj();
	m_pCam2P->UpdateViewProj();

	return 0;
}


