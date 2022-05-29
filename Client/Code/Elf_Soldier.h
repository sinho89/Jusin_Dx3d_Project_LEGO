/*!
 * \file Elf_Soldier.h
 * \date 2016/08/23 21:14
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
#ifndef Elf_Soldier_h__
#define Elf_Soldier_h__

#include "Unit.h"

class CCollisionMgr;
class CTerrainCol;
class CElf_Soldier : public CUnit
{
public:
	enum NPCTYPE{ NPC_SOLDIER, NPC_HEALER, NPC_END };

private:
	explicit CElf_Soldier(LPDIRECT3DDEVICE9 pDevice);
	explicit CElf_Soldier(LPDIRECT3DDEVICE9 pDevice, NPCTYPE _eNPCType);
public:
	~CElf_Soldier(void);

private:
	virtual HRESULT Initialize(void);
	virtual HRESULT	AddComponent(void);
	void			Release(void);

private:
	void			CreateBodyPart(ENGINE::SAVECUBE*);

public:
	virtual void	 Update(void);
	virtual void	 Render(void);

private:
	void			Ani_Idle(void);	
	void			Ani_Attack(void);
	void			Ani_Sword(void);
	void			Ani_SkillUse(void);

public:
	static CElf_Soldier*	Create(LPDIRECT3DDEVICE9 pDevice);
	static CElf_Soldier*	Create(LPDIRECT3DDEVICE9 pDevice, NPCTYPE _eNPCType);
	void					SetBodyPos(D3DXVECTOR3& pos);
	void					BodyAnimation(void);

private:	
	ENGINE::CLayerMgr*			m_pLayerMgr;
	CCollisionMgr*				m_pCollisionMgr;
	CTerrainCol*				m_pTerrainCol;
	const ENGINE::VTXTEX*		m_pTerrainVertex;
	CClientObj*					m_pSword;

	NPCTYPE						m_eNPCType;

	bool						m_bSwordIdle;
	bool						m_bArmIdle;
	bool						m_bHeadIdle;
	bool						m_bBodyIdle;
	bool						m_bLegIdle;

	bool						m_bSwordAttack;
	bool						m_bAttackBtn;
	float						m_fAttackArmAngle;
	float						m_fAttackTime;

	bool						m_bSkillUse;
	float						m_fSkillTime;
	float						m_fSkillUseTime;

	int							m_iEffectRand;
	bool						m_bEffect;
};

#endif // Elf_Soldier_h__