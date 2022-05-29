#include "Renderer.h"
#include "Scene.h"
#include "CameraMgr.h"
#include "DivisionMgr.h"
#include "DivisionCamera.h"
#include "LayerMgr.h"
#include "Transform.h"

ENGINE::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_pScene(NULL)
, m_pD3DXFont(NULL)
, m_fTime(0.f)
, m_wFrameCnt(0)
{
	ZeroMemory(m_szFPS, sizeof(TCHAR) * 128);

	m_pCamMn	= NULL;

	m_iTxW		= 700;
	m_iTxH		= 700;

	m_pCurBck	= NULL;
	m_pCurDpt	= NULL;

	memset(m_RndSf, 0, sizeof(m_RndSf));

	m_pCam = CCameraMgr::GetInstance();
}

ENGINE::CRenderer::~CRenderer(void)
{

}

ENGINE::CRenderer* ENGINE::CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderer*		pRenderer = new CRenderer(pDevice);

	if(FAILED(pRenderer->InitRenderer()))
		ENGINE::Safe_Delete(pRenderer);

	return pRenderer;		
}

HRESULT ENGINE::CRenderer::InitRenderer(void)
{
	D3DXFONT_DESC	hFont;
	ZeroMemory(&hFont, sizeof(D3DXFONT_DESC));

	hFont.Weight	=	10;
	hFont.Height	=	15;
	hFont.Weight	=	FW_NORMAL;
	lstrcpy(hFont.FaceName, L"±Ã¼­");
	hFont.CharSet = HANGEUL_CHARSET;

	ENGINE::CDivisionMgr::GetInstance()->Create(m_pDevice);

	ENGINE::CDivisionMgr::GetInstance()->m_pCam1P->SetLook(D3DXVECTOR3(0.f, 0.f, 100.f));
	ENGINE::CDivisionMgr::GetInstance()->m_pCam2P->SetLook(D3DXVECTOR3(0.f, 0.f, 0.f));

	FAILED_CHECK(D3DXCreateFontIndirect(m_pDevice, &hFont, &m_pD3DXFont));	
	
	for(int i=0; i<2; ++i)
	{
		m_pDevice->CreateTexture(m_iTxW, m_iTxW + 200, 0
			, D3DUSAGE_RENDERTARGET
			, D3DFMT_X8R8G8B8
			, D3DPOOL_DEFAULT
			, &m_RndSf[i].pTx, NULL);

		m_RndSf[i].pTx->GetSurfaceLevel(0, &m_RndSf[i].pSf);
	}

	D3DXCreateSprite(m_pDevice, &m_pd3dSprite);

	m_pDevice->GetRenderTarget(0,&m_pCurBck);
	m_pDevice->GetDepthStencilSurface(&m_pCurDpt);

	return S_OK;
}

void ENGINE::CRenderer::Update()
{

}
void ENGINE::CRenderer::Render(float fTime)
{
	RECT	rc = {10, 10, 1400, 900};

	++m_wFrameCnt;
	m_fTime += fTime;

	if(1.f < m_fTime)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_wFrameCnt);
		m_fTime = 0.f;
		m_wFrameCnt = 0;
	}	

	m_pDevice->Clear(0, NULL, 
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);

	m_pDevice->BeginScene();
	
	if(CLayerMgr::GetInstance()->GetDivisionType() == CLayerMgr::DIV_MULTI )
	{			
		if(m_pCam->GetDisType() != m_pCam->DIS_STAGE1_BOSS 
			|| m_pCam->GetDisType() != m_pCam->DIR_STAGE1_BOSS_START)
		{
			m_pDevice->SetRenderTarget(0,m_RndSf[0].pSf);
			m_pDevice->Clear( 0L, NULL, D3DCLEAR_TARGET| D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );	

			D3DXMATRIX	matView;

			m_pCam->Player1ViewMatrix(&matView);
			m_pDevice->SetTransform(D3DTS_VIEW, &matView);

			m_pScene->Render();

			m_pDevice->SetRenderTarget(0,m_RndSf[1].pSf);
			m_pDevice->Clear( 0L, NULL, D3DCLEAR_TARGET| D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );	

			m_pCam->Player2ViewMatrix(&matView);
			m_pDevice->SetTransform(D3DTS_VIEW, &matView);

			m_pScene->Render();

			m_pDevice->SetRenderTarget(0,m_pCurBck);		//·»´õ Å¸°ÙÀ» ¿ø·¡´ë·Î
			m_pDevice->SetDepthStencilSurface(m_pCurDpt);


			RECT		rcDiv = {0,0,m_iTxH, m_iTxW + 200};
			D3DXVECTOR3	vcPos;

			for(int i=0; i<2; ++i)
			{
				vcPos	= D3DXVECTOR3(i,0, 0) * m_iTxH;

				m_pd3dSprite->Begin(D3DXSPRITE_ALPHABLEND);
				m_pd3dSprite->Draw(m_RndSf[i].pTx, &rcDiv, NULL, &vcPos, D3DXCOLOR(1,1,1,1));
				m_pd3dSprite->End();
			}

			m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
			m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE , FALSE);
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
			m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		}
		else
			m_pScene->Render();
	}
	else if(CLayerMgr::GetInstance()->GetDivisionType() == CLayerMgr::DIV_SINGLE)
		m_pScene->Render();

	else if(CLayerMgr::GetInstance()->GetDivisionType() == CLayerMgr::DIV_LOGO)
		m_pScene->Render();
	

	m_pD3DXFont->DrawText(NULL, m_szFPS, lstrlen(m_szFPS),
		&rc, DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 255, 0));

	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

void ENGINE::CRenderer::SetScene(CScene* pScene)
{
	m_pScene = pScene;
}

ENGINE::DIVISION* ENGINE::CRenderer::GetRenderInfo()
{
	return m_RndSf;
}

