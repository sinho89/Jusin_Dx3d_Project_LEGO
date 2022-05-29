#include "stdafx.h"
#include "RcObject.h"
#include "Transform.h"

CRcObject::CRcObject( LPDIRECT3DDEVICE9 pDevice )
:CClientObj(pDevice)
{

}

CRcObject::~CRcObject()
{

}

void CRcObject::Update( void )
{

}

void CRcObject::Render( void )
{

}

HRESULT CRcObject::Initialize( void )
{
	return S_OK;
}