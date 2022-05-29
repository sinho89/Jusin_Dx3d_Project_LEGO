/*!
 * \file SceneSelector.h
 * \date 2016/08/11 8:57
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
#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "Logo.h"
#include "Stage_1.h"
#include "Stage_2.h"
#include "Stage_3.h"

class CSceneSelector
{
private:
	ENGINE::SCENEID		m_eScene;

public:
	explicit CSceneSelector(ENGINE::SCENEID _eScene): m_eScene(_eScene){}

public:
	HRESULT operator()(ENGINE::CScene** ppScene, LPDIRECT3DDEVICE9 pDevice)
	{
		switch(m_eScene)
		{
		case ENGINE::SC_LOGO:
			*ppScene = CLogo::Create(pDevice);
			break;

		case ENGINE::SC_STAGE_1:
			*ppScene = CStage_1::Create(pDevice);
			break;

		case ENGINE::SC_STAGE_2:
			*ppScene = CStage_2::Create(pDevice);
			break;

		case ENGINE::SC_STAGE_3:
			*ppScene = CStage_3::Create(pDevice);
			break;
		}

		return S_OK;
	}
};

#endif // SceneSelector_h__