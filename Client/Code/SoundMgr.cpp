#include "SoundMgr.h"

IMPLEMENT_SINGLETON(ENGINE::CSoundMgr)

ENGINE::CSoundMgr::CSoundMgr(void)
{
	m_pSystem	= NULL;
	m_iVersion	= 0;

	m_pEffect = NULL;	
	m_pMainBGM = NULL;
	m_pSkill = NULL;	
	m_pMonster = NULL;

	Initialize();
}

ENGINE::CSoundMgr::~CSoundMgr(void)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter = m_mapSound.begin();
	map<TCHAR*, FMOD_SOUND*>::iterator iter_end = m_mapSound.end();

	for( ; iter != iter_end; ++iter)
	{
		m_Result = FMOD_Sound_Release(iter->second);
		ErrorCheck(m_Result);

		delete [] (iter->first);

		iter = m_mapSound.erase(iter);
		iter_end = m_mapSound.end();

		if(iter == iter_end)
			break;
	}

	m_Result = FMOD_System_Close(m_pSystem);
	ErrorCheck(m_Result);

	m_Result = FMOD_System_Release(m_pSystem);
	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::Initialize(void)
{
	m_Result = FMOD_System_Create(&m_pSystem);
	ErrorCheck(m_Result);

	m_Result = FMOD_System_GetVersion(m_pSystem, &m_iVersion);
	ErrorCheck(m_Result);

	m_Result = FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);
	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::ErrorCheck(FMOD_RESULT _result)
{
	if(_result != FMOD_OK)
	{
		cout << "에러!!" << endl;
		system("pause");
		return;
	}
}



void ENGINE::CSoundMgr::LoadSoundFile(void)
{
	_finddata_t  fd;

	long handle;

	int iResult = 1;

	handle = _findfirst("../../SoundFile/*.*", &fd);

	if(handle == -1)
		return;

	while(iResult != -1)
	{
		TCHAR szName[256];

		ZeroMemory(szName, sizeof(szName));		//메크로함수 : memset

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
			fd.name, strlen(fd.name), szName, 256);
		//멀티바이트를 유니코드로 변경해주는 함수.

		TCHAR* pName = new TCHAR[256];
		ZeroMemory(pName, sizeof(TCHAR) * 256);
		lstrcpy(pName, szName);		//유니코드 문자열 복사.

		char szFullName[256];

		strcpy_s(szFullName, "../../SoundFile/");
		strcat_s(szFullName, fd.name);

		FMOD_SOUND* pSound;

		m_Result = FMOD_System_CreateSound(m_pSystem, szFullName
			,FMOD_HARDWARE, 0, &pSound);

		if(m_Result ==  FMOD_OK)
		{
			//사운드파일 정상적인 완성!
			m_mapSound.insert( make_pair(pName, pSound) );
		}
		else
		{
			delete [] pName;
		}

		iResult = _findnext(handle, &fd);

	}

	_findclose(handle);

	m_Result = FMOD_System_Update(m_pSystem);
	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::PlaySound(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}
void ENGINE::CSoundMgr::PlayBGMSound(TCHAR* pSoundKey)
{
	//주석
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	FMOD_Sound_SetMode(iter->second, FMOD_LOOP_NORMAL);

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pMainBGM);

	ErrorCheck(m_Result);
}
void ENGINE::CSoundMgr::SoundAllStop(void)
{
	if(m_pMainBGM)
	{
		m_Result = FMOD_Channel_Stop(m_pMainBGM);
		ErrorCheck(m_Result);
	}

	if(m_pEffect)
	{
		m_Result = FMOD_Channel_Stop(m_pEffect);
		ErrorCheck(m_Result);
	}

}

void ENGINE::CSoundMgr::PlayerMetheoSound(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::Zarya(TCHAR* pSoundKey)
{	
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);

}

void ENGINE::CSoundMgr::Energy(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);

}

void ENGINE::CSoundMgr::GengiSound(TCHAR* pSoundKey)
{

	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::EarthBoom(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::PlayerAtt(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::Hit(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::MonDie(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::IsildurSound(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}	

void ENGINE::CSoundMgr::HitSound(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::BossSound(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::Hit2(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::PlayerAtt2(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::MonsterAtt(TCHAR* pSoundkey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundkey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::Crowing(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}

void ENGINE::CSoundMgr::BossAtt(TCHAR* pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), CStringCmp(pSoundKey));

	if(iter == m_mapSound.end())
		return;

	m_Result = FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_REUSE, 
		iter->second, 0, &m_pEffect);

	ErrorCheck(m_Result);
}



