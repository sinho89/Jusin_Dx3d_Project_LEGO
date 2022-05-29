#include "TimeMgr.h"

IMPLEMENT_SINGLETON(ENGINE::CTimeMgr)

ENGINE::CTimeMgr::CTimeMgr(void)
{

}

ENGINE::CTimeMgr::~CTimeMgr(void)
{

}

float ENGINE::CTimeMgr::GetTime(void)
{
	return m_fTime;
}

void ENGINE::CTimeMgr::InitTime(void)
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);
}

void ENGINE::CTimeMgr::SetTime(void)
{
	QueryPerformanceCounter(&m_FrameTime);

	if(m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);

		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;

	m_FixTime = m_FrameTime;
}