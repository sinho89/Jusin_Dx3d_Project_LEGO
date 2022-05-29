/*!
 * \file Boss_1.h
 * \date 2016/08/28 20:25
 *
 * \author earth
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef Boss_1_h__
#define Boss_1_h__

#include "Unit.h"

class CCollisionMgr;
class CTerrainCol;
class CUnitCol;
class CBoss_1 : public CUnit
{
private:
	explicit CBoss_1(LPDIRECT3DDEVICE9 pDevice);
public:
	~CBoss_1(void);

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
	void					Ani_SkillUse(void);

public:
	static CBoss_1*			Create(LPDIRECT3DDEVICE9 pDevice);
	void					BodyAnimation(void);
	void					BoomCrush(void);
	bool&					GetApear(void){ return m_bAppear; }

	void					AI_01(void);
	void					ElondAtt(void);
	void					IsildurAtt(void);
	void					Jump(void);
	int&					GetAttHp(void)
	{
		return m_iHp;
	}


private:	
	CCollisionMgr*				m_pCollisionMgr;
	CTerrainCol*				m_pTerrainCol;
	const ENGINE::VTXTEX*		m_pTerrainVertex;
	CUnitCol*					m_pUnitCol;
	CClientObj*					m_pSword;
	float						m_fAttTime;
	CClientObj*					m_pDyingBodyPart[ENGINE::BP_END];
	
	bool						m_ElondAtt;
	bool						m_IsildurAtt;

	float						m_fTime;
	int							m_iHp;
	float						m_Gravity;
	float						m_fBossJumpTime;

	D3DXMATRIX					m_matParent;
	D3DXVECTOR3					m_vecDir;

	bool						m_bCollision;
	bool						MonDestination;
	bool						m_bSwordIdle;
	bool						m_bArmIdle;
	bool						m_bHeadIdle;
	bool						m_bBodyIdle;
	bool						m_bLegIdle;
	bool						m_bJumpTiming;
	bool						m_bSpeedSkill;

	bool						m_bMove_LegWalk;
	bool						m_bMove_HandShake;
	bool						m_Att;
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

	bool						m_bAppear;//그려줄 타이밍
	bool						m_bAppearScene;//하강 타이밍
	bool						m_bFrontWalk;	//전진 워킹.
	float						m_fActionMove;
};

#endif // Boss_1_h__
