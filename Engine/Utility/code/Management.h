/*!
 * \file Management.h
 * \date 2016/08/10 16:33
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Management_h__
#define Management_h__

#include "Engine_Include.h"
#include "Renderer.h"

BEGIN(ENGINE)

class CScene;
class ENGINE_DLL CManagement
{
	DECLARE_SINGLETON(CManagement)

public:
	HRESULT		InitManagement(LPDIRECT3DDEVICE9 pDevice);
	void		Update(void);
	void		Render(float fTime);

private:
	void		Release(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	CScene*					m_pScene;
	CRenderer*				m_pRenderer;

private:
	CManagement(void);
	~CManagement(void);

public:
	template<typename T>
	HRESULT	SceneChange(T& Functor);
};

template<typename T>
HRESULT ENGINE::CManagement::SceneChange(T& Functor)
{
	if(m_pScene != NULL)
		ENGINE::Safe_Delete(m_pScene);

	FAILED_CHECK(Functor(&m_pScene, m_pDevice));

	m_pRenderer->SetScene(m_pScene);

	return S_OK;
}

END

#endif // Management_h__