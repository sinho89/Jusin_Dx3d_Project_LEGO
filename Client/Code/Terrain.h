/*!
 * \file Terrain.h
 * \date 2016/08/17 15:25
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

#ifndef Terrain_h__
#define Terrain_h__

#include "TerrainObject.h"

class CTerrain : public CTerrainObject
{
private:
	wstring					m_wstrResourceKey;
	int						m_VtxCntX;
	int						m_VtxCntZ;
	int						m_Itv;
	float					m_fAngle;

private:
	ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;
	DWORD					m_dwVtxCnt;

public:
	DWORD*				GetVtxCnt();
	ENGINE::VTXTEX*		GetVtxTex();
	ENGINE::VTXTEX*		GetVtxConvertTex();
	int*				GetVtxCntX();
	int*				GetVtxCntZ();
	int*				GetVtxItv();

private:
	virtual		HRESULT	Initialize(void);
	void		Release(void);
	void		KeyInput(void);
	void		SetDirection(void);
	HRESULT		AddComponent(void);

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv);
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv, const wstring& wstrResourceKey);

public:
	virtual void			Update(void);
	virtual void			Render(void);

public:
	virtual const ENGINE::VTXTEX* GetTerrainVertex(void);

private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv);
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice, int _iCntX, int _iCntZ, int _iItv, const wstring& wstrResourceKey);

public:
	~CTerrain();
};


#endif // Terrain_h__