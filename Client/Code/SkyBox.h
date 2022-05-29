#ifndef SkyBox_h__
#define SkyBox_h__

#include "CubeObject.h"


class CTerrainCol;
class CSkyBox : public CCubeObject
{
private:

	D3DXVECTOR3				m_vDestPos;


	bool					JumpAble;
	float					m_Gravity;
	bool					m_bCollision;

	//ENGINE::VTXCUBE*		m_pVertex;
	//ENGINE::VTXCUBE*		m_pConvertVertex;
	//DWORD					m_dwVtxCnt;

private:
	HRESULT				Initialize(void);
	void				KeyInput(void);
	HRESULT				AddComponent(void);
	void				SetTransform(void);
public:
	static	CSkyBox*	Create(LPDIRECT3DDEVICE9 pDevice);

	void				Update(void);
	void				Render(void);
	void				Release(void);
	void				Jump(void);
	void				CollisionTerrain(void);
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9		pDevice);
public:
	~CSkyBox();

};

#endif // SkyBox_h__
