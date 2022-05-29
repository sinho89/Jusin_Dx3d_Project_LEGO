#ifndef CubeObject_h__
#define CubeObject_h__

#include "ClientObj.h"

class CCubeObject : public CClientObj
{
protected:
	explicit CCubeObject(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CCubeObject();
public:
	virtual void 	Update(void);
	virtual void 	Render(void);

protected:
	virtual HRESULT	Initialize(void);

private:
	void			Release(void);

public:
	void				SetInterver(ENGINE::SAVECUBE*);
	void				SetDirection(void);
	D3DXMATRIX*			GetMatrix(void){ return m_mMat; };

	ENGINE::VTXCUBE*	GetConvertVertex(void){ return m_pConvertVertex; }

protected:
	ENGINE::VTXCUBE*		m_pVertex;
	ENGINE::VTXCUBE*		m_pConvertVertex;
	DWORD					m_dwVtxCnt;
	D3DXMATRIX				m_mMat[ENGINE::MAT_END];
	float					m_fItv[ENGINE::ITV_END];
	float					m_fSpeed;
};

#endif // CubeObject_h__
