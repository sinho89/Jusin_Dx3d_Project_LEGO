/*!
 * \file Observer.h
 * \date 2016/07/28 10:13
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

#ifndef Observer_h__
#define Observer_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class ENGINE_DLL CObserver
{
protected:
	CObserver(void);
public:
	virtual ~CObserver(void);

public:
	virtual void Update(int	iMessage)		PURE;
};

END
#endif // Observer_h__