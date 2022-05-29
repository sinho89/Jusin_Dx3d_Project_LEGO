/*!
 * \file Player_Elond.h
 * \date 2016/08/20 13:42
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
#ifndef Player_Elond_h__
#define Player_Elond_h__

#include "Unit.h"

class CCollisionMgr;
class CTerrainCol;
class CMetheo;
class CPlayer2_Skill2;
class CUnitCol;
class CPlayer_Elond : public CUnit
{
private:
	explicit CPlayer_Elond(LPDIRECT3DDEVICE9 pDevice);
public:
	~CPlayer_Elond(void);

private:
	virtual HRESULT Initialize(void);
	HRESULT			AddComponent(void);
	void			Release(void);

private:
	void			CreateBodyPart(ENGINE::SAVECUBE*);
	void			KeyDown(void);

public:
	virtual void	 Update(void);
	virtual void	Render(void);

private:
	void			Ani_Idle(void);	
	void			Ani_Attack(void);

	void			Ani_LegWalk(void);
	void			Ani_WalkHandShake(void);

	void			Ani_Sword(void);
	void			Ani_Reflection(void);
	void			Ani_SkillUse(void);

	void			Skill_1_Metheo(void);
	void			Skill_2_Adonbbagadon(void);

public:
	static CPlayer_Elond*	Create(LPDIRECT3DDEVICE9 pDevice);
	void					SetBodyPos(D3DXVECTOR3& pos);
	void					BodyAnimation(void);
	void					Jump(void);
	void					AttMode(void);
	void					AttBoss(void);

public:
	bool&					GetReflectSkill(void){ return m_bReflectAni; }

private:
	ENGINE::CLayerMgr*			m_pLayerMgr;
	CCollisionMgr*				m_pCollisionMgr;
	CTerrainCol*				m_pTerrainCol;
	const ENGINE::VTXTEX*		m_pTerrainVertex;
	CMetheo*					m_pMetheo;
	CPlayer2_Skill2*			m_pPlayerSkill2;
	CClientObj*					m_pSword;
	float						m_fAttTime;
	CUnitCol*					m_pUnitCol;


	list<ENGINE::CGameObject*>*	m_Heartlist;

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

	bool						m_bMeteo;
	bool						m_bBuff;

	bool						m_bMetheo_Use;
	int							m_iCountMetheo;
	float						m_fMetheoTime;

	D3DXVECTOR3					m_vecParticlePos;
	ENGINE::CGameObject*		m_pAdonbbagadon_IN;
	ENGINE::CGameObject*		m_pAdonbbagadon_OUT;
	bool						m_bUseAdonbbagadon;
	bool						m_bSkill_2_fase_1;
	bool						m_bSkill_2_fase_2;
	bool						m_bSkill_2_fase_3;
	float						m_fSkill_2_Time;
	float						m_fSkill_2_Size;
};

#endif // Player_Elond_h__