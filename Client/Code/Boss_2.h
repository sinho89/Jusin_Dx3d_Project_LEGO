/*!
 * \file Boss_2.h
 * \date 2016/08/29 20:54
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
#ifndef Boss_2_h__
#define Boss_2_h__

#include "Unit.h"

class CBoss_2 : public CUnit
{
private:
	explicit CBoss_2(LPDIRECT3DDEVICE9 pDevice);
public:
	~CBoss_2(void);

private:
	virtual HRESULT 		Initialize(void);
	virtual HRESULT			AddComponent(void);
	void					Release(void);

public:
	virtual void	 		Update(void);
	virtual void	 		Render(void);

public:
	static CBoss_2*			Create(LPDIRECT3DDEVICE9 pDevice);

private:
	ENGINE::VTXTEX*			m_pVertex;
	ENGINE::VTXTEX*			m_pConvertVertex;
	DWORD					m_dwVtxCnt;
};

#endif // Boss_2_h__