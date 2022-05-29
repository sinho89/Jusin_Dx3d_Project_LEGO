#ifndef LogoBack_h__
#define LogoBack_h__

#include "RcObject.h"



class CLogoBack : public CRcObject
{
private:
	explicit CLogoBack(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CLogoBack(void);

public:
	virtual void Update(void);
	virtual void Render(void);
	virtual D3DXMATRIX*	GetWorldMatrix(void);
	virtual D3DXMATRIX* GetViewMatrix(void);
	virtual D3DXMATRIX* GetProjMatrix(void);
	virtual LPDIRECT3DDEVICE9* GetDevice(void);

private:
	virtual HRESULT	Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

public:
	static CLogoBack* Create(LPDIRECT3DDEVICE9 pDevice);

};



#endif // LogoBack_h__
