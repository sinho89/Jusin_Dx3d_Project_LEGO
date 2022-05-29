/*!
 * \file Monster_Orc.h
 * \date 2016/08/23 21:09
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
#ifndef Monster_Orc_h__
#define Monster_Orc_h__

#include "Unit.h"

class CCollisionMgr;
class CTerrainCol;
class CUnitCol;
class CMonster_Orc : public CUnit
{
public:
	enum ORCTYPE{ ORC_DUMMY, ORC_MOB };

private:
	explicit CMonster_Orc(LPDIRECT3DDEVICE9 pDevice, ORCTYPE OrcType);
public:
	~CMonster_Orc(void);

private:
	virtual HRESULT 		Initialize(void);
	virtual HRESULT			AddComponent(void);
	void					Release(void);

private:
	void					CreateBodyPart(ENGINE::SAVECUBE*);

public:
	virtual void	 		Update(void);
	virtual void	 		Render(void);

private:
	void					Ani_Idle(void);	
	void					Ani_Attack(void);

	void					Ani_LegWalk(void);
	void					Ani_WalkHandShake(void);
	
	void					Ani_Sword(void);

public:
	static CMonster_Orc*	Create(LPDIRECT3DDEVICE9 pDevice, ORCTYPE OrcType);
	void					SetBodyPos(D3DXVECTOR3& pos);
	void					BodyAnimation(void);
	void					BoomCrush(void);
	void					AI_01(void);
	int&					GetAttHp(void);
	bool&					GetCrushBoom(void);
	void					IsildurAtt(void);
	void					ElondAtt(void);

private:	
	ENGINE::CLayerMgr*			m_pLayerMgr;
	CCollisionMgr*				m_pCollisionMgr;
	CTerrainCol*				m_pTerrainCol;
	const ENGINE::VTXTEX*		m_pTerrainVertex;
	CUnitCol*					m_pUnitCol;
	CClientObj*					m_pSword;

	CClientObj*					m_pDyingBodyPart[ENGINE::BP_END];
	D3DXMATRIX					m_matParent;
	D3DXVECTOR3					m_vecDir;

	ORCTYPE						m_eOrcType;

	bool						MonDestination;
	float						m_Gravity;
	bool						m_bCollision;

	float						m_fAttTime;
	int							m_iHp;
	bool						m_bSwordIdle;
	bool						m_bArmIdle;
	bool						m_bHeadIdle;
	bool						m_bBodyIdle;
	bool						m_bLegIdle;

	bool						m_bMove_LegWalk;
	bool						m_bMove_HandShake;

	bool						m_bLeg_MoveUp;

	bool						m_bSwordAttack;
	bool						m_bAttackBtn;
	float						m_fAttackArmAngle;

	bool						m_bCrush;
	float						m_fCrushTime;

	bool						m_bSkillUse;
	float						m_fSkillTime;

	bool						m_bSwordMove;
	float						m_fSwordMove;

	float						m_fLeftLeg_AngleX;
	float						m_fRightLeg_AngleX;

	float						m_fLeftArm_AngleX;
	float						m_fRightArm_AngleX;
	float						m_fTime;
	int							m_iAiCount;

	bool						m_bIsildurAtt;
	bool						m_bElondAtt;
};

#endif // Monster_Orc_h__