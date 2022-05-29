/*!
 * \file Engine_fuction.h
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

#ifndef Engine_fuction_h__
#define Engine_fuction_h__

namespace ENGINE
{
	template<typename T>
	void Safe_Delete(T& Temp)
	{
		if(NULL != Temp)
		{
			delete Temp;
			Temp = NULL;
		}
	}

	template <typename T> 
	void Safe_Delete_Array (T& pointer)
	{
		if(NULL != pointer)
		{
			delete [] pointer;
			pointer = NULL;
		}
	}


	template <typename T> 
	DWORD Safe_Release (T& pointer)
	{
		DWORD		dwRefCnt = 0;
		if(NULL != pointer)
		{
			dwRefCnt = pointer->Release();
			if(dwRefCnt == 0)
				pointer = NULL;
		}

		return dwRefCnt;
	}


	template <typename T> 
	void Safe_Single_Destory(T& pointer)
	{
		if(NULL != pointer)
		{
			pointer->DestroyInstance();
			pointer = NULL;
		}
	}
}

#endif // Engine_fuction_h__