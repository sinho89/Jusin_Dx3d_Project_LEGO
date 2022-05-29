#include "Subject.h"
#include "Observer.h"

ENGINE::CSubject::CSubject(void)
{

}

ENGINE::CSubject::~CSubject(void)
{
	Release();
}

void ENGINE::CSubject::Subscribe(CObserver* pObserver)
{
	NULL_CHECK(pObserver);

	m_ObserverList.push_back(pObserver);
}

void ENGINE::CSubject::UnSubscribe(CObserver* pObserver)
{
	OBSERVERLIST::iterator		iter		= m_ObserverList.begin();
	OBSERVERLIST::iterator		iter_end	= m_ObserverList.end();

	for( ; iter != iter_end; )
	{
		if((*iter) == pObserver)
		{
			m_ObserverList.erase(iter);
			return;
		}
		else
			++iter;
	}
}

void ENGINE::CSubject::Notify(int iMessage)
{	
	OBSERVERLIST::iterator		iter		= m_ObserverList.begin();
	OBSERVERLIST::iterator		iter_end	= m_ObserverList.end();

	for( ; iter != iter_end; ++iter)
	{
		(*iter)->Update(iMessage);
	}
}

void ENGINE::CSubject::Release(void)
{
	m_ObserverList.clear();
}

