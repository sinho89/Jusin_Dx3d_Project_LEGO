#ifndef TerrainObject_h__
#define TerrainObject_h__

#include "ClientObj.h"

class CTerrainObject : public CClientObj
{

protected:
	explicit CTerrainObject(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CTerrainObject();
public:
	virtual void Update(void);
	virtual void Render(void);


private:
	virtual HRESULT	Initialize(void);
	void			Release(void);
};

#endif // TerrainObject_h__
