#include "Management.h"
#include "Renderer.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(ENGINE::CManagement)

ENGINE::CManagement::CManagement(void)
: m_pRenderer(NULL)
, m_pScene(NULL)
{

}

ENGINE::CManagement::~CManagement(void)
{
	Release();
}

HRESULT ENGINE::CManagement::InitManagement(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;

	m_pRenderer = CRenderer::Create(pDevice);
	NULL_CHECK_RETURN_MSG(m_pRenderer, E_FAIL, L"Renderer Create Failed");

	return S_OK;
}

void ENGINE::CManagement::Update(void)
{
	if(m_pScene != NULL)
		m_pScene->Update();
	/*if(m_pRenderer != NULL)
		m_pRenderer->Update();	*/
}

void ENGINE::CManagement::Render(float fTime)
{
	if(m_pRenderer != NULL)
		m_pRenderer->Render(fTime);	
}

void ENGINE::CManagement::Release(void)
{
	ENGINE::Safe_Delete(m_pScene);
	ENGINE::Safe_Delete(m_pRenderer);
}
