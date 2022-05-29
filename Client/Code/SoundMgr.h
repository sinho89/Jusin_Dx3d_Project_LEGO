
#ifndef SoundMgr_h__
#define SoundMgr_h__

#include "Engine_Include.h"


#include "fmod.h"
#include "fmod.hpp"
#include "fmod_dsp.h"
#include "fmod_errors.h"

#pragma comment (lib, "fmodex_vc.lib")


BEGIN(ENGINE)

class ENGINE_DLL CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)

	//////////fmod 인터페이스
private:
	FMOD_SYSTEM*		m_pSystem;		//fmod 시스템 포인터
	//FMOD_CHANNEL*		m_pSound[ST_END];		//배열로해도...
	FMOD_CHANNEL*		m_pEffect;		//효과음 
	FMOD_CHANNEL*		m_pMainBGM;		//배경음 
	FMOD_CHANNEL*		m_pSkill;		//스킬
	FMOD_CHANNEL*		m_pMonster;		//몬스터 소리 

	FMOD_RESULT			m_Result;		//fmod실행중에 에러체크

	unsigned int		m_iVersion;

	map<TCHAR*, FMOD_SOUND*>		m_mapSound;

	//기능 함수를 구현하자.
public:
	void Initialize(void);
	void LoadSoundFile(void);
	void PlaySound(TCHAR* pSoundKey);
	void PlayBGMSound(TCHAR* pSoundKey);


	void PlayerMetheoSound(TCHAR* pSoundKey);
	void Zarya(TCHAR* pSoundKey);
	void Energy(TCHAR* pSoundKey);
	void GengiSound(TCHAR* pSoundKey);
	void EarthBoom(TCHAR* pSoundKey);


	void PlayerAtt(TCHAR* pSoundKey);
	void PlayerAtt2(TCHAR* pSoundKey);
	void MonsterAtt(TCHAR* pSoundkey);
	void Hit(TCHAR* pSoundKey);
	void Hit2(TCHAR* pSoundKey);
	void MonDie(TCHAR* pSoundKey);
	void Crowing(TCHAR* pSoundKey);

	void BossAtt(TCHAR* pSoundKey);
	

	void SoundAllStop(void);


	void IsildurSound(TCHAR* pSoundKey);
	void HitSound(TCHAR* pSoundKey);
	void BossSound(TCHAR* pSoundKey);




public:
	map<TCHAR*, FMOD_SOUND*>* GetSoundMap(void)
	{
		return &m_mapSound;
	}

private:
	void ErrorCheck(FMOD_RESULT _result);


private:
	CSoundMgr(void);
public:
	~CSoundMgr(void);
};

END
#endif // SoundMgr_h__
