/*!
 * \file CameraMgr.h
 * \date 2016/08/20 16:50
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

#ifndef CameraMgr_h__
#define CameraMgr_h__

#include "Engine_Include.h"

BEGIN(ENGINE)

class CComponent;
class ENGINE_DLL CCameraMgr
{
	DECLARE_SINGLETON(CCameraMgr)
public:
	enum CameraType{ CAM_PLAY, CAM_DIR_1, CAM_DIR_2, CAM_DIR_3, CAM_END };
	enum DisplayType{ DIS_NORMAL, DIS_STAGE1_BOSS, DIS_STAGE2, DIS_STAGE2_BOSS, DIS_STAGE2_ONE, DIS_END };
	enum DirectType{ DIR_NONE, DIR_OPENING, DIR_STAGE1_BOSS_START, DIR_STAGE2_BOSS_START, DIR_END };
	enum OverlapProtectType{ OP_OPENING, OP_STAGE1, OP_STAGE2, OP_END};

private:
	LPDIRECT3DDEVICE9				m_pDevice;
	
	CameraType				m_eCamType; // 카메라 기본 타입
	DisplayType				m_eType; // 베이스 뷰 타입
	DirectType				m_eDirectType; // 연출 타입

	CAM_INFO				m_tInfo; // 카메라 정보 모음 구조체

	float					m_Gravity;
	CComponent*				m_pPlayer1;  
	CComponent*				m_pPlayer2; 

	D3DXMATRIX				m_vView;
	D3DXVECTOR3*			m_pAt;

	bool					m_bOverlapProtect[OP_END];
	bool					m_bDirect[DIR_END];
	D3DXVECTOR3				m_vDirectTarget[DIR_END];

	D3DXVECTOR3				m_OriPos;

	bool					m_bStage1_ViewBossAction;
	bool					m_bStage1_View_Volcano;
	bool					m_bStage1_View_Boss_Ready;

	bool					m_bShake;

	float					m_fShakeTimer;
	float					m_fTime;

	int						m_fShakeCount;

	bool					m_bBoss1Shake;

	bool					m_bPlayer1Delete;
	bool					m_bOzoom;
public:
	bool	GetOzoom() {return m_bOzoom;}
	void	SetOzoom(bool _Ozoom) {m_bOzoom = _Ozoom;}
public:
	bool	GetPlayer1Delete() {return m_bPlayer1Delete;}
	void	SetPlayer1Delete(bool _Delete) {m_bPlayer1Delete = _Delete;}
	
	bool&	GetStage2BossStart(void){ return m_bDirect[DIR_STAGE2_BOSS_START];}

public:
	bool*	GetBossShake();
	bool*	GetDirectState();
	void	SetTime(float _fTime);
public:
	void Strafe(float _fUnits); // Left  | Right
	void Fly(float _fUnits);	// Up	 | Down
	void Walk(float _fUnits);	// Front | Behind

	void Pitch(float _fAngle);  // Right Vector Rotation
	void Yaw(float _fAngle);	// Up Vector Rotation
	void Roll(float _fAngle);	// Front Vector Rotation

	void pitch(float angle);
	void rotateY(float angle);
	D3DXVECTOR3 rotateAroundAxis(const D3DXVECTOR3& vec, const D3DXVECTOR3& axis, float angle);

	void SetStage2BossView();
public:
	void Stage1BasicView();
	void Stage2BasicView();

	void Stage1BossView();
	void Stage2BossView();

	void DivisionCamera();




public:
	void OpeningAction(float _fTime);
	bool Stage1BossAction(float _fTime);
	void Stage2BossAction(float _fTime);

	bool& GetBossAction(void){ return m_bStage1_ViewBossAction; }
	bool& GetBossReady(void){ return m_bStage1_View_Boss_Ready;}
	bool& GetViewVolcano(void){ return m_bStage1_View_Volcano; }

public:
	void GetViewMatrix(D3DXMATRIX*	_pMat, D3DXVECTOR3*	_pAt);

	void Player1ViewMatrix(D3DXMATRIX*	_pMat);
	void Player2ViewMatrix(D3DXMATRIX*	_pMat);

	void Player1BossView(D3DXMATRIX*	_pMat);
	void Player2BossView(D3DXMATRIX*	_pMat);

	void SetPlayerCamPos(D3DXVECTOR3*	_pPos);
	void SetCameraType(CameraType	_Type);

	void GetPos(D3DXVECTOR3*	_pPos);
	void SetPos(D3DXVECTOR3*	_pPos);

	void GetRight(D3DXVECTOR3*	_pRight);
	void GetUp(D3DXVECTOR3*		_pUp);
	void GetLook(D3DXVECTOR3*	_pLook);

public:
	D3DXVECTOR3* GetAt();
	D3DXVECTOR3* GetTarget(){return m_vDirectTarget;}
	ENGINE::CAM_INFO* GetInfo();
	D3DXMATRIX*	 GetView();

	DisplayType	GetDisType();
	void SetDisType(DisplayType _eType);

	DirectType GetDirectType();
	void SetDirectType(DirectType _eType);

	void SetPlayer1(CComponent*	_pPlayer1);
	void SetPlayer2(CComponent*	_pPlayer2);

public:
	void CamInit(LPDIRECT3DDEVICE9 pDevice);

	void Stage1InitCam(void);
	void Stage2InitCam(void);

public:
	bool ShakeCamera(D3DXVECTOR3*	_ViewTarget);
	bool Stage1BossShakeCamera();

private:
	explicit CCameraMgr(void);
public:
	~CCameraMgr(void);
};

END
#endif // CameraMgr_h__