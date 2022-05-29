#include "InfoSubject.h"

IMPLEMENT_SINGLETON(ENGINE::CInfoSubject)

ENGINE::CInfoSubject::CInfoSubject(void)
{

}

ENGINE::CInfoSubject::~CInfoSubject(void)
{
	Release();
}

ENGINE::DATALIST* ENGINE::CInfoSubject::GetDataList(int iMessage)
{
	MAPDATALIST::iterator		iter = m_mapDataList.find(iMessage);

	if(iter == m_mapDataList.end())
		return NULL;

	return &iter->second;
}

void ENGINE::CInfoSubject::AddData(int iMessage, void* pData)
{
	if(pData)
	{
		MAPDATALIST::iterator	iter = m_mapDataList.find(iMessage);

		if(iter == m_mapDataList.end())
		{
			m_mapDataList[iMessage] = DATALIST();
		}

		m_mapDataList[iMessage].push_back(pData);
		Notify(iMessage);
	}
}

void ENGINE::CInfoSubject::RemoveData(int iMessage, void* pData)
{
	MAPDATALIST::iterator		mapiter = m_mapDataList.find(iMessage);

	if(mapiter != m_mapDataList.end())
	{
		DATALIST::iterator		iterList = mapiter->second.begin();
		DATALIST::iterator		iterEnd	 = mapiter->second.end();

		for( ;iterList != iterEnd; ++iterList)
		{
			if((*iterList) == pData)
			{
				mapiter->second.erase(iterList);
				return;
			}
		}
	}
}

void ENGINE::CInfoSubject::Release(void)
{
	for(MAPDATALIST::iterator	iter = m_mapDataList.begin();
		iter != m_mapDataList.end(); ++iter)
	{
		iter->second.clear();
	}
	m_mapDataList.clear();
}

