#ifndef RcObject_h__
#define RcObject_h__

#include "ClientObj.h"

class CRcObject : public CClientObj
{

protected:
	explicit CRcObject(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CRcObject();
public:
	virtual void Update(void);
	virtual void Render(void);

private:
	virtual HRESULT	Initialize(void);

};

#endif // RcObject_h__
