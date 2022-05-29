/*!
 * \file Component.h
 * \date 2016/07/21 11:02
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

#ifndef Component_h__
#define Component_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class ENGINE_DLL CComponent 
{
protected:
	CComponent(void);
public:
	virtual ~CComponent(void) = 0;

public:
	virtual void Update(void) { };
};

END
#endif // Component_h__