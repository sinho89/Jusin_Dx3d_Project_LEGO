#pragma once

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