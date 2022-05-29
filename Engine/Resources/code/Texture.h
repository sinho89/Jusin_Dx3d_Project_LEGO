/*!
 * \file Texture.h
 * \date 2016/07/15 11:35
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

#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(ENGINE)

class ENGINE_DLL CTexture : public CResources
{
private:
	typedef vector<IDirect3DBaseTexture9*>		VECTEXTURE;
	VECTEXTURE		m_vecTexture;

	DWORD			m_dwContainerSize;

private:
	explicit CTexture(LPDIRECT3DDEVICE9	pDevice);
public:
	virtual ~CTexture(void);

private:
	HRESULT	LoadTexture(TEXTURETYPE _eTexture, const wstring& wstrFilePath, const WORD& wCnt);

public:
	void Render(const DWORD& iIndex);
	void Render(void);

	static CTexture* Create(LPDIRECT3DDEVICE9 pDevice, TEXTURETYPE _eTexture, 
		const wstring& wstrFilePath, const WORD& wCnt);

	void Release(void);

	virtual CResources* CloneResource(void);
};

END
#endif // Texture_h__