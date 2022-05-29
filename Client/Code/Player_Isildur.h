/*!
 * \file Player_Isildur.h
 * \date 2016/08/20 13:40
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
#ifndef Player_Isildur_h__
#define Player_Isildur_h__

#include "Unit.h"

class CCollisionMgr;
class CTerrainCol;
class CUnitCol;
class CHeart;
class CPlayer_Isildur : public CUnit
{
private:
	explicit CPlayer_Isildur(LPDIRECT3DDEVICE9 pDevice);
public:
	~CPlayer_Isildur(void);

private:
	virtual HRESULT Initialize(void);
	virtual HRESULT	AddComponent(void);
	void			Release(void);
	
private:
	void			CreateBodyPart(ENGINE::SAVECUBE*);
	void			KeyDown(void);

public:
	virtual void	 Update(void);
	virtual void	 Render(void);

private:
	void			Ani_Idle(void);	
	void			Ani_Attack(void);

	void			Ani_LegWalk(void);
	void			Ani_WalkHandShake(void);

	void			Ani_Sword(void);
	void			Ani_Reflection(void);
	void			Ani_SkillUse(void);

	void			Skill_1_Explosion(void);
	void			Skill_1_Effect(void);
	void			Skill_2_Slash(void);

public:
	static CPlayer_Isildur*	Create(LPDIRECT3DDEVICE9 pDevice);
	void					SetBodyPos(D3DXVECTOR3& pos);
	void					BodyAnimation(void);
	void					Jump(void);
	void					BoomCrush(void);
	void					AttMode(void);
	void					SetState(ENGINE::UNITSTATEID _eType){ m_eUnitStateType = _eType;}
	void					DeletePlayer(void);
	void					AttBoss(void);

private:	
	ENGINE::CLayerMgr*			m_pLayerMgr;
	CCollisionMgr*				m_pCollisionMgr;
	CTerrainCol*				m_pTerrainCol;
	const ENGINE::VTXTEX*		m_pTerrainVertex;
	CUnitCol*					m_pUnitCol;
	CClientObj*					m_pSword;

	bool						m_bPaticleMake;
	D3DXVECTOR3					m_bSkill_1_Pos;
	ENGINE::CGameObject*		m_pParticle[8];

	float						m_fSkill2_Size;
	D3DXVECTOR3					m_bSkill_2_Pos;
	bool						m_bSkill_2_Fase1;
	bool						m_bSkill_2_Fase2;
	bool						m_bSkill_2_Fase3;
	bool						m_bSkill_2_Use;
	ENGINE::CGameObject*		m_pParticle2;
	
	CClientObj*					m_pDyingBodyPart[ENGINE::BP_END];

	float						m_fAttTime;
	float						m_Gravity;

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

	bool						m_bReflectAni;
	float						m_fReflectAngle;

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

	bool						m_bSkill_1_Use;
	float						m_fSkill_1_Time;
	float						m_fSkill_1_Length;

	float						m_fSkill_2_Time;
};

#endif // Player_Isildur_h__