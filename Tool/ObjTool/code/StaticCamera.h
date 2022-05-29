

#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Camera.h"

namespace ENGINE
{
	class CTimeMgr;
	class CTransform;
	class CInfoSubject;
}

class CStaticCamera : public ENGINE::CCamera
{
private:
	explicit	CStaticCamera(LPDIRECT3DDEVICE9 pDevice);
public:
	~CStaticCamera();

public:
	static  CStaticCamera*	Create(LPDIRECT3DDEVICE9 pDevice, const ENGINE::CTransform* pTargetInfo);
	void SetCameraTarget(const ENGINE::CTransform* pTargetInfo);

public:
	virtual HRESULT	Initialize(void);
	virtual void	Update(void);

public:
	void	KeyCheck(void);
	void	TargetRenewal(void);

private:
	ENGINE::CTimeMgr*				m_pTimeMgr;
	ENGINE::CInfoSubject*			m_pInfoSubject;
	const ENGINE::CTransform*		m_pTargetInfo;

private:
	float		m_fTargetDistance;
	float		m_fCameraSpeed;
	float		m_fAngle;

};

#endif // StaticCamera_h__