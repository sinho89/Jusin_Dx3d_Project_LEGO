#ifndef ClientObj_h__
#define ClientObj_h__

#include "Export_Function.h"
#include "GameObject.h"
#include "ResourcesMgr.h"
#include "Management.h"
#include "InfoSubject.h"
#include "Transform.h"
#include "TimeMgr.h"
#include "Texture.h"
#include "VIBuffer.h"
#include "LayerMgr.h"
#include "SoundMgr.h"

namespace ENGINE
{
	class CSoundMgr;
}

class CClientObj : public  ENGINE::CGameObject
{
protected:
	explicit CClientObj(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CClientObj();

public:
	virtual void Update(void);
	virtual void Render(void);
	virtual D3DXMATRIX*	GetWorldMatrix(void);
	virtual D3DXMATRIX* GetViewMatrix(void);
	virtual D3DXMATRIX* GetProjMatrix(void);
	virtual LPDIRECT3DDEVICE9* GetDevice(void);

private:
	virtual HRESULT	Initialize(void);
	void			Release(void);

public:
	void	SetInfo(ENGINE::SAVECUBE*);

protected:
	ENGINE::CVIBuffer*					m_pBuffer;
	ENGINE::CTexture*					m_pTexture;
	ENGINE::CResourcesMgr*				m_pResourcesMgr;
	ENGINE::CTimeMgr*					m_pTimeMgr;
	ENGINE::CManagement*				m_pManagement;
	ENGINE::CCameraMgr*					m_pCameraMgr;
	ENGINE::CLayerMgr*					m_pLayerMgr;
	ENGINE::CSoundMgr*					m_pSoundMgr;

public:
	inline ENGINE::CTransform*	GetInfo(void){return m_pInfo;}
	inline void					SetPos(D3DXVECTOR3& pv){m_pInfo->m_vPos = pv;}
};

#endif // ClientObj_h__
