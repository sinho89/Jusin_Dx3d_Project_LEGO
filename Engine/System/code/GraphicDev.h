/*!
 * \file GraphicDev.h
 * \date 2016/07/12 11:26
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

#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class ENGINE_DLL CGraphicDev
{
	DECLARE_SINGLETON(CGraphicDev)

private:
	explicit CGraphicDev(void);
	~CGraphicDev(void);

public:
	enum WINMODE	{ MODE_FULL, MODE_WIN };

public: // getter
	LPDIRECT3DDEVICE9 GetDevice(void);

public:
	HRESULT InitGraphicDev(WINMODE Mode, HWND hWnd, const WORD& wSizeX, const WORD& wSizeY);

private:
	LPDIRECT3D9					m_pSDK;
	LPDIRECT3DDEVICE9			m_pDevice; // 장치를 대표하는 객체다.

private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, 
		WINMODE Mode, HWND hWnd, 
		const WORD& wSizeX, const WORD& wSizeY);

	void Release(void);
};	

END

#endif // GraphicDev_h__