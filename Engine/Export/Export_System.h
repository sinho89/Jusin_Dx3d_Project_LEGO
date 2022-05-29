/*!
 * \file Export_System.h
 * \date 2016/07/13 11:19
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

#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDev.h"
#include "TimeMgr.h"

BEGIN(ENGINE)

inline CGraphicDev* Get_GraphicDev(void);
inline CTimeMgr*	Get_Time(void);

#include "Export_System.inl"

END
#endif // Export_System_h__