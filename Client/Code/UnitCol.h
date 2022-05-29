#ifndef UnitCol_h__
#define UnitCol_h__


#include "Collision.h"

namespace ENGINE
{
	class CTimeMgr;
	class CSoundMgr;
}
class CHeart;
class CUnitCol : public ENGINE::CCollision
{
private:
	CUnitCol(void);

public:
	virtual ~CUnitCol(void);
private:
	D3DXVECTOR3* m_pPos;
	ENGINE::CTimeMgr* m_pTime;
	float		 m_fTime;
	//ENGINE::CSoundMgr* m_pSoundMgr;
	bool			m_bAtt;

public:
	virtual void			  Update(void);
	virtual ENGINE::CCollision* Clone(void);

public:
	static CUnitCol*		  Create(void);
	void					  Release(void);
	void					  SetColInfo(D3DXVECTOR3* pPos, D3DXVECTOR3* pOrcPos);
	void					  SetMinHeart(void);						

};


#endif // UnitCol_h__
