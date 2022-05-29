/*!
 * \file MathMgr.h
 * \date 2016/07/25 12:04
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

#ifndef MathMgr_h__
#define MathMgr_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class ENGINE_DLL CMathMgr
{
public:
	static void MyRotaionX(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle);
	static void MyRotaionY(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle);
	static void MyRotaionZ(D3DXVECTOR3* pOut, D3DXVECTOR3* pIn, float fAngle);

private:
	CMathMgr();
	~CMathMgr();
};

END
#endif // MathMgr_h__