/*!
 * \file Renderer.h
 * \date 2016/08/10 17:06
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
#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CScene;
class CDivisionMgr;
class CCameraMgr;
class CTransform;
class ENGINE_DLL CRenderer
{

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	CScene*					m_pScene;
	ID3DXFont*				m_pD3DXFont;
	float					m_fTime;
	WORD					m_wFrameCnt;
	TCHAR					m_szFPS[128];

	////// 화면 동적 분할 
	CDivisionMgr*			m_pCamMn;
	CCameraMgr*				m_pCam;
	LPDIRECT3DSURFACE9		m_pCurBck;												// Back Buffer
	LPDIRECT3DSURFACE9		m_pCurDpt;	
	LPD3DXSPRITE			m_pd3dSprite;	

	DIVISION				m_RndSf[2];
	INT						m_iTxW;	
	INT						m_iTxH;

public:
	static	CRenderer*		Create(LPDIRECT3DDEVICE9 pDevice);
	HRESULT	InitRenderer(void);
	void	Update();
	void	Render(float fTime);
	DIVISION*	GetRenderInfo();
public:
	void	SetScene(CScene* pScene);

private:
	CRenderer(LPDIRECT3DDEVICE9 pDevice);

public:
	~CRenderer(void);
};
END

#endif // Renderer_h__