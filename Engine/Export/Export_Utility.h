/*!
 * \file Export_Utility.h
 * \date 2016/07/13 11:20
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

#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "InfoSubject.h"
#include "LayerMgr.h"
#include "CameraMgr.h"
#include "DivisionMgr.h"

BEGIN(ENGINE)

inline CManagement*	Get_Management(void);
inline CInfoSubject* Get_InfoSubject(void);
inline CLayerMgr*	Get_LayerMgr(void);
inline CCameraMgr*	Get_CameraMgr(void);
inline CDivisionMgr*	Get_DivisionMgr(void);

#include "Export_Utility.inl"

END
#endif // Export_Utility_h__