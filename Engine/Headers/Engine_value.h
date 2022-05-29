/*!
 * \file Engine_value.h
 * \date 2016/08/11 13:49
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
#ifndef Engine_value_h__
#define Engine_value_h__

namespace ENGINE
{
	extern HWND	g_hWnd;

	const int WINCX	= 1400;
	const int WINCY = 900;

	const D3DXVECTOR3	g_vLook = D3DXVECTOR3(0.f, 0.f, 1.f);

	const WORD	VTXCNTX = 129;
	const WORD	VTXCNTZ = 129;
	const WORD	VTXITV	= 1;
}

#endif // Engine_value_h__