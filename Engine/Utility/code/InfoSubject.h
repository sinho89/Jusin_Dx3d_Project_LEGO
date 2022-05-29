/*!
 * \file InfoSubject.h
 * \date 2016/07/28 10:32
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

#ifndef InfoSubject_h__
#define InfoSubject_h__

#include "Subject.h"
BEGIN(ENGINE)

typedef	list<void*>				DATALIST;
typedef	unordered_map<int, DATALIST>		MAPDATALIST;

class ENGINE_DLL CInfoSubject
	: public CSubject
{
public:
	DECLARE_SINGLETON(CInfoSubject)

private:
	CInfoSubject(void);
	virtual ~CInfoSubject(void);

public:
	DATALIST*	GetDataList(int iMessage);

public:
	void		AddData(int iMessage, void* pData);
	void		RemoveData(int iMessage, void* pData);

private:
	void		Release(void);

private:
	MAPDATALIST		m_mapDataList;
};

END
#endif // InfoSubject_h__