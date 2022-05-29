/*!
 * \file Engine_functor.h
 * \date 2016/07/12 11:22
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

#ifndef Engine_functor_h__
#define Engine_functor_h__

namespace ENGINE
{
	class CDeleteObj
	{
	public:
		explicit CDeleteObj(){}
		~CDeleteObj(){}

	public:
		template<typename T>
		void operator()(T& Temp)
		{
			if(NULL != Temp)
			{
				delete Temp;
				Temp = NULL;
			}
		}
	};

	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void){}
		~CDeleteMap(){}

	public:
		template<typename T>
		void operator()(T& pair)
		{
			if(NULL != pair.second)
			{
				delete pair.second;
				pair.second = NULL;
			}
		}
	};

	class CTagFinder
	{
	private:
		const TCHAR*			m_pTag;

	public:
		explicit CTagFinder(const TCHAR* pTag) : m_pTag(pTag) {}
		~CTagFinder(){}

	public:
		template<typename T>
		bool operator()(T& pair)
		{
			int iResult = lstrcmp(m_pTag, pair.first);

			if(0 == iResult)
				return true;

			return false;
		}
	};

	class CStringCmp
	{
	private:
		const TCHAR* m_pString;

	public:
		explicit CStringCmp(const TCHAR* pKey)
			:m_pString(pKey){}
		~CStringCmp(){}

	public:
		template<typename T>
		bool operator () (T data)
		{
			return !lstrcmp(data.first, m_pString);
		}
	};


}

#endif // Engine_functor_h__